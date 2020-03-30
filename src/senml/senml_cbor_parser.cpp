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
 * parse cbor
 */

#include "senml_cbor_parser.h"
#include "actuator/senml_binary_actuator.h"
#include "senml_helpers.h"

void SenMLCborParser::parse(Stream *source) {
    this->ctx.data.stream = source;
    this->ctx.dataAsBlob = false;
    this->internalParse();
}

void SenMLCborParser::parse(char *source, int length) {
    this->ctx.data.blob.data = source;
    this->ctx.data.blob.curPos = 0;
    this->ctx.data.blob.length = length;
    this->ctx.dataAsBlob = true;
    this->internalParse();
}

void SenMLCborParser::internalParse() {
    unsigned int read_bytes =
        this->processArray(); // the root element of senml input is always an array. We do this cause the readable()
                              // function on mbed doesn't work as expected, so we can't read until there is no more
                              // data, we need to read until we have valid input.

    if (read_bytes == 0) {
        log_debug(F("invalid input"));
    }
    flush(); // make certain that the input streams are empty when done. This also resets any internally stored data. If
             // we don't do this, we can only handle 1 bad input stream, ten it breaks.
}

size_t SenMLCborParser::processArray() {
    const bool is_indefinite = (peekChar() == (CBOR_ARRAY | CBOR_VAR_FOLLOWS));
    uint64_t array_length = 0;
    size_t read_bytes;

    if (is_indefinite) {
        log_debug(F("not supported"));
    } else {
        read_bytes = decode_int(&array_length);
    }

    size_t i = 0;

    while (i < array_length) {
        size_t inner_read_bytes = this->parseNext();
        if (inner_read_bytes == 0) {
            log_debug(F("invalid input"));
            break;
        }
        read_bytes += inner_read_bytes;
        ++i;
    }

    return read_bytes;
}

size_t SenMLCborParser::parseNext() {
    switch (CBOR_TYPE) {
        case CBOR_UINT:
            return this->processUnsignedInt();
        case CBOR_NEGINT:
            return this->processInt();
        case CBOR_BYTES:
            return this->processBytes(CBOR_TYPE_DATA);
        case CBOR_TEXT:
            return this->processBytes(CBOR_TYPE_STRING);
        case CBOR_ARRAY:
            return this->processArray();
        case CBOR_MAP:
            return this->processMap();
        case CBOR_7: {
            bool boolRes;
            float floatVal;
            double doubleVal;
            size_t read_bytes;
            switch ((int)peekChar()) {
                case CBOR_FALSE:
                    readChar(); // need to remove the char from the stream.
                    boolRes = false;
                    this->setValue((void *)&boolRes, sizeof(boolRes), CBOR_TYPE_BOOL);
                    return 1;
                case CBOR_TRUE:
                    readChar(); // need to remove the char from the stream.
                    boolRes = true;
                    this->setValue((void *)&boolRes, sizeof(boolRes), CBOR_TYPE_BOOL);
                    return 1;
                case CBOR_FLOAT16:
                    read_bytes = cbor_deserialize_float_half(&floatVal);
                    this->processDouble(floatVal);
                    return read_bytes;
                case CBOR_FLOAT32:
                    read_bytes = cbor_deserialize_float(&floatVal);
                    this->processDouble(floatVal);
                    return read_bytes;
                case CBOR_FLOAT64:
                    read_bytes = cbor_deserialize_double(&doubleVal);
                    this->processDouble(doubleVal);
                    return read_bytes;
            }
        }
    }
    return 0; // if we get here, something went wrong
}

size_t SenMLCborParser::processMap() {
    const bool is_indefinite = (peekChar() == (CBOR_MAP | CBOR_VAR_FOLLOWS));
    uint64_t map_length = 0;
    size_t read_bytes;

    if (is_indefinite) {
        log_debug(F("not supported"));
    } else {
        read_bytes = decode_int(&map_length);
    }

    unsigned char curState = this->state;
    size_t i = 0;
    while (i < map_length) {
        size_t key_read_bytes, value_read_bytes;
        this->state = SENML_CBOR_KEY;
        key_read_bytes = this->parseNext(); /* key */
        this->state = SENML_CBOR_VALUE;
        value_read_bytes = this->parseNext(); /* value */
        if (key_read_bytes == 0 || value_read_bytes == 0) {
            log_debug(F("invalid input"));
            break;
        }
        read_bytes += key_read_bytes + value_read_bytes;
        ++i;
    }
    this->state = curState;           // reset to the original state. was changed inside loop
    this->ctx.baseValue.baseUint = 0; // if there was a base value, reset it for the next run.
    return read_bytes;
};

void SenMLCborParser::processDouble(double value) {
    switch (this->curLabel) {
        case SENML_CBOR_BV_LABEL:
            this->ctx.baseValue.baseDouble = value;
            break;
        case SENML_CBOR_V_LABEL:
            double calculated = this->ctx.baseValue.baseDouble + value;
            this->setValue(&calculated, sizeof(double), CBOR_TYPE_DOUBLE);
            break;
    }
}

size_t SenMLCborParser::processBytes(SenMLDataType type) {
    uint64_t bytes_length; // needs to be this big for decode_int
    size_t bytes_read = decode_int(&bytes_length);

    if (bytes_read == 0) {
        return 0;
    }
    char *buffer = (char *)malloc(bytes_length + 1);
    for (int i = 0; i < (int)bytes_length; i++) {
        buffer[i] = readChar();
    }
    buffer[bytes_length] = 0; // close it, just to be save. not always needed, but it is for strings
    String value(buffer);
    free(buffer);

    if (type == CBOR_TYPE_DATA) { // we are expecting binary data, so it has to be for a binary data value.
        if (this->curLabel == SENML_CBOR_VD_LABEL) {
            this->setBinaryValue(value.c_str(), bytes_length);
        } else {
            log_debug(F("invalid input"));
        }
    } else { // its text
        switch (this->curLabel) {
            case SENML_CBOR_BN_LABEL:
                this->setCurrentPack(value);
                break;
            case SENML_CBOR_BU_LABEL:
                break;
            case SENML_CBOR_N_LABEL:
                this->setCurrentRecord(value);
                break;
            case SENML_CBOR_VS_LABEL:
                this->setValue((void *)value.c_str(), value.length(), CBOR_TYPE_STRING);
                break;
        }
    }

    return bytes_read + bytes_length;
}

size_t SenMLCborParser::processUnsignedInt() {
    uint64_t val;
    uint64_t calculated;
    size_t read_bytes = cbor_deserialize_uint64_t(&val);
    if (this->state == SENML_CBOR_VALUE) {
        switch (this->curLabel) {
            case SENML_CBOR_BV_LABEL:
                this->ctx.baseValue.baseUint = val;
                break;
            case SENML_CBOR_V_LABEL:
                calculated = this->ctx.baseValue.baseUint + val;
                this->setValue((void *)&calculated, sizeof(uint64_t), CBOR_TYPE_UINT);
                break;
            case THINGSML_CBOR_I_LABEL:
                this->setCurrentRecord(val);
                break;
        }
    } else if (this->state == SENML_CBOR_KEY) {
        // store the value type (basename, baseunit, value, stringvalue,...)
        this->curLabel = (int)val;
    }
    return read_bytes;
}

size_t SenMLCborParser::processInt() {
    int64_t val;
    int64_t calculated;
    size_t read_bytes = cbor_deserialize_int64_t(&val);
    if (this->state == SENML_CBOR_VALUE) {
        switch (this->curLabel) {
            case SENML_CBOR_BV_LABEL:
                this->ctx.baseValue.baseInt = val;
                break;
            case SENML_CBOR_V_LABEL:
                calculated = this->ctx.baseValue.baseInt + val;
                this->setValue((void *)&calculated, sizeof(int64_t), CBOR_TYPE_INT);
                break;
            case THINGSML_CBOR_I_LABEL:
                this->setCurrentRecord(val);
                break;
        }
    } else if (this->state == SENML_CBOR_KEY) {
        // store the value type (basename, baseunit, value, stringvalue,...)
        this->curLabel = val;
    }
    return read_bytes;
}

void SenMLCborParser::setValue(void *value, int length, SenMLDataType type) {
    if (this->curRec) {
        this->curRec->actuate(value, length, type);
    } else {
        SenMLBasePack *pack = this->curPack;
        if (!pack) {
            pack = this->root;
        }
        if (pack && pack->isActuator()) {
            ((SenMLPack *)pack)->actuate(this->curPackName.c_str(), this->curRecName.c_str(), value, length, type);
        }
    }
}

void SenMLCborParser::setBinaryValue(const char *value, int length) {
    if (this->curRec) {
        ((SenMLBinaryActuator *)this->curRec)->actuate(value, length, CBOR_TYPE_DATA);
    } else {
        SenMLBasePack *pack = this->curPack;
        if (!pack) {
            pack = this->root;
        }
        if (pack && pack->isActuator()) {
            ((SenMLPack *)pack)
                ->actuate(this->curPackName.c_str(), this->curRecName.c_str(), value, length, CBOR_TYPE_DATA);
        }
    }
}
