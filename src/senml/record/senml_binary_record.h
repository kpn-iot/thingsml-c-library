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
 * support for binary sensors header
 */

#ifndef SENMLBINARYRECORD
#define SENMLBINARYRECORD

#include "senml_record.h"

/**
 * A SenMLRecord that stores binary data.
 * This type of object can only be used for sensor data. If actuation is needed, use SenMLBinaryActuator
 * instead.
 */
class SenMLBinaryRecord : public SenMLRecord {
  public:
    SenMLBinaryRecord(const char *name) : SenMLRecord(name){};
    SenMLBinaryRecord(const char *name, SenMLUnit unit) : SenMLRecord(name, unit){};
    SenMLBinaryRecord(ThingsMLMeasurementIndex idx) : SenMLRecord(idx){};

#ifdef __MBED__

    /**
     * set the value and length.
     * warning: value is not copied over. a direct reference to the buffer is stored, so if this
     * memory is changed/freed, then the data stored in the object will also be changed.
     */
    bool set(unsigned char *value, unsigned int length) {
        return this->set(value, length, NAN);
    }

    /**
     * set the value and length.
     * warning: value is not copied over. a direct reference to the buffer is stored, so if this
     * memory is changed/freed, then the data stored in the object will also be changed.
     */
    bool set(unsigned char *value, unsigned int length, double time);

#else
    /**
     * set the value and length.
     * warning: value is not copied over. a direct reference to the buffer is stored, so if this
     * memory is changed/freed, then the data stored in the object will also be changed.
     */
    bool set(unsigned char *value, unsigned int length, double time = NAN);

#endif

    unsigned char * get();

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
    unsigned char *_value; // raw data buffer (not null terminated)
    unsigned int _length;  // size of the _data packet
};

#endif // SENMLBINARYRECORD
