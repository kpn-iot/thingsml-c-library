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
 * support for int actuators
 */

#include "senml_int_actuator.h"
#include "../senml_helpers.h"
#include "../senml_logging.h"

void SenMLIntActuator::actuate(const void *value, int dataLength, SenMLDataType dataType) {
    if (dataType == SENML_TYPE_NR) {
        this->set((int)*((double *)value));
        if (this->callback) {
            this->callback((int)*((double *)value));
        }
    } else if (dataType == CBOR_TYPE_INT) {
        int64_t val = *((int64_t *)value);
        this->set((int)val);
        if (this->callback) {
            this->callback((int)val);
        }
    } else if (dataType == CBOR_TYPE_UINT) {
        uint64_t val = *((uint64_t *)value);
        this->set((int)val);
        if (this->callback) {
            this->callback((int)val);
        }
    } else {
        log_debug(F("invalid type"));
    }
}
