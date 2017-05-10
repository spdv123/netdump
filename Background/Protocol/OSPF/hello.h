//
// Created by Deva on 2017/5/5.
//

#ifndef NETDUMP_HELLO_H
#define NETDUMP_HELLO_H


#include "base.h"

class hello : public base {
public:
    BYTE network_mask[4];
    u_short hello_interval;
    BYTE options;
    BYTE router_priority;
    u_int router_dead_interval;
    BYTE designated_router[4];
    BYTE backup_designated_router[4];

    hello();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_HELLO_H
