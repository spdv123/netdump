//
// Created by Deva on 2017/5/9.
//

#ifndef NETDUMP_DD_H
#define NETDUMP_DD_H


#include "base.h"

class DD : public base {
public:
    u_short interface_mtu;
    BYTE options;
    BYTE db_description;
    u_int dd_sequence;

    DD();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_DD_H
