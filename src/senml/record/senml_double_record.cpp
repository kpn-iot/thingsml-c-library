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
 * support for double sensors
 */

#include "senml_double_record.h"
#include "../../cbor/cbor.h"
#include "../senml_double_pack.h"
#include "../senml_helpers.h"
#include "../senml_int_pack.h"

double SenMLDoubleRecord::getAdjustedValue() {
    double adjustedValue = this->get();
    if (_streamCtx->baseDataType == CBOR_TYPE_DOUBLE) {
        if (this->asSum()) {
            adjustedValue -= ((SenMLDoublePack *)this->getRoot())->getBaseSum();
        } else {
            adjustedValue -= ((SenMLDoublePack *)this->getRoot())->getBaseValue();
        }
    } else if (_streamCtx->baseDataType == CBOR_TYPE_INT) {
        if (this->asSum()) {
            adjustedValue -= ((SenMLIntPack *)this->getRoot())->getBaseSum();
        } else {
            adjustedValue -= ((SenMLIntPack *)this->getRoot())->getBaseValue();
        }
    }
    return adjustedValue;
}

int SenMLDoubleRecord::fieldsToJson() {
    int res = SenMLRecord::fieldsToJson();
    if (this->asSum()) {
        res += printText(F(",\"s\":"), 5);
    } else {
        res += printText(F(",\"v\":"), 5);
    }
    res += printDouble(this->getAdjustedValue(), SENML_MAX_DOUBLE_PRECISION);
    return res;
}

int SenMLDoubleRecord::fieldsToCbor() {
    int res = SenMLRecord::fieldsToCbor();
    if (this->asSum()) {
        res += cbor_serialize_int(SENML_CBOR_S_LABEL);
    } else {
        res += cbor_serialize_int(SENML_CBOR_V_LABEL);
    }
    double value = this->getAdjustedValue();
    if (is_lossless_to_half_float(value)) {
        res += cbor_serialize_half_float(value);
    } else if (is_lossless_to_float(value)) {
        res += cbor_serialize_float(value);
    } else {
        res += cbor_serialize_double(value);
    }
    return res;
}
