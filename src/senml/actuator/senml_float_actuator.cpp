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
 * support for float actuators
 */

#include "senml_float_actuator.h"
#include "../senml_helpers.h"
#include "../senml_logging.h"

void SenMLFloatActuator::actuate(const void *value, int dataLength, SenMLDataType dataType) {
    if (dataType == SENML_TYPE_NR || dataType == CBOR_TYPE_DOUBLE) {
        this->set((float)*((double *)value));
        if (this->callback) {
            this->callback((float)*((double *)value));
        }
    } else if (dataType == CBOR_TYPE_FLOAT) {
        this->set(*((float *)value));
        if (this->callback) {
            this->callback(*((float *)value));
        }
    } else {
        log_debug(F("invalid type"));
    }
}
