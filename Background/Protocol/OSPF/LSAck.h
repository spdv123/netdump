//
// Created by Deva on 2017/5/9.
//

#ifndef NETDUMP_LSACK_H
#define NETDUMP_LSACK_H


#include "base.h"

class LSAck : public base {
public:
    BYTE donotage;
    u_int age;
    BYTE options;
    BYTE ls_type;
    BYTE link_state_id[4];
    BYTE advertising_router[4];
    u_int seqnum;
    u_short checksum;
    u_short length;

    LSAck();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_LSACK_H
