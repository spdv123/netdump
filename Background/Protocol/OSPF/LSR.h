//
// Created by Deva on 2017/5/9.
//

#ifndef NETDUMP_LSR_H
#define NETDUMP_LSR_H


#include "base.h"

class LSR : public base {
public:
    u_int ls_type;
    BYTE link_state_id[4];
    BYTE advertising_router[4];

    LSR();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_LSR_H
