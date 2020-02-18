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
 * support for bool actuators
 */

#include "senml_bool_actuator.h"
#include "../senml_helpers.h"
#include "../senml_logging.h"

void SenMLBoolActuator::actuate(const void *value, int dataLength, SenMLDataType dataType) {
    if (dataType == SENML_TYPE_BOOL || dataType == CBOR_TYPE_BOOL) {
        this->set(*((bool *)value));
        if (this->callback) {
            this->callback(*((bool *)value));
        }
    } else {
        log_debug(F("invalid type"));
    }
}
