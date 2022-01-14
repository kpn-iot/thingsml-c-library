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
 * pack (document) without base values
 */
#include <math.h>
#include "defaults.h"
#include "../cbor/cbor.h"
#include "senml_jsonstreamingparser.h"
#include "senml_base.h"
#include "senml_cbor_parser.h"
#include "senml_helpers.h"
#include "senml_json_parser.h"
#include "senml_logging.h"
#include "senml_pack.h"
#include <string.h>
#ifdef THINGSML_LOW_RAM_DEVICE
#include <avr/pgmspace.h>
#endif

void SenMLPack::setBaseName(const char *name) {
    this->_bn = name;
}

const char *SenMLPack::getBaseName() {
    return this->_bn;
}

void SenMLPack::setBaseUnit(SenMLUnit unit) {
    this->_bu = unit;
}

void SenMLPack::setBaseTime(double time) {
    double prev = this->_bt;
    this->setBaseTimeSilently(time);
    SenMLBase *item = this->getFirst();
    while (item) {
        item->adjustToBaseTime(prev, time);
        item = item->getNext();
    }
}

void SenMLPack::setBaseTimeSilently(double time) {
    this->_bt = time;
}

int SenMLPack::getFieldLength() {
    int result = 0;
    if (this->_bn != NULL) {
        result++;
    }
    if (this->_bu) {
        result++;
    }
    if (!isnan(this->_bt)) {
        result++;
    }

    SenMLBase *first = this->getFirst();
    // we can only inline the first record. If the first item is a Pack (child device), then don't inline it.
    if (first && first->isPack() == false) {
        result += first->getFieldLength();
    }
    return result;
}

int SenMLPack::fieldsToJson() {
    bool doComma = false;
    int res = 0;
    if (this->_bn != NULL) {
        res += printText(F("\"bn\":\""), 6);
        res += printText(this->_bn, strlen(this->_bn));
        res += printText(F("\""), 1);
        doComma = true;
    }
    if (this->_bu) {
        if (doComma) {
            printText(F(","), 1);
        }
        res += printText(F("\"bu\":\""), 6);
        res += printUnit(this->_bu);
        res += printText(F("\""), 1);
        doComma = true;
    }
    if (!isnan(this->_bt)) {
        if (doComma) {
            res += printText(F(","), 1);
        }
        res += printText(F("\"bt\":"), 5);
        res += printDouble(this->_bt, SENML_MAX_DOUBLE_PRECISION);
    }
    return res;
}

int SenMLPack::fieldsToCbor() {
    int res = 0;
    if (this->_bn != NULL) {
        res += cbor_serialize_int(SENML_CBOR_BN_LABEL);
        res += cbor_serialize_unicode_string(this->_bn);
    }
    if (this->_bu) {
        res += cbor_serialize_int(SENML_CBOR_BU_LABEL);
#ifdef THINGSML_LOW_RAM_DEVICE
        strcpy_P(pgmBuff, (char *)pgm_read_word(&(senml_units_names[this->_bu])));
        res += cbor_serialize_unicode_string(pgmBuff);
#else
        res += cbor_serialize_unicode_string(senml_units_names[this->_bu]);
#endif
    }
    if (!isnan(this->_bt)) {
        res += cbor_serialize_int(SENML_CBOR_BT_LABEL);
        if (is_lossless_to_half_float(this->_bt)) {
            res += cbor_serialize_half_float(this->_bt);
        } else if (is_lossless_to_float(this->_bt)) {
            res += cbor_serialize_float(this->_bt);
        } else {
            res += cbor_serialize_double(this->_bt);
        }
    }
    return res;
}
