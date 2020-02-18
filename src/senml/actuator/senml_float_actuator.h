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
 * support for float actuators headers
 */

#ifndef SENMLFLOATACTUATOR
#define SENMLFLOATACTUATOR

#include "../record/senml_float_record.h"

#define FLOAT_ACTUATOR_SIGNATURE void (*callback)(float)

/**
 * A SenMLRecord that stores float data and supports actuation.
 */
class SenMLFloatActuator : public SenMLFloatRecord {
  public:
    SenMLFloatActuator(const char *name, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLFloatRecord(name, SENML_UNIT_NONE, 0.0), callback(callback){};
    SenMLFloatActuator(const char *name, SenMLUnit unit, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLFloatRecord(name, unit, 0.0), callback(callback){};
    SenMLFloatActuator(const char *name, SenMLUnit unit, float value, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLFloatRecord(name, unit, value), callback(callback){};
    ~SenMLFloatActuator(){};

  protected:
    // called while parsing a senml message, when the parser found the value for an SenMLJsonListener
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

  private:
    FLOAT_ACTUATOR_SIGNATURE;
};

#endif // SENMLFLOATACTUATOR
