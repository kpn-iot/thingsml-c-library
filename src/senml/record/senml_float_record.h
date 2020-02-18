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
 * support for float sensors header
 */

#ifndef SENMLFLOATRECORD
#define SENMLFLOATRECORD

#include "senml_record_t.h"

/**
 * A SenMLRecord that stores float data.
 * This type of object can only be used for sensor data. If actuation is needed, use SenMLFloatActuator
 * instead.
 */
class SenMLFloatRecord : public SenMLRecordTemplate<float> {
  public:
    SenMLFloatRecord(const char *name) : SenMLRecordTemplate(name){};
    SenMLFloatRecord(const char *name, SenMLUnit unit) : SenMLRecordTemplate(name, unit){};
    SenMLFloatRecord(const char *name, SenMLUnit unit, float value) : SenMLRecordTemplate(name, unit, value){};
    SenMLFloatRecord(ThingsMLMeasurementIndex index) : SenMLRecordTemplate(index){};
    SenMLFloatRecord(ThingsMLMeasurementIndex index, float value) : SenMLRecordTemplate(index, value){};
    ~SenMLFloatRecord(){};

    /**
     * renders all the fields to json, without the starting and ending brackets.
     * Inheriters can extend this function if they want to add extra fields to the json output
     * note: this is public so that custom implementations for the record object can use other objects
     * internally and render to json using this function (ex: coordinatesRecord using 3 floatRecrods for lat, lon & alt.
     * @returns: The number of bytes that were written.
     */
    virtual int fieldsToJson();

    /**
     * renders all the fields to cbor format. renders all the fields of the object without the length info
     * at the beginning
     * note: this is public so that custom implementations for the record object can use other objects
     * internally and render to json using this function (ex: coordinatesRecord using 3 floatRecrods for
     * lat, lon & alt.
     * @returns: The number of bytes that were written.
     */
    virtual int fieldsToCbor();

  protected:
  private:
    float getAdjustedValue();
};

#endif // SENMLFLOATRECORD
