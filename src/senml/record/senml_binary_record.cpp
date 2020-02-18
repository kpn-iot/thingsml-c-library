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
 * support for binary sensors
 */

#include "senml_binary_record.h"
#include "../senml_helpers.h"
#include "../../cbor/cbor.h"

bool SenMLBinaryRecord::set(unsigned char * value, unsigned int length, double time) 
{
    this->_value = value;
    this->_length = length;
    return this->setTime(time);
}


int SenMLBinaryRecord::fieldsToJson()
{
    int res = SenMLRecord::fieldsToJson();
    res += printText(F(",\"vd\":\""), 7);
    res += printBinaryAsBase64(this->_value, this->_length);
    res += printText(F("\""), 1);
    return res;
}

int SenMLBinaryRecord::fieldsToCbor()
{
    int res = SenMLRecord::fieldsToCbor();
    res += cbor_serialize_int(SENML_CBOR_VD_LABEL);
    res += cbor_serialize_byte_string((const char*)this->_value, this->_length);
    return res;
}






