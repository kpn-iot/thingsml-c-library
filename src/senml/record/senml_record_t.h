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
 * record base class for simple data types
 */

#ifndef SENMLRECORDTEMPLATE
#define SENMLRECORDTEMPLATE

#include "senml_record.h"

/**
 * A template class that can be used to create new SenMLRecord types that store a value with a basic
 * data type (no structs or classes).
 * When you create a new class, you should always implement the following functions in order
 * for the new class to operate correctly: fieldsToJson() and fieldsToCbor()
 */
template <class T> class SenMLRecordTemplate : public SenMLRecord {
  public:
    SenMLRecordTemplate(const char *name) : SenMLRecord(name), _valAsSum(false){};
    SenMLRecordTemplate(const char *name, SenMLUnit unit) : SenMLRecord(name, unit), _valAsSum(false){};
    SenMLRecordTemplate(const char *name, SenMLUnit unit, T value)
        : SenMLRecord(name, unit), _value(value), _valAsSum(false){};
    SenMLRecordTemplate(ThingsMLMeasurementIndex index) : SenMLRecord(index){};
    SenMLRecordTemplate(ThingsMLMeasurementIndex index, T value)
        : SenMLRecord(index), _value(value), _valAsSum(false){};

    /**
     * Get the value assigned to this SenMLRecord.
     * This function always returns the absolute value, also when the record is part of a pack that
     * has a base value assigned.
     * @returns: the value (including the base-value when applicable).
     */
    T get() {
        return _value;
    };

    /**
     * Get if the value should be interpreted as a sum or regular value. These are exclusive: a SenMLRecord
     * either has a sum or a value, but never both.
     * Sum and value are always assigned with the function set().
     * @returns: True when the value is interpreted as a sum, otherwise false.
     */
    bool asSum() {
        return this->_valAsSum;
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
        return this->set(value, (double)NAN, false);
    };

    /**
     * Assign a value to the SenMLRecord. You can optionally assign the time at which the measurement was taken
     * and if it should be interpreted as a sum or regular value.
     * The value is interpreted as a regular value, not as a sum
     * @param  value the value to store in the record
     * @param time optional (default = NAN, meaning not time info). The time at which the measurement was taken.
     *             This should always be the absolute time value which will be converted relative to the base
     *             time when applicable (if the root is a SenMLPack with baseTime) . If you want to set the time
     *             manually relative to the basetime of the root-pack, then use setTime() instead.
     * @returns: true if the operation was succesful. See setTime() for more info when the operation can fail.
     */
    bool set(T value, double time) {
        return this->set(value, time, false);
    };

    /**
     * Assign a value to the SenMLRecord. You can optionally assign the time at which the measurement was taken
     * and if it should be interpreted as a sum or regular value.
     * @param  value the value to store in the record
     * @param time optional (default = NAN, meaning not time info). The time at which the measurement was taken.
     *             This should always be the absolute time value which will be converted relative to the base
     *             time when applicable (if the root is a SenMLPack with baseTime) . If you want to set the time
     *             manually relative to the basetime of the root-pack, then use setTime() instead.
     * @param asSum when true, the value will be interpreted as a sum, otherwise as a regular value.
     * @returns: true if the operation was succesful. See setTime() for more info when the operation can fail.
     */
    bool set(T value, double time, bool asSum)

#else

    /**
     * Assign a value to the SenMLRecord. You can optionally assign the time at which the measurement was taken
     * and if it should be interpreted as a sum or regular value.
     * @param  value the value to store in the record
     * @param time optional (default = NAN, meaning not time info). The time at which the measurement was taken.
     *             This should always be the absolute time value which will be converted relative to the base
     *             time when applicable (if the root is a SenMLPack with baseTime) . If you want to set the time
     *             manually relative to the basetime of the root-pack, then use setTime() instead.
     * @param asSum when true, the value will be interpreted as a sum, otherwise as a regular value.
     * @returns: true if the operation was succesful. See setTime() for more info when the operation can fail.
     */
    bool set(T value, double time = (double)NAN, bool asSum = false)
#endif
    {
        this->_value = value;
        this->_valAsSum = asSum;
        return this->setTime(time);
    };

  protected:
  private:
    T _value;
    bool _valAsSum;
};

#endif // SENMLRECORDTEMPLATE
