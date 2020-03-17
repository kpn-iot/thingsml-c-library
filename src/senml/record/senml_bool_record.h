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
 * support for bool sensor header
 */

#ifndef SENMLBOOLRECORD
#define SENMLBOOLRECORD

#include "senml_record_t.h"

/**
 * A SenMLRecord that stores boolean data.
 * This type of object can only be used for sensor data. If actuation is needed, use
 * SenMLBoolActuator instead.
 */
class SenMLBoolRecord : public SenMLRecord {
  public:
    SenMLBoolRecord(const char *name) : SenMLRecord(name){};
    SenMLBoolRecord(const char *name, SenMLUnit unit) : SenMLRecord(name, unit){};
    SenMLBoolRecord(const char *name, SenMLUnit unit, bool value) : SenMLRecord(name, unit), _value(value){};
    SenMLBoolRecord(ThingsMLMeasurementIndex index) : SenMLRecord(index){};
    SenMLBoolRecord(ThingsMLMeasurementIndex index, bool value) : SenMLRecord(index), _value(value){};
    ~SenMLBoolRecord(){};

    /**
     * renders all the fields to json, without the starting and ending brackets.
     * Inheriters can extend this function if they want to add extra fields to the json output
     * note: this is public so that custom implementations for the record object can use other
     * objects internally and render to json using this function (ex: coordinatesRecord using 3
     * floatRecrods for lat, lon & alt.
     * @returns: The number of bytes that were written.
     */
    virtual int fieldsToJson();

    /**
     * renders all the fields to cbor format. renders all the fields of the object without the
     * length info at the beginning note: this is public so that custom implementations for the
     * record object can use other objects internally and render to json using this function (ex:
     * coordinatesRecord using 3 floatRecrods for lat, lon & alt.
     * @returns: The number of bytes that were written.
     */
    virtual int fieldsToCbor();

    /**
     * Get the value assigned to this SenMLRecord.
     * This function always returns the absolute value, also when the record is part of a pack that
     * has a base value assigned.
     * @returns: the value (including the base-value when applicable).
     */
    bool get() {
        return _value;
    };

#ifdef __MBED__

    /**
     * Assign a value to the SenMLRecord. You can optionally assign the time at which the measurement was taken
     * and if it should be interpreted as a sum or regular value.
     * No timestamp is added and the value is interpreted as a regular value, not as a sum
     * @param  value the value to store in the record
     * @returns: true if the operation was succesful. See setTime() for more info when the operation can fail.
     */
    bool set(T value) {
        return this->set(value, (double)NAN);
    };

    /**
     * Assign a value to the SenMLRecord. You can optionally assign the time at which the measurement was taken
     * and if it should be interpreted as a sum or regular value.
     * @param  value the value to store in the record
     * @param time optional (default = NAN, meaning no time info). The time at which the measurement was taken.
     *             This should always be the absolute time value which will be converted relative to the base
     *             time when applicable (if the root is a SenMLPack with baseTime) . If you want to set the time
     *             manually relative to the basetime of the root-pack, then use setTime() instead.
     * @returns: true if the operation was succesful. See setTime() for more info when the operation can fail.
     */
    bool set(T value, double time)
#else

    /**
     * Assign a value to the SenMLRecord. You can optionally assign the time at which the measurement was taken
     * and if it should be interpreted as a sum or regular value.
     * @param  value the value to store in the record
     * @param time optional (default = NAN, meaning no time info). The time at which the measurement was taken.
     *             This should always be the absolute time value which will be converted relative to the base
     *             time when applicable (if the root is a SenMLPack with baseTime) . If you want to set the time
     *             manually relative to the basetime of the root-pack, then use setTime() instead.
     * @returns: true if the operation was succesful. See setTime() for more info when the operation can fail.
     */
    bool set(bool value, double time = (double)NAN)
#endif
    {
        this->_value = value;
        return this->setTime(time);
    };

  protected:
  private:
    bool _value = false; // Initialize else we get undefined behaviour
};

#endif // SENMLBOOLRECORD
