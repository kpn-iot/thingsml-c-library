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
 * support for int sensors header
 */

#ifndef SENMLINTRECORD
#define SENMLINTRECORD

#include "senml_record_t.h"

/**
 * A SenMLRecord that stores integer data.
 * This type of object can only be used for sensor data. If actuation is needed, use SenMLIntActuator
 * instead.
 */
class SenMLIntRecord : public SenMLRecordTemplate<int> {
  public:
    SenMLIntRecord(const char *name) : SenMLRecordTemplate(name){};
    SenMLIntRecord(const char *name, SenMLUnit unit) : SenMLRecordTemplate(name, unit){};
    SenMLIntRecord(const char *name, SenMLUnit unit, int value) : SenMLRecordTemplate(name, unit, value){};
    SenMLIntRecord(ThingsMLMeasurementIndex index) : SenMLRecordTemplate(index){};
    SenMLIntRecord(ThingsMLMeasurementIndex index, int value) : SenMLRecordTemplate(index, value){};
    ~SenMLIntRecord(){};

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
    int getAdjustedValue();
    double getAdjustedValueD();
};

#endif // SENMLINTRECORD
