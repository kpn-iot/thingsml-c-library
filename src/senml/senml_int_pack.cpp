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
 * pack (document) with int base values
 */

#include "senml_int_pack.h"
#include "../cbor/cbor.h"
#include "senml_helpers.h"

void SenMLIntPack::setupStreamCtx(Stream *dest, SenMLStreamMethod format) {
    SenMLPack::setupStreamCtx(dest, format);
    _streamCtx->baseValue.baseInt = this->getBaseValue();
    _streamCtx->baseSum.baseInt = this->getBaseValue();
    _streamCtx->baseDataType = CBOR_TYPE_INT;
}

void SenMLIntPack::setupStreamCtx(char *dest, int length, SenMLStreamMethod format) {
    SenMLPack::setupStreamCtx(dest, length, format);
    _streamCtx->baseValue.baseInt = this->getBaseValue();
    _streamCtx->baseSum.baseInt = this->getBaseValue();
    _streamCtx->baseDataType = CBOR_TYPE_INT;
}

int SenMLIntPack::fieldsToJson() {
    int val;
    String strVal;
    int res = SenMLPack::fieldsToJson();
    val = this->getBaseValue();
    if (val != 0) {
        res += printText(F(",\"bv\":"), 6);
        strVal = String(val);
        res += printText(strVal.c_str(), strVal.length());
    }

    val = this->getBaseSum();
    if (val != 0) {
        res += printText(F(",\"bs\":"), 6);
        strVal = String(val);
        res += printText(strVal.c_str(), strVal.length());
    }
    return res;
}

int SenMLIntPack::fieldsToCbor() {
    int val;
    int res = SenMLPack::fieldsToCbor();

    val = this->getBaseValue();
    if (val) {
        res += cbor_serialize_int(SENML_CBOR_VB_LABEL);
        res += cbor_serialize_int(val);
    }

    val = this->getBaseSum();
    if (val) {
        res += cbor_serialize_int(SENML_CBOR_BS_LABEL);
        res += cbor_serialize_int(val);
    }
    return res;
}
