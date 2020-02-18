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

#ifndef SENMLSTRINGACTUATOR
#define SENMLSTRINGACTUATOR

#include "../record/senml_string_record.h"

#define STRING_ACTUATOR_SIGNATURE void (*callback)(const char *)

/**
 * A SenMLRecord that stores text data and supports actuation.
 */
class SenMLStringActuator : public SenMLStringRecord {
  public:
    SenMLStringActuator(const char *name, STRING_ACTUATOR_SIGNATURE)
        : SenMLStringRecord(name, SENML_UNIT_NONE, NULL), callback(callback){};
    SenMLStringActuator(const char *name, SenMLUnit unit, STRING_ACTUATOR_SIGNATURE)
        : SenMLStringRecord(name, unit, NULL), callback(callback){};
    SenMLStringActuator(const char *name, SenMLUnit unit, const char *value, STRING_ACTUATOR_SIGNATURE)
        : SenMLStringRecord(name, unit, value), callback(callback){};
    ~SenMLStringActuator(){};

  protected:
    /**
     * called while parsing a senml message, when the parser found the value for an SenMLJsonListener
     */
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

  private:
    STRING_ACTUATOR_SIGNATURE;
};

#endif // SENMLSTRINGACTUATOR
