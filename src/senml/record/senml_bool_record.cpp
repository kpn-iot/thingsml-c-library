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
 * support for bool sensor
 */

#include "senml_bool_record.h"
#include "../../cbor/cbor.h"
#include "../senml_helpers.h"

int SenMLBoolRecord::fieldsToJson() {
    int res = SenMLRecord::fieldsToJson();
    res += printText(F(",\"vb\":"), 6);
    if (this->get()) {
        res += printText(F("true"), 4);
    } else {
        res += printText(F("false"), 5);
    }
    return res;
}

int SenMLBoolRecord::fieldsToCbor() {
    int res = SenMLRecord::fieldsToCbor();
    res += cbor_serialize_int(SENML_CBOR_VB_LABEL);
    res += cbor_serialize_bool(this->get());
    return res;
}
