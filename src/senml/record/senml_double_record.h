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
 * support for double sensors header
 */

#ifndef SENMLDOUBLERECORD
#define SENMLDOUBLERECORD

#include "senml_record_t.h"

/**
 * A SenMLRecord that stores double data.
 * For cbor encoding this record automatically chooses the most efficient lossless storage method.
 * This means that when possible it will encode as half float or float.
 * This type of object can only be used for sensor data. If actuation is needed, use SenMLFloatActuator
 * instead.
 */
class SenMLDoubleRecord : public SenMLRecordTemplate<double> {
  public:
    SenMLDoubleRecord(const char *name) : SenMLRecordTemplate(name){};
    SenMLDoubleRecord(const char *name, SenMLUnit unit) : SenMLRecordTemplate(name, unit){};
    SenMLDoubleRecord(const char *name, SenMLUnit unit, double value) : SenMLRecordTemplate(name, unit, value){};
    SenMLDoubleRecord(ThingsMLMeasurementIndex index) : SenMLRecordTemplate(index){};
    SenMLDoubleRecord(ThingsMLMeasurementIndex index, double value) : SenMLRecordTemplate(index, value){};
    ~SenMLDoubleRecord(){};

    /**
     * renders all the fields to json, without the starting and ending brackets.
     * Inheriters can extend this function if they want to add extra fields to the json output
     * note: this is public so that custom implementations for the record object can use other objects
     * internally and render to json using this function (ex: coordinatesRecord using 3 doubleRecords for lat, lon &
     * alt.
     * @returns: The number of bytes that were written.
     */
    virtual int fieldsToJson();

    /**
     * renders all the fields to cbor format. renders all the fields of the object without the length info
     * at the beginning
     * note: this is public so that custom implementations for the record object can use other objects
     * internally and render to json using this function (ex: coordinatesRecord using 3 doubleRecords for
     * lat, lon & alt.
     * @returns: The number of bytes that were written.
     */
    virtual int fieldsToCbor();

  protected:
  private:
    double getAdjustedValue();
};

#endif // SENMLDOUBLERECORD
