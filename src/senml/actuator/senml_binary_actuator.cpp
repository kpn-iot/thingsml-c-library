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
 * support for binary actuators
 */

#include "senml_binary_actuator.h"
#include "../senml_helpers.h"
#include "../senml_logging.h"
#include <string.h>

void SenMLBinaryActuator::actuate(const void *value, int dataLength, SenMLDataType dataType) {
    if (dataType == SENML_TYPE_DATA) {
        int decodedLen = base64_dec_len((char *)value, dataLength);
        char *decoded = (char *)malloc(decodedLen);
#ifdef ESP32
        base64_decode_chars((const char *)value, dataLength, decoded);
#else
        base64_decode(decoded, (char *)value, dataLength);
#endif
        set((unsigned char *)decoded, decodedLen);
        lastAllocated = (unsigned char *) decoded;
        if (callback) {
            callback((unsigned char *)decoded, decodedLen);
        }
    } else if (dataType == CBOR_TYPE_DATA) {
        unsigned char * copy = (unsigned char *) malloc(dataLength);
        memcpy(copy, value, dataLength);
        set(copy, dataLength);
        lastAllocated = copy;
        if (callback) {
            callback((unsigned char *)copy, dataLength);
        }
    } else {
        log_debug(F("invalid type"));
    }
}

bool SenMLBinaryActuator::set(unsigned char * value, unsigned int length, double time) {
    if (lastAllocated != nullptr) {
        free(lastAllocated);
        lastAllocated = nullptr;
    }
    return SenMLBinaryRecord::set(value, length, time);
}
