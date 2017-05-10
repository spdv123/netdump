//
// Created by Deva on 2017/5/9.
//

#include "LSAck.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

extern void parse_ip(const BYTE *ip, char *buf);

LSAck::LSAck() {
    ospf_type = _LSAck;
}

picojson::object LSAck::get_json() {
    picojson::object lsack;

    lsack["donotage"] = picojson::value((double) donotage);
    lsack["age"] = picojson::value((double) age);
    lsack["options"] = picojson::value((double) options);
    lsack["ls_type"] = picojson::value((double) ls_type);
    lsack["seqnum"] = picojson::value((double) seqnum);
    lsack["checksum"] = picojson::value((double) checksum);
    lsack["length"] = picojson::value((double) length);

    char ip1[20] = {0}, ip2[20] = {0};
    parse_ip(link_state_id, ip1);
    parse_ip(advertising_router, ip2);
    lsack["link_state_id"] = picojson::value(ip1);
    lsack["advertising_router"] = picojson::value(ip2);

    return lsack;
}

void LSAck::parse_data(const BYTE *data, u_int length) {
    donotage = data[0] >> 3;
    age = pp_int(data);
    age &= 0x7fff;
    options = data[2];
    ls_type = data[3];

    memcpy(link_state_id, data + 4, 4);
    memcpy(advertising_router, data + 8, 4);
    seqnum = pp_int(data + 12);
    checksum = pp_short(data + 16);
    this->length = pp_short(data + 18);
}
