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
 * record base class
 */

#include "senml_record.h"
#include "../../cbor/cbor.h"
#include "../senml_helpers.h"
#include "../senml_logging.h"
#include "../senml_pack.h"
#include <math.h>
#include <string.h>
#ifdef THINGSML_LOW_RAM_DEVICE
#include <avr/pgmspace.h>
#endif

bool SenMLRecord::setTime(double value, bool absolute) {
    SenMLBase *root = this->getRoot();
    if (absolute) {
        if (root) {
            if (root->isPack()) {
                double baseTime = ((SenMLPack *)root)->getBaseTime();
                if (!isnan(baseTime))
                    value -= baseTime;
            } else {
                return false;
            }
        }
    } else if (root == NULL) {
        return false;
    }
    this->_time = value;
    return true;
}

bool SenMLRecord::setUpdateTime(double value, bool absolute) {
    this->_updateTime = value;
    return true;
}

int SenMLRecord::contentToJson() {
    int res = printText(F("{"), 1);
    res += this->fieldsToJson();
    res += printText(F("}"), 1);
    return res;
}

void SenMLRecord::adjustToBaseTime(double prev, double time) {
    if (!isnan(this->_time)) {
        if (!isnan(prev)) {
            this->_time += prev;
        }
        if (!isnan(time)) {
            this->_time -= time;
        }
    }
}

int SenMLRecord::fieldsToJson() {
    int res = 0;
    if (this->_name != NULL) {
        res += printText(F("\"n\":\""), 5);
        res += printText(this->_name, strlen(this->_name));
        res += printText(F("\""), 1);
    } else if (this->_index != THINGSML_NO_INDEX) {
        res += printText(F("\"i_\":"), 5);
        res += printInt(this->_index);
    }
    if (!isnan(this->_time)) {
        res += printText(F(",\"t\":"), 5);
        res += printDouble(this->_time, SENML_MAX_DOUBLE_PRECISION);
    }
    if (this->_unit != SENML_UNIT_NONE) {
        res += printText(F(",\"u\":\""), 6);
        res += printUnit(this->_unit);
        res += printText(F("\""), 1);
    }
    if (this->_updateTime != 0) {
        res += printText(F(",\"ut\":"), 6);
        res += printDouble(this->_updateTime, SENML_MAX_DOUBLE_PRECISION);
    }
    return res;
}

void SenMLRecord::actuate(const void *value, int dataLength, SenMLDataType dataType) {
    log_debug(this->getName());
    log_debug(F("no actuator"));
}

int SenMLRecord::contentToCbor() {
    int res = cbor_serialize_map(this->getFieldLength());
    res += this->fieldsToCbor();
    return res;
}

int SenMLRecord::getFieldLength() {
    int result = 1; // always have 1 item for the value
    if (this->_name != NULL) {
        result++;
    }
    if (this->_index != THINGSML_NO_INDEX) {
        result++;
    }
    if (!isnan(this->_time)) {
        result++;
    }
    if (this->_unit != SENML_UNIT_NONE) {
        result++;
    }
    if (this->_updateTime != 0) {
        result++;
    }
    return result;
}

int SenMLRecord::fieldsToCbor() {
    int res = 0;
    if (this->_name != NULL) {
        res += cbor_serialize_int(SENML_CBOR_N_LABEL);
        res += cbor_serialize_unicode_string(this->_name);
    } else if (this->_index != THINGSML_NO_INDEX) {
        res += cbor_serialize_int(THINGSML_CBOR_I_LABEL);
        res += cbor_serialize_int(this->_index);
    }
    if (!isnan(this->_time)) {
        res += cbor_serialize_int(SENML_CBOR_T_LABEL);
        if (is_lossless_to_half_float(this->_time)) {
            res += cbor_serialize_half_float(this->_time);
        } else if (is_lossless_to_float(this->_time)) {
            res += cbor_serialize_float(this->_time);
        } else {
            res += cbor_serialize_double(this->_time);
        }
    }
    if (this->_unit != SENML_UNIT_NONE) {
        res += cbor_serialize_int(SENML_CBOR_U_LABEL);
#ifdef THINGSML_LOW_RAM_DEVICE
        // char pgmBuff[9] = {0};
        strcpy_P(pgmBuff, (char *)pgm_read_word(&(senml_units_names[this->_unit])));
        res += cbor_serialize_unicode_string(pgmBuff);
#else
        res += cbor_serialize_unicode_string(senml_units_names[this->_unit]);
#endif
    }
    if (this->_updateTime != 0) {
        res += cbor_serialize_int(SENML_CBOR_UT_LABEL);
        res += cbor_serialize_int(this->_updateTime);
    }
    return res;
}
