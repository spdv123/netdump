//
// Created by Deva on 2017/4/12.
//

#ifndef NETDUMP_PACK_H
#define NETDUMP_PACK_H

#include <sys/time.h>
#include <string>
#include "Protocol/Protocol.h"
#include "Protocol/ARP.h"
#include "Protocol/TCP.h"
#include "Protocol/UDP.h"
#include "Protocol/ICMP.h"
#include "Protocol/OSPF.h"
#include "socket/socklib.h"


class Pack {
public:
    // Ethernet
    BYTE mac_dst[6];
    BYTE mac_src[6];
    u_short type;

    BYTE * data;
    u_int data_length;

    Pack();

    ~Pack();

    Pack(register const BYTE *package, register u_int length);

    void viewInfo();

    Protocol *mProtocol;

    void sendInfo(Socket& mSocket);

private:
    struct timeval pack_time;

    std::string printable_raw_data();

    std::string get_info();
};

#endif //NETDUMP_PACK_H
