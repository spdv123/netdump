//
// Created by Deva on 2017/4/14.
//

#include "Protocol.h"

void Protocol::show_info() {
    printf("Unknown protocol\n");
}

Protocol::Protocol() {
    type = _UNKNOWN;
    //puts("change type to unknown");
}


void Protocol::parse_data(const BYTE *data, u_int length) {

    ipv4Hdr.hdr_len = (u_short) ((data[14] & 0xf) * 4);
    ipv4Hdr.DSCP = data[15];
    ipv4Hdr.total_len = pp_short(data + 16);
    ipv4Hdr.identification = pp_short(data + 18);
    ipv4Hdr.flags = (data[20] >> 5);
    ipv4Hdr.fragment = (u_short) (((data[20] & 0b11111) << 8) + data[21]);
    ipv4Hdr.ttl = data[22];
    ipv4Hdr.protocol = data[23];
    ipv4Hdr.hdr_checksum = pp_short(data + 24);
    memcpy(ipv4Hdr.ip_src, data + 26, 4);
    memcpy(ipv4Hdr.ip_dst, data + 30, 4);
}

Protocol::~Protocol() {

}

picojson::object Protocol::get_json() {
    picojson::object v;

    v["hdr_len"] = picojson::value((double) ipv4Hdr.hdr_len);
    v["DSCP"] = picojson::value((double) ipv4Hdr.DSCP);
    v["total_len"] = picojson::value((double) ipv4Hdr.total_len);
    v["identification"] = picojson::value((double) ipv4Hdr.identification);
    v["flags"] = picojson::value((double) ipv4Hdr.flags);
    v["fragment"] = picojson::value((double) ipv4Hdr.fragment);
    v["ttl"] = picojson::value((double) ipv4Hdr.ttl);
    v["protocol"] = picojson::value((double) ipv4Hdr.protocol);
    v["hdr_checksum"] = picojson::value((double) ipv4Hdr.hdr_checksum);
    char src_ip[15] = {0}, dst_ip[15] = {0};
    parse_ip(ipv4Hdr.ip_src, src_ip);
    parse_ip(ipv4Hdr.ip_dst, dst_ip);
    v["src_ip"] = picojson::value(src_ip);
    v["dst_ip"] = picojson::value(dst_ip);

    return v;
}

u_short pp_short(const BYTE *data) {
    return (data[0] << 8) + data[1];
}

u_int pp_int(const BYTE *data) {
    return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
}

void parse_mac(const BYTE *mac, char *buf) {
    for (int i = 0; i < 6; i++) {
        sprintf(buf + i * 3, "%02x%c", mac[i], i == 5 ? '\0' : ':');
    }
}

void parse_ip(const BYTE *ip, char *buf) {
    for (int i = 0; i < 4; i++) {
        unsigned int l = strlen(buf);
        sprintf(buf + l, "%d%c", ip[i], i == 3 ? '\0' : '.');
    }
}

u_double pp_time(const BYTE *data) {
    // 解析8位时间戳
    u_double ret = 0.0;
    u_int time_1 = (data[3] << 24) + (data[2] << 16) + (data[1] << 8) + data[0];
    u_int time_2 = (data[7] << 24) + (data[6] << 16) + (data[5] << 8) + data[4];
    char buf[30];
    sprintf(buf, "%d.%d", time_1, time_2);
    sscanf(buf, "%lf", &ret);
    // printf("Test: %d.%d %lf\n", time_1, time_2, ret);
    return ret;
}