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
 * support for string sensors
 */

#ifndef SENMLSTRINGRECORD
#define SENMLSTRINGRECORD

#include "../defaults.h"
#include "senml_record_t.h"

/**
 * A SenMLRecord that stores text data.
 * This type of object can only be used for sensor data. If actuation is needed, use SenMLStringActuator
 * instead.
 */
class SenMLStringRecord : public SenMLRecordTemplate<const char *> {
  public:
    SenMLStringRecord(const char *name) : SenMLRecordTemplate(name){};
    SenMLStringRecord(const char *name, SenMLUnit unit) : SenMLRecordTemplate(name, unit){};
    SenMLStringRecord(const char *name, SenMLUnit unit, const char *value) : SenMLRecordTemplate(name, unit, value){};
    SenMLStringRecord(ThingsMLMeasurementIndex index) : SenMLRecordTemplate(index){};
    SenMLStringRecord(ThingsMLMeasurementIndex index, const char *value) : SenMLRecordTemplate(index, value){};

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
};

#endif // SENMLSTRINGRECORD
