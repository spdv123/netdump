//
// Created by Deva on 2017/4/14.
//

#include "ARP.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

extern void parse_mac(const BYTE *mac, char *buf);

extern void parse_ip(const BYTE *ip, char *buf);

void ARP::show_info() {
    puts("I'm arp!");
}

ARP::ARP() {
    type = _ARP;
    //puts("change type to arp");
    //raw_data = NULL;
}

void ARP::parse_data(const BYTE *data, u_int length) {
    hw_type = pp_short(data + 14);
    proto_type = pp_short(data + 16);
    hw_size = data[18];
    proto_size = data[19];
    opcode = pp_short(data + 20);
    memcpy(src_mac, data + 22, sizeof(BYTE) * 6);
    memcpy(src_ip, data + 28, sizeof(BYTE) * 4);
    memcpy(dst_mac, data + 32, sizeof(BYTE) * 6);
    memcpy(dst_ip, data + 38, sizeof(BYTE) * 4);
}

picojson::object ARP::get_json() {
    picojson::object v;

    v["hw_type"] = picojson::value((double) hw_type);
    v["proto_type"] = picojson::value((double) proto_type);
    v["hw_size"] = picojson::value((double) hw_size);
    v["proto_size"] = picojson::value((double) proto_size);
    v["opcode"] = picojson::value((double) opcode);

    char ip_buf1[15] = {0}, ip_buf2[15] = {0}, mac_buf1[30], mac_buf2[30];
    parse_ip(src_ip, ip_buf1);
    parse_ip(dst_ip, ip_buf2);
    v["src_ip"] = picojson::value(ip_buf1);
    v["dst_ip"] = picojson::value(ip_buf2);

    parse_mac(src_mac, mac_buf1);
    parse_mac(dst_mac, mac_buf2);
    v["src_mac"] = picojson::value(mac_buf1);
    v["dst_mac"] = picojson::value(mac_buf2);

    return v;
}
