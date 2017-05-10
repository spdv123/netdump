//
// Created by Deva on 2017/4/14.
//

#ifndef NETDUMP_PROTOCOL_H
#define NETDUMP_PROTOCOL_H


typedef unsigned int u_int;
typedef unsigned char BYTE;
typedef unsigned short u_short;
typedef double u_double;

#include <cstdio>
#include <cstring>
#include <string>
#include "../picojson.h"

enum {
    _UNKNOWN = 0x00,
    _ARP = 0x01,
    _TCP = 0x02,
    _UDP = 0x03,
    _ICMP = 0x04,
    _OSPF = 0x05
};

struct ipv4_hdr {
    u_short hdr_len;
    BYTE DSCP;
    u_short total_len;
    u_short identification;

    BYTE flags; // 3 bits
    u_short fragment; // 13bits, 0 for flags[DF] set

    BYTE ttl;
    BYTE protocol; // tcp udp
    u_short hdr_checksum;

    BYTE ip_src[4];
    BYTE ip_dst[4];

};

class Protocol {
public:
    u_int type;
    ipv4_hdr ipv4Hdr;

    Protocol();

    virtual ~Protocol();

    virtual void show_info();

    virtual picojson::object get_json();

    virtual void parse_data(const BYTE *data, u_int length);
};


u_short pp_short(const BYTE *data);

u_int pp_int(const BYTE *data);

void parse_mac(const BYTE *mac, char *buf);

void parse_ip(const BYTE *ip, char *buf);

u_double pp_time(const BYTE *data);

#endif //NETDUMP_PROTOCOL_H
