//
// Created by Deva on 2017/5/5.
//

#include "ICMP.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

extern u_double pp_time(const BYTE *data);

ICMP::ICMP() {
    type = _ICMP;
}

void ICMP::show_info() {
    puts("I'm ICMP!");
}

picojson::object ICMP::get_json() {
    picojson::object v = Protocol::get_json();
    picojson::object icmp;

    icmp["type"] = picojson::value((double) type1);
    icmp["code"] = picojson::value((double) code);
    icmp["checksum"] = picojson::value((double) checksum);
    icmp["ident"] = picojson::value((double) ident);
    icmp["seq"] = picojson::value((double) seq);
    icmp["timestamp"] = picojson::value(timestamp);

    v["icmp"] = picojson::value(icmp);
    return v;
}

void ICMP::parse_data(const BYTE *data, u_int length) {
    Protocol::parse_data(data, length);

    type1 = data[34];
    code = data[35];
    checksum = pp_short(data + 36);
    ident = pp_short(data + 38);
    seq = pp_short(data + 40);
    timestamp = pp_time(data + 42);
}
