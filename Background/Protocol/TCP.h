//
// Created by Deva on 2017/5/3.
//

#ifndef NETDUMP_TCP_H
#define NETDUMP_TCP_H


#include "Protocol.h"

class TCP : public Protocol {
public:
    u_short src_port;
    u_short dst_port;
    u_int seq;
    u_int ack;
    u_short hdr_len;
    u_short flags;
    u_short window_size;
    u_short checksum;
    u_short urgent_pointer;

    TCP();

    void show_info();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_TCP_H
