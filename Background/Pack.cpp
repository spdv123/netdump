//
// Created by Deva on 2017/4/12.
//

#include <cstdio>
#include <ctime>
#include "cstring"
#include "Pack.h"
#include <string>
#include "picojson.h"

extern void parse_mac(const BYTE *mac, char *buf);

Pack::Pack(register const BYTE *package, register u_int length) {
    gettimeofday(&pack_time, NULL);
    data = NULL;

    data_length = length;
    data = new BYTE[length];
    memcpy(data, package, length);

    if (length < 14)
        return;
    memcpy(mac_dst, package, sizeof(BYTE) * 6);
    memcpy(mac_src, package + 6, sizeof(BYTE) * 6);
    type = (package[12] << 8) + package[13];

    u_short protocol_type;
    switch (type) {
        case 0x0800:
            protocol_type = package[23];
            switch (protocol_type) {
                case 0x01:
                    mProtocol = new ICMP();
                    break;
                case 0x06:
                    mProtocol = new TCP();
                    break;
                case 0x11:
                    mProtocol = new UDP();
                    break;
                case 0x59:
                    mProtocol = new OSPF();
                    break;
                default:
                    mProtocol = new Protocol();
                    break;
            }
            break;
        case 0x0806:
            mProtocol = new ARP();
            break;
        default:
            mProtocol = NULL;
            break;
    }
    if (mProtocol != NULL) {
        mProtocol->parse_data(package, length);
    }
}

void addTabs() {
    printf("\t\t");
}

bool printable(char c) {
    return c > 32 && c < 127;
}

void Pack::viewInfo() {
    std::string json = get_info();
    const char *json_ = json.c_str();

    printf("%s\n", json_);
}

std::string Pack::printable_raw_data() {
    std::string nice;

    char tmp[10];
    int lines = data_length / 16 + (data_length % 16 ? 1 : 0);
    for (int i = 0; i < lines; i++) {
        int outBytes = 16;
        if (i == lines - 1) outBytes = data_length % 16;
        if (outBytes == 0) outBytes = 16;

        int printed = 0;
        for (int j = 0; j < outBytes; j++) {
            sprintf(tmp, "%02x ", data[i * 16 + j]);
            nice += tmp;
            printed += 3;
            if (j == 8) {
                nice += ' ';
                printed++;
            }
        }

        for (int j = 0; j < 52 - printed; j++) nice += ' ';;
        for (int j = 0; j < outBytes; j++) {
            sprintf(tmp, "%c", printable(data[i * 16 + j]) ? data[i * 16 + j] : '.');
            nice += tmp;
            if (j == 8) nice += ' ';;
        }
        nice += '\n';
    }

    return nice;
}

Pack::~Pack() {
    if (data != NULL) {
        delete[] data;
        data = NULL;
    }
    if (mProtocol != NULL) {
        delete mProtocol;
        mProtocol = NULL;
    }
}

Pack::Pack() {
    mProtocol = NULL;
}

void Pack::sendInfo(Socket &mSocket) {
    if (data_length < 14) {
        puts("Error packet!");
        return;
    }
    std::string json = get_info();
    json += '\n';
    try {
        mSocket.sendMesg(mSocket, json);
    } catch (...) {
        puts("Socket error!");
        exit(2);
    }
}

std::string Pack::get_info() {
    picojson::object v;
    char timebuf[64], timebuf2[128];
    strftime(timebuf, sizeof(timebuf) - 1, "%Y-%m-%d %H:%M:%S", localtime(&pack_time.tv_sec));
    sprintf(timebuf2, "%s.%ld", timebuf, (long int) pack_time.tv_usec);
    v["time"] = picojson::value(timebuf2);

    char dst_mac[40], src_mac[40];
    parse_mac(mac_dst, dst_mac);
    v["dst_mac"] = picojson::value(dst_mac);

    parse_mac(mac_src, src_mac);
    v["src_mac"] = picojson::value(src_mac);

    char protocol_buf[100];
    if (mProtocol != NULL) {
        switch (mProtocol->type) {
            case _ARP:
                v["protocol"] = picojson::value("ARP");
                v["arp"] = picojson::value(mProtocol->get_json());
                break;
            case _TCP:
                v["protocol"] = picojson::value("TCP");
                v["ip"] = picojson::value(mProtocol->get_json());
                break;
            case _UDP:
                v["protocol"] = picojson::value("UDP");
                v["ip"] = picojson::value(mProtocol->get_json());
                break;
            case _ICMP:
                v["protocol"] = picojson::value("ICMP");
                v["ip"] = picojson::value(mProtocol->get_json());
                break;
            case _OSPF:
                ((OSPF *) mProtocol)->get_protocol(protocol_buf);
                v["protocol"] = picojson::value(protocol_buf);
                v["ip"] = picojson::value(mProtocol->get_json());
                break;
            case _UNKNOWN:
                v["protocol"] = picojson::value("UNKNOWN_IPv4");
                v["ip"] = picojson::value(mProtocol->get_json());
                break;
            default:
                v["protocol"] = picojson::value("UNKNOWN");
                break;
        }
    } else {
        v["protocol"] = picojson::value("UNKNOWN");
    }

    v["data"] = picojson::value(printable_raw_data());
    v["len"] = picojson::value((double) data_length);
    std::string json = picojson::value(v).serialize();
    return json;
}
