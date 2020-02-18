/*  _  __  ____    _   _
 * | |/ / |  _ \  | \ | |
 * | ' /  | |_) | |  \| |
 * | . \  |  __/  | |\  |
 * |_|\_\ |_|     |_| \_|
 *
 * (c) 2020 KPN
 * License: MIT License.
 * Author: Joseph Verburg, Jan Bogaerts 
 *
 * support for string sensors
 */

#include "senml_string_record.h"
#include "../../cbor/cbor.h"
#include "../senml_helpers.h"
#include <string.h>

int SenMLStringRecord::fieldsToJson() {
    int res = SenMLRecord::fieldsToJson();
    res += printText(F(",\"vs\":\""), 7);
    const char *val = this->get();
    res += printText(val, strlen(val));
    res += printText(F("\""), 1);
    return res;
}
int SenMLStringRecord::fieldsToCbor() {
    int res = SenMLRecord::fieldsToCbor();
    res += cbor_serialize_int(SENML_CBOR_VS_LABEL);
    res += cbor_serialize_unicode_string(this->get());
    return res;
}
