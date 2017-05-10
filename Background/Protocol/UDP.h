//
// Created by Deva on 2017/5/5.
//

#ifndef NETDUMP_UDP_H
#define NETDUMP_UDP_H


#include "Protocol.h"

class UDP : public Protocol {
public:
    u_short src_port;
    u_short dst_port;
    u_short length;
    u_short checksum;

    UDP();

    void show_info();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_UDP_H
