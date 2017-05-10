//
// Created by Deva on 2017/5/9.
//

#include "LSR.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

extern void parse_ip(const BYTE *ip, char *buf);

LSR::LSR() {
    ospf_type = _LSR;
}

picojson::object LSR::get_json() {
    picojson::object lsr;
    lsr["hello_interval"] = picojson::value((double) ls_type);

    char ip1[20] = {0}, ip2[20] = {0};
    parse_ip(link_state_id, ip1);
    parse_ip(advertising_router, ip2);
    lsr["link_state_id"] = picojson::value(ip1);
    lsr["advertising_router"] = picojson::value(ip2);

    return lsr;
}

void LSR::parse_data(const BYTE *data, u_int length) {
    ls_type = pp_int(data);
    memcpy(link_state_id, data + 4, 4);
    memcpy(advertising_router, data + 8, 4);
}
