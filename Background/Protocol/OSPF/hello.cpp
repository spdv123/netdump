//
// Created by Deva on 2017/5/5.
//

#include "hello.h"

extern u_short pp_short(const BYTE *data);

extern u_int pp_int(const BYTE *data);

extern void parse_ip(const BYTE *ip, char *buf);

picojson::object hello::get_json() {
    picojson::object hello;
    hello["hello_interval"] = picojson::value((double) hello_interval);
    hello["options"] = picojson::value((double) options);
    hello["router_priority"] = picojson::value((double) router_priority);
    hello["router_dead_interval"] = picojson::value((double) router_dead_interval);

    char ip1[20] = {0}, ip2[20] = {0}, ip3[20] = {0};
    parse_ip(network_mask, ip1);
    parse_ip(designated_router, ip2);
    parse_ip(backup_designated_router, ip3);

    hello["network_mask"] = picojson::value(ip1);
    hello["designated_router"] = picojson::value(ip2);
    hello["backup_designated_router"] = picojson::value(ip3);

    return hello;
}

void hello::parse_data(const BYTE *data, u_int length) {
    // 这里以后的data是自带了基础偏移，所以不用绝对位置
    memcpy(network_mask, data, 4);
    hello_interval = pp_short(data + 4);
    options = data[6];
    router_priority = data[7];
    router_dead_interval = pp_int(data + 8);
    memcpy(designated_router, data + 12, 4);
    memcpy(backup_designated_router, data + 16, 4);
}

hello::hello() {
    ospf_type = _HELLO;
}
