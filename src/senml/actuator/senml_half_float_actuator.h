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

#ifndef SENMLHALFFLOATACTUATOR
#define SENMLHALFFLOATACTUATOR

#include "../record/senml_half_float_record.h"

#define FLOAT_ACTUATOR_SIGNATURE void (*callback)(float)

/**
 * A SenMLRecord that stores float data and supports actuation.
 */
class SenMLHalfFloatActuator : public SenMLHalfFloatRecord {
  public:
    SenMLHalfFloatActuator(const char *name, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLHalfFloatRecord(name, SENML_UNIT_NONE, 0.0), callback(callback){};
    SenMLHalfFloatActuator(const char *name, SenMLUnit unit, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLHalfFloatRecord(name, unit, 0.0), callback(callback){};
    SenMLHalfFloatActuator(const char *name, SenMLUnit unit, float value, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLHalfFloatRecord(name, unit, value), callback(callback){};
    SenMLHalfFloatActuator(ThingsMLMeasurementIndex index, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLHalfFloatRecord(index), callback(callback){};
    SenMLHalfFloatActuator(ThingsMLMeasurementIndex index, float value, FLOAT_ACTUATOR_SIGNATURE)
        : SenMLHalfFloatRecord(index, value), callback(callback){};

    SenMLHalfFloatActuator(const char *name) : SenMLHalfFloatRecord(name){};
    SenMLHalfFloatActuator(const char *name, SenMLUnit unit) : SenMLHalfFloatRecord(name, unit){};
    SenMLHalfFloatActuator(const char *name, SenMLUnit unit, float value) : SenMLHalfFloatRecord(name, unit, value){};
    SenMLHalfFloatActuator(ThingsMLMeasurementIndex index) : SenMLHalfFloatRecord(index){};
    SenMLHalfFloatActuator(ThingsMLMeasurementIndex index, float value) : SenMLHalfFloatRecord(index, value){};
    ~SenMLHalfFloatActuator(){};

  protected:
    // called while parsing a senml message, when the parser found the value for an SenMLJsonListener
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

  private:
    FLOAT_ACTUATOR_SIGNATURE = NULL;
};

#endif // SENMLHALFFLOATACTUATOR
