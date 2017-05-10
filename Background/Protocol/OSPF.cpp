//
// Created by Deva on 2017/5/5.
//

#include "OSPF.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

extern void parse_ip(const BYTE *ip, char *buf);

void OSPF::show_info() {
    puts("I'm OSPF!");
}

picojson::object OSPF::get_json() {
    picojson::object v = Protocol::get_json();
    picojson::object ospf;

    ospf["version"] = picojson::value((double) version);
    ospf["msg_type"] = picojson::value((double) msg_type);
    ospf["packet_length"] = picojson::value((double) packet_length);
    ospf["checksum"] = picojson::value((double) checksum);
    ospf["auth_type"] = picojson::value((double) auth_type);
    ospf["auth_crypt_key_id"] = picojson::value((double) auth_crypt_key_id);
    ospf["auth_crypt_data_length"] = picojson::value((double) auth_crypt_data_length);
    ospf["auth_crypt_seq_nbr"] = picojson::value((double) auth_crypt_seq_nbr);

    char ip1[20] = {0}, ip2[20] = {0};
    parse_ip(src_router, ip1);
    parse_ip(area_id, ip2);
    ospf["src_router"] = picojson::value(ip1);
    ospf["area_id"] = picojson::value(ip2);

    if (ospfProtocol != NULL) {
        switch (msg_type) {
            case _HELLO:
                ospf["hello"] = picojson::value(ospfProtocol->get_json());
                break;
            case _DD:
                ospf["dd"] = picojson::value(ospfProtocol->get_json());
                break;
            case _LSR:
                ospf["lsr"] = picojson::value(ospfProtocol->get_json());
                break;
            case _LSU:
                ospf["lsu"] = picojson::value(ospfProtocol->get_json());
                break;
            case _LSAck:
                ospf["lsa"] = picojson::value(ospfProtocol->get_json());
                break;
            default:
                break;
        }
    }

    v["ospf"] = picojson::value(ospf);
    return v;
}

void OSPF::parse_data(const BYTE *data, u_int length) {
    Protocol::parse_data(data, length);

    version = data[34];
    msg_type = data[35];
    packet_length = pp_short(data + 36);
    memcpy(src_router, data + 38, 4);
    memcpy(area_id, data + 42, 4);
    checksum = pp_short(data + 46);
    auth_type = pp_short(data + 48);
    // 这里空了2 bytes
    auth_crypt_key_id = data[52];
    auth_crypt_data_length = data[53];
    auth_crypt_seq_nbr = pp_int(data + 54);
    // TODO: parse auth_crypt_data and ospfProtocol ptr

    switch (msg_type) {
        case _HELLO:
            ospfProtocol = new hello();
            ospfProtocol->parse_data(data + 58, 20);
            break;
        case _DD:
            ospfProtocol = new DD();
            ospfProtocol->parse_data(data + 58, 8);
            break;
        case _LSR:
            ospfProtocol = new LSR();
            ospfProtocol->parse_data(data + 58, 12);
            break;
        case _LSU:
            ospfProtocol = new LSU();
            ospfProtocol->parse_data(data + 58, 40);
            break;
        case _LSAck:
            ospfProtocol = new LSAck();
            ospfProtocol->parse_data(data + 58, 36);
            break;
        default:
            break;
    }
}

OSPF::OSPF() {
    ospfProtocol = NULL;
    auth_crypt_data = NULL;
    type = _OSPF;
}

OSPF::~OSPF() {
    if (auth_crypt_data != NULL) {
        delete[] auth_crypt_data;
        auth_crypt_data = NULL;
    }
    if (ospfProtocol != NULL) {
        delete ospfProtocol;
        ospfProtocol = NULL;
    }
}

void OSPF::get_protocol(char *buf) {
    switch (msg_type) {
        case _HELLO:
            sprintf(buf, "OSPF Hello");
            break;
        case _DD:
            sprintf(buf, "OSPF DB Description");
            break;
        case _LSU:
            sprintf(buf, "OSPF LS Request");
            break;
        case _LSR:
            sprintf(buf, "OSPF LS Update");
            break;
        case _LSAck:
            sprintf(buf, "OSPF LS Acknowledge");
            break;
        default:
            sprintf(buf, "OSPF");
            break;
    }
}
