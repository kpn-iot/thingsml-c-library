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
 * parse cbor header
 */

#ifndef THINGSML_SENML_CBOR_PARSER
#define THINGSML_SENML_CBOR_PARSER

#include "../cbor/cbor.h"
#include "senml_base_parser.h"
#include "senml_enums.h"
#include "senml_helpers.h"
#include "senml_logging.h"

enum SenMLCborDataType {};

#define SENML_CBOR_KEY 1
#define SENML_CBOR_VALUE 2

/**
 * Inernal helper class for parsing cbor data.
 */
class SenMLCborParser : public SenMLBaseParser {

  public:
    SenMLCborParser(SenMLBasePack *root, SenMLStreamMethod format) : SenMLBaseParser(root), state(0) {
        this->ctx.format = format;
        this->ctx.baseValue.baseUint = 0; // init to 0, so we get correct results for first element as well.
        this->ctx.baseSum.baseUint = 0;
        _streamCtx = &this->ctx; // set the global variable so that we don't have to pass it along on the stack all the
                                 // time (saves mem & codesize)
    };

    // convert the cbor raw data into senml and actuate the records in the root pack.
    void parse(Stream *source);

    void parse(char *source, int length);

  private:
    unsigned char state; // keeps track of the current parse state
    int curLabel; // the cbor number that represents the current senml label (unit, value, boolvalue, basename,..). The
                  // next item to read has to be the value for the label
    StreamContext ctx;

    unsigned int parseNext();

    void setValue(void *value, int length, SenMLDataType type);
    void setBinaryValue(const char *value, int length);
    void processDouble(double value);

    void internalParse();

    unsigned int processBytes(SenMLDataType type);

    unsigned int processArray();

    inline unsigned int processMap() {
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

    inline unsigned int processUnsignedInt() {
        uint64_t val;
        size_t read_bytes = cbor_deserialize_uint64_t(&val);
        if (this->state == SENML_CBOR_VALUE) {
            switch (this->curLabel) {
            case SENML_CBOR_BV_LABEL:
                this->ctx.baseValue.baseUint = val;
                break;
            case SENML_CBOR_V_LABEL:
                uint64_t calculated = this->ctx.baseValue.baseUint + val;
                this->setValue((void *)&calculated, sizeof(uint64_t), CBOR_TYPE_UINT);
                break;
            }
        }
        // store the value type (basename, baseunit, value, stringvalue,...)
        else if (this->state == SENML_CBOR_KEY) {
            this->curLabel = (int)val;
        }
        return read_bytes;
    };

    inline unsigned int processInt() {
        int64_t val;
        size_t read_bytes = cbor_deserialize_int64_t(&val);
        if (this->state == SENML_CBOR_VALUE) {
            switch (this->curLabel) {
            case SENML_CBOR_BV_LABEL:
                this->ctx.baseValue.baseInt = val;
                break;
            case SENML_CBOR_V_LABEL:
                uint64_t calculated = this->ctx.baseValue.baseInt + val;
                this->setValue((void *)&calculated, sizeof(int64_t), CBOR_TYPE_INT);
                break;
            }
        }
        // store the value type (basename, baseunit, value, stringvalue,...)
        else if (this->state == SENML_CBOR_KEY) {
            this->curLabel = val;
        }
        return read_bytes;
    };
};

#endif // THINGSML_SENML_CBOR_PARSER
