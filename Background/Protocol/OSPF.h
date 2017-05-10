//
// Created by Deva on 2017/5/5.
//

#ifndef NETDUMP_OSPF_H
#define NETDUMP_OSPF_H

#include "Protocol.h"
#include "OSPF/base.h"
#include "OSPF/hello.h"
#include "OSPF/DD.h"
#include "OSPF/LSR.h"
#include "OSPF/LSU.h"
#include "OSPF/LSAck.h"

class OSPF : public Protocol {
public:
    BYTE version;
    BYTE msg_type;
    u_short packet_length;
    BYTE src_router[4];
    BYTE area_id[4];
    u_short checksum;
    u_short auth_type;
    BYTE auth_crypt_key_id;
    BYTE auth_crypt_data_length;
    u_int auth_crypt_seq_nbr;
    BYTE *auth_crypt_data;

    base *ospfProtocol;

    OSPF();

    ~OSPF();

    void get_protocol(char * buf);

    void show_info();

    virtual picojson::object get_json();

    virtual void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_OSPF_H
