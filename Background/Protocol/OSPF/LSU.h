//
// Created by Deva on 2017/5/9.
//

#ifndef NETDUMP_LSU_H
#define NETDUMP_LSU_H


#include "base.h"
#include "LSAck.h"

class LSU : public base {
public:
    u_int number_of_lsas;
    LSAck *lsas;

    ~LSU();

    LSU();

    picojson::object get_json();

    void parse_data(const BYTE *data, u_int length);
};


#endif //NETDUMP_LSU_H
