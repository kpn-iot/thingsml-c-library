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
 * parse json header
 */

#ifndef THINGSML_SENML_JSON_PARSER
#define THINGSML_SENML_JSON_PARSER

#include "senml_JsonListener.h"
#include "senml_base_parser.h"

/**
 * Internal helper class for parsing json data.
 */
class SenMLJsonListener : public JsonListener, SenMLBaseParser {

  public:
    SenMLJsonListener(SenMLBasePack *root) : JsonListener(), SenMLBaseParser(root){};

    virtual void key(String key);
    virtual void value(String value);

  private:
    int expected;     // name of key who's value is expected next
    double baseValue; // if the current pack defined a base value, add it to all values that we find

    void setValue(const void *value, int length, SenMLDataType dataType);
};

#endif // THINGSML_SENML_JSON_PARSER
