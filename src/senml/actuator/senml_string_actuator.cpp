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
 * support for string actuators
 */

#include "senml_string_actuator.h"
#include "../senml_helpers.h"
#include "../senml_logging.h"

void SenMLStringActuator::actuate(const void *value, int dataLength, SenMLDataType dataType) {
    if (dataType == SENML_TYPE_STRING || dataType == CBOR_TYPE_STRING) {
        
        char * copy = (char *) malloc(dataLength + 1);
        copy[dataLength] = 0;
        memcpy(copy, value, dataLength);
        set((char *)copy);
        lastAllocated = copy;

        if (callback) {
            callback((char *)copy);
        }
    } else {
        log_debug(F("invalid type"));
    }
}

bool SenMLStringActuator::set(const char * value, double time, bool asSum) {
    if (lastAllocated != nullptr) {
        free(lastAllocated);
        lastAllocated = nullptr;
    }
    return SenMLStringRecord::set(value, time, asSum);
}