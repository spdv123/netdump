//
// Created by Deva on 2017/5/9.
//

#include "LSU.h"

LSU::LSU() {
    ospf_type = _LSU;
    lsas = NULL;
}

picojson::object LSU::get_json() {
    picojson::object lsu;

    lsu["number_of_lsas"] = picojson::value((double) number_of_lsas);
    char buf[100];

    for (int i = 0; i < number_of_lsas; i++) {
        sprintf(buf, "lsa_%d", i + 1);
        lsu[buf] = picojson::value(lsas[i].get_json());
    }

    return lsu;
}

void LSU::parse_data(const BYTE *data, u_int length) {
    number_of_lsas = pp_int(data);
    lsas = new LSAck[number_of_lsas];
    for (int i = 0; i < number_of_lsas; i++) {
        lsas[i].parse_data(data + 4 + 36 * i, 36);
    }
}

LSU::~LSU() {
    if (lsas != NULL) {
        delete[] lsas;
        lsas = NULL;
    }
}
