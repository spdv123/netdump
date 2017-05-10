//
// Created by Deva on 2017/5/5.
//

#ifndef NETDUMP_ICMP_H
#define NETDUMP_ICMP_H


#include "Protocol.h"

class ICMP : public Protocol {
public:
    BYTE type1;
    BYTE code;
    u_short checksum;
    u_short ident;
    u_short seq;
    u_double timestamp;

    ICMP();

    void show_info();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_ICMP_H
