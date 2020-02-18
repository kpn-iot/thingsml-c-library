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
 * support for bool actuator header
 */

#ifndef SENMLBOOLACTUATOR
#define SENMLBOOLACTUATOR

#include "../record/senml_bool_record.h"

#define BOOL_ACTUATOR_SIGNATURE void (*callback)(bool)

/**
 * A SenMLRecord that stores boolean data and supports actuation.
 */
class SenMLBoolActuator : public SenMLBoolRecord {
  public:
    SenMLBoolActuator(const char *name, BOOL_ACTUATOR_SIGNATURE)
        : SenMLBoolRecord(name, SENML_UNIT_NONE, false), callback(callback){};
    SenMLBoolActuator(const char *name, SenMLUnit unit, BOOL_ACTUATOR_SIGNATURE)
        : SenMLBoolRecord(name, unit, false), callback(callback){};
    SenMLBoolActuator(const char *name, SenMLUnit unit, bool value, BOOL_ACTUATOR_SIGNATURE)
        : SenMLBoolRecord(name, unit, value), callback(callback){};
    ~SenMLBoolActuator(){};

  protected:
    // called while parsing a senml message, when the parser found the value for an SenMLJsonListener
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

  private:
    BOOL_ACTUATOR_SIGNATURE;
};

#endif // SENMLBOOLACTUATOR
