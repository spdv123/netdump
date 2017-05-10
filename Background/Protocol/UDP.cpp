//
// Created by Deva on 2017/5/5.
//

#include "UDP.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

UDP::UDP() {
    type = _UDP;
}

void UDP::show_info() {
    puts("I'm UDP!");
}

picojson::object UDP::get_json() {
    picojson::object v = Protocol::get_json();
    picojson::object udp;
    udp["src_port"] = picojson::value((double) src_port);
    udp["dst_port"] = picojson::value((double) dst_port);
    udp["length"] = picojson::value((double) this->length);
    udp["checksum"] = picojson::value((double) checksum);
    v["udp"] = picojson::value(udp);
    return v;
}

void UDP::parse_data(const BYTE *data, u_int length) {
    Protocol::parse_data(data, length);

    src_port = pp_short(data + 34);
    dst_port = pp_short(data + 36);
    this->length = pp_short(data + 38);
    checksum = pp_short(data + 40);
}
