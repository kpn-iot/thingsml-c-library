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
 * support for binary actuators header
 */

#ifndef SENMLBINARYACTUATOR
#define SENMLBINARYACTUATOR

#include "../record/senml_binary_record.h"

#define BINARY_ACTUATOR_SIGNATURE void (*callback)(const unsigned char *, int)

/**
 * A SenMLRecord that stores binary data and supports actuation.
 */
class SenMLBinaryActuator : public SenMLBinaryRecord {
    friend class SenMLCborParser;

  public:
    SenMLBinaryActuator(const char *name, BINARY_ACTUATOR_SIGNATURE)
        : SenMLBinaryRecord(name, SENML_UNIT_NONE), callback(callback){};
    SenMLBinaryActuator(const char *name, SenMLUnit unit, BINARY_ACTUATOR_SIGNATURE)
        : SenMLBinaryRecord(name, unit), callback(callback){};
    SenMLBinaryActuator(ThingsMLMeasurementIndex index, BINARY_ACTUATOR_SIGNATURE)
        : SenMLBinaryRecord(index), callback(callback){};

    SenMLBinaryActuator(const char *name) : SenMLBinaryRecord(name){};
    SenMLBinaryActuator(const char *name, SenMLUnit unit) : SenMLBinaryRecord(name, unit){};
    SenMLBinaryActuator(ThingsMLMeasurementIndex index) : SenMLBinaryRecord(index){};

    ~SenMLBinaryActuator(){};

    virtual bool set(unsigned char *value, unsigned int length, double time = NAN);

  protected:
    // called while parsing a senml message, when the parser found the value for an SenMLJsonListener
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

  private:
    BINARY_ACTUATOR_SIGNATURE = NULL;
    unsigned char * lastAllocated = nullptr;
};

#endif // SENMLBINARYACTUATOR
