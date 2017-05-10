//
// Created by Deva on 2017/5/5.
//

#include "base.h"

base::base() {
    ospf_type = _UNKNOWN;
}

picojson::object base::get_json() {
    return picojson::object();
}

void base::parse_data(const BYTE *data, u_int length) {

}

base::~base() {

}
