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
 * support for double actuators headers
 */

#ifndef SENMLDOUBLEACTUATOR
#define SENMLDOUBLEACTUATOR

#include "../record/senml_double_record.h"

#define DOUBLE_ACTUATOR_SIGNATURE void (*callback)(double)

/**
 * A SenMLRecord that stores double data and supports actuation.
 */
class SenMLDoubleActuator : public SenMLDoubleRecord {
  public:
    SenMLDoubleActuator(const char *name, DOUBLE_ACTUATOR_SIGNATURE)
        : SenMLDoubleRecord(name, SENML_UNIT_NONE, 0.0), callback(callback){};
    SenMLDoubleActuator(const char *name, SenMLUnit unit, DOUBLE_ACTUATOR_SIGNATURE)
        : SenMLDoubleRecord(name, unit, 0.0), callback(callback){};
    SenMLDoubleActuator(const char *name, SenMLUnit unit, double value, DOUBLE_ACTUATOR_SIGNATURE)
        : SenMLDoubleRecord(name, unit, value), callback(callback){};
    SenMLDoubleActuator(ThingsMLMeasurementIndex index, DOUBLE_ACTUATOR_SIGNATURE)
        : SenMLDoubleRecord(index), callback(callback){};
    SenMLDoubleActuator(ThingsMLMeasurementIndex index, double value, DOUBLE_ACTUATOR_SIGNATURE)
        : SenMLDoubleRecord(index, value), callback(callback){};

    SenMLDoubleActuator(const char *name) : SenMLDoubleRecord(name){};
    SenMLDoubleActuator(const char *name, SenMLUnit unit) : SenMLDoubleRecord(name, unit){};
    SenMLDoubleActuator(const char *name, SenMLUnit unit, double value) : SenMLDoubleRecord(name, unit, value){};
    SenMLDoubleActuator(ThingsMLMeasurementIndex index) : SenMLDoubleRecord(index){};
    SenMLDoubleActuator(ThingsMLMeasurementIndex index, double value) : SenMLDoubleRecord(index, value){};

    ~SenMLDoubleActuator(){};

  protected:
    // called while parsing a senml message, when the parser found the value for an SenMLJsonListener
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

  private:
    DOUBLE_ACTUATOR_SIGNATURE = NULL;
};

#endif // SENMLDOUBLEACTUATOR
