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
 * support for int actuators headers
 */

#ifndef SENMLINTACTUATOR
#define SENMLINTACTUATOR

#include "../record/senml_int_record.h"

#define INT_ACTUATOR_SIGNATURE void (*callback)(int)

/**
 * A SenMLRecord that stores integer data and supports actuation.
 */
class SenMLIntActuator : public SenMLIntRecord {
  public:
    SenMLIntActuator(const char *name, INT_ACTUATOR_SIGNATURE)
        : SenMLIntRecord(name, SENML_UNIT_NONE, 0), callback(callback){};
    SenMLIntActuator(const char *name, SenMLUnit unit, INT_ACTUATOR_SIGNATURE)
        : SenMLIntRecord(name, unit, 0), callback(callback){};
    SenMLIntActuator(const char *name, SenMLUnit unit, int value, INT_ACTUATOR_SIGNATURE)
        : SenMLIntRecord(name, unit, value), callback(callback){};
    ~SenMLIntActuator(){};

  protected:
    /**
     * called while parsing a senml message, when the parser found the value for an SenMLJsonListener
     */
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

  private:
    INT_ACTUATOR_SIGNATURE;
};

#endif // SENMLINTACTUATOR
