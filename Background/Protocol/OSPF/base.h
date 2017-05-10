//
// Created by Deva on 2017/5/5.
//

#ifndef NETDUMP_BASE_H
#define NETDUMP_BASE_H

#include "../Protocol.h"

enum {
    _HELLO = 0x01,
    _DD = 0x02,
    _LSR = 0x03,
    _LSU = 0x04,
    _LSAck = 0x05
};

class base {
public:
    u_int ospf_type;

    base();
    virtual ~base();

    virtual picojson::object get_json();

    virtual void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_BASE_H
