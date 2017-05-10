//
// Created by Deva on 2017/5/3.
//

#include "TCP.h"
extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

void TCP::show_info() {
    puts("I'm TCP!");
}

picojson::object TCP::get_json() {
    picojson::object v = Protocol::get_json();
    picojson::object tcp;
    tcp["src_port"] = picojson::value((double) src_port);
    tcp["dst_port"] = picojson::value((double) dst_port);
    tcp["seq"] = picojson::value((double) seq);
    tcp["ack"] = picojson::value((double) ack);
    tcp["hdr_len"] = picojson::value((double) hdr_len);
    tcp["flags"] = picojson::value((double) flags);
    tcp["window_size"] = picojson::value((double) window_size);
    tcp["checksum"] = picojson::value((double) checksum);
    tcp["urgent_pointer"] = picojson::value((double) urgent_pointer);
    v["tcp"] = picojson::value(tcp);
    return v;
}

void TCP::parse_data(const BYTE *data, u_int length) {
    Protocol::parse_data(data, length);

    src_port = pp_short(data + 34);
    dst_port = pp_short(data + 36);
    seq = pp_int(data + 38);
    ack = pp_int(data + 42);
    hdr_len = (u_short) ((data[46] >> 4) * 4);
    flags = (u_short) (((data[46] & 0xf) << 8) + data[47]);
    window_size = pp_short(data + 48);
    checksum = pp_short(data + 50);
    urgent_pointer = pp_short(data + 52);
}

TCP::TCP() {
    type = _TCP;
}
