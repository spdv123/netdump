//
// Created by Deva on 2017/4/14.
//

#ifndef NETDUMP_ARP_H
#define NETDUMP_ARP_H


#include "Protocol.h"

class ARP : public Protocol {
public:
    u_short hw_type;
    u_short proto_type;
    BYTE hw_size;
    BYTE proto_size;
    u_short opcode;
    BYTE src_mac[6];
    BYTE src_ip[4];
    BYTE dst_mac[6];
    BYTE dst_ip[4];

    ARP();

    void show_info();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_ARP_H
