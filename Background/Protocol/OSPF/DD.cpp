//
// Created by Deva on 2017/5/9.
//

#include "DD.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

extern void parse_ip(const BYTE *ip, char *buf);

DD::DD() {
    ospf_type = _DD;
}

picojson::object DD::get_json() {
    picojson::object dd;
    dd["interface_mtu"] = picojson::value((double) interface_mtu);
    dd["options"] = picojson::value((double) options);
    dd["db_description"] = picojson::value((double) db_description);
    dd["dd_sequence"] = picojson::value((double) dd_sequence);
    return dd;
}

void DD::parse_data(const BYTE *data, u_int length) {
    interface_mtu = pp_short(data);
    options = data[2];
    db_description = data[3];
    dd_sequence = pp_int(data + 4);
}
