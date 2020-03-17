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


    void setValue(void *value, int length, SenMLDataType type);
    void setBinaryValue(const char *value, int length);
    void processDouble(double value);

    void internalParse();

    size_t processBytes(SenMLDataType type);

    size_t parseNext();
    
    size_t processUnsignedInt();
    size_t processInt();

    size_t processArray();
    size_t processMap();

};

#endif // THINGSML_SENML_CBOR_PARSER