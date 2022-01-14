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
 * record base class
 */

#ifndef SENMLTIMED
#define SENMLTIMED

#include "../defaults.h"
#include "../senml_base.h"
#include "../senml_enums.h"

/**
 * SenMLRecord represents a single measurement. Besides the value, it also stores the name,
 * optional unit, timestamp and update time.
 *
 * This is the base class for all record types, you can not work directly with this class.
 * Instead, use one of it's derivatives such as SenMLBinaryRecord, SenMLBoolRecord, SenMLFloatRecord,
 * SenMLIntRecord or SenMLStringRecord. Alternatively, you can create your own records by inheriting from
 * this class, or SenMLRecordTemplate, a convenience template which allows you to create records for
 * different data types.
 *
 * Although a record object can be used by itself, usually record objects are added to SenMLPack objects.
 */
class SenMLRecord : public SenMLBase {
    friend class SenMLCborParser;
    friend class SenMLJsonListener;

  public:
    /**
     * create a SenMLRecord object.
     */
    SenMLRecord() : _index(THINGSML_NO_INDEX), _unit(SENML_UNIT_NONE), _time(NAN), _updateTime(0){};

    /**
     * create a SenMLRecord object.
     * @param name the identifier for this record. This is a free-form string, but a set of
     *             predefined names, supported by the KPN network can be found in senml_enums.h
     */
    SenMLRecord(const char *name)
        : _index(THINGSML_NO_INDEX), _name(name), _unit(SENML_UNIT_NONE), _time(NAN), _updateTime(0){};

    SenMLRecord(ThingsMLMeasurementIndex index) : _index(index), _unit(SENML_UNIT_NONE), _time(NAN), _updateTime(0){};
    /**
     * create a SenMLRecord object.
     * @param name the string that will be prepended to all records in this pack.
     *                 Is used to represent the name of the device.
     * @param unit the unit that should be included in the output. See the SenMLUnit enum for
     *             supported unit types.
     */
    SenMLRecord(const char *name, SenMLUnit unit)
        : _index(THINGSML_NO_INDEX), _name(name), _unit(unit), _time(NAN), _updateTime(0){};

    /**
     * returns the time assigned to this record. NaN represents 'no time assigned'.
     * See [base fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info on the time
     * value.
     * @returns the timestamp assigned to the record, expressed in unix epoch, relative to the
     *          parent SenMLPack object's base time (if any).
     */
    double getTime() {
        return this->_time;
    };

    /**
     * set the time, expressed as a unix epoch time, absolute or relative to the base time of the
     * parent SenMLPack object..
     * when absolute is true (default behaviour), the time value will be made relative to the
     * base time of the pack object, if it has a base time.
     * See [base fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info on the time
     * value. Possible reasons for failure:
     *  - if there is a root object, but it is not a SenMLPack
     *  - if absolute is false, but there is no parent SenMLPack object.
     * @param value the unix epoch time value to assign to the record
     * @param absolute When true (default), the value will be interpreted as an absolute time stamp. If there
     *                 is a parent SenMLPack, the value will be made relative to the pack's base time, if there is any.
     *                 When false, no conversion is done, but a parent SenMLPack has to be present.
     * @returns true upon success, otherwise false.
     */
    bool setTime(double value, bool absolute = true);

    /**
     * set the time, expressed as a unix epoch time.
     * This assigns the value directly without taking the parent senml pack into consideration.
     * Use this only when you are calculating the values in a way that is not compatible with the functionality of setTime();
     * See [base fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info on the time value. 
     * @param value the unix epoch time value to assign to the record
     */
    void setTimeDirect(double value) {
        this->_time = value;
    }

    /**
     * Get the name of the record.
     * @returns the name of the record as an immutable string. Don't delete this pointer, it refers to the
     *          internal buffer of the object and is managed by the record.
     */
    inline const char *getName() {
        return this->_name;
    }

    inline ThingsMLMeasurementIndex getIndex() {
        return this->_index;
    }

    /**
     * Assign an identifier to the record. This is a free-form string, but a set of
     * predefined names, supported by the KPN network can be found in senml_enums.h
     * @param name a string that represents the identifier for the record. A copy of the value
     *             is made.
     * @returns none
     */
    inline void setName(const char *name) {
        this->_name = name;
    }

    /**
     * Get the expected timestamp at which this record will be updated again. NaN represents 'no time assigned'.
     * @returns a unix epoch time, relative to the parent SenMLPack object's base time (if any).
     */
    inline double getUpdateTime() {
        return this->_updateTime;
    }

    /**
     * Assign a timestamp to the record at which it is expected to update the value.
     * See [base fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info on the time
     * 
     * Note: previously update time was interpreted as being relative, however this is not in line with the spec.
     * the optional paramter absolute will now be ignored and true will always be returned.
     *
     * @param value the unix epoch update time value to assign to the record
     * @param absolute ignored. previously updatetime was interpreted as being relative to basetime, this is not in line with the spec however.
     * @returns always returns true
     */
    bool setUpdateTime(double value, bool absolute = true);

    /**
     * Get the enum that identifies the unit name assigned to the record.
     * The value SENML_UNIT_NONE means that no unit will be generated in the output.
     * Senml defines a fixed set of supported [unit
     * names](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-12.1).
     * @returns a SenMLUnit enum value representing the unit name.
     */
    SenMLUnit getUnit() {
        return this->_unit;
    }

    /**
     * Assigns a unit value to the record.
     * The value SENML_UNIT_NONE means that no unit will be generated in the output.
     * Senml defines a fixed set of supported [unit
     * names](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-12.1).
     * @param value a SenMLUnit enum value representing the unit name.
     * @returns none
     */
    void setUnit(SenMLUnit value) {
        this->_unit = value;
    }

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
    /*
    renders all the fields to json, including the starting and ending brackets.
    Inheriters can extend this function if they want to add extra fields to the json output
    */
    virtual int contentToJson();

    // renders all the fields to cbor format. renders all the fields of the object including the
    // length info at the beginning
    virtual int contentToCbor();

    // This function is called by the root SenMLPack object to indicate that the object
    // should adjust it's time info relative to the new base time (if applicable)
    // doesn't do anything by default
    virtual void adjustToBaseTime(double prev, double time);

    // called while parsing a senml message, when the parser found the value for an SenMLJsonListener
    virtual void actuate(const void *value, int dataLength, SenMLDataType dataType);

    // calculates the nr of fields that this record will produce.
    // The default implementation already adds 1 field for the value.
    int getFieldLength();

  private:
    ThingsMLMeasurementIndex _index;
    const char *_name = NULL;
    SenMLUnit _unit;
    double _time;
    double _updateTime;
};

#endif // SENMLRECORD
