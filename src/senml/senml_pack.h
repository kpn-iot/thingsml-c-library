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
 * pack (document) without base values headers
 */

#ifndef THINGSML_SENML_PACK
#define THINGSML_SENML_PACK

#include "defaults.h"
#include "senml_basepack.h"

#define PACK_ACTUATOR_SIGNATURE void (*callback)(const char *, const char *, const void *, int, SenMLDataType)

/**
 * SenMLPack represents a single senml document that can be sent or received.
 *
 * A senmlPack object has SenMLRecords and/or other SenMLPack objects as children. These
 * represent the data that the object contains.
 * A SenMLRecord represents a single value, while child SenMLPacks represent data of other
 * devices. When a SenMLPack contains other SenMLPack objects, the root object represents a
 * gateway.
 *
 * A SenMLPack object is able to render and parse the data to/from a json string or to/from binary
 * CBOR data. Both rendering and parsing can be done either directly from a stream (like an UART
 * connected to a modem), or from a memory buffer.
 * Rendering to and parsing from a stream is useful for devices that have extreme low memory available.
 * Almost no buffers are used in this case. Error handling is limited in this case though.
 * The parser and generator are able to render in the native format (strings for json, binary data for
 * cbor) and as a hex string. This is especially useful when directly working on a stream: some modems
 * (ex lora) accept instructions with data in HEX format.
 *
 * example:
 *
 * @code
 * #include <thingsml.h>
 *
 * SenMLPack doc;
 * SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
 *
 * void setup(){
 *     doc.add(temperature);
 *     Serial.begin(57600);
 *     senMLSetLogger(Serial);
 *     delay(1000);
 *     Serial.println("start");
 * }

 * void loop(){
 *     int val = 10;                                  //just give it some value
 *     doc.toJson(Serial);                            //as text
 *     Serial.println();
 *     doc.toJson(Serial, SENML_HEX);                 //in hex format (often used in communication with lora modems)
 *     Serial.println();
 *     delay(1000);
 * }

 * @endcode
 *
 */
class SenMLPack : public SenMLBasePack {
    friend class SenMLJsonListener;
    friend class SenMLCborParser;

  public:
    /**
     * create a SenMLPack object.
     */
    SenMLPack() : _bn(NULL), _bu(SENML_UNIT_NONE), _bt(NAN){};

    /**
     * create a SenMLPack object.
     * @param baseName the string that will be prepended to all records in this pack.
     *                 Is used to represent the name of the device.
     */
    SenMLPack(const char *baseName)
        : _bn(baseName), // mbed compiler doesnt support delegating constructors
          _bu(SENML_UNIT_NONE), _bt(NAN){};

    /**
     * create a SenMLPack object.
     * @param baseName the string that will be prepended to all records in this pack.
     *                 Is used to represent the name of the device.
     * @param baseUnit the unit name that will be used by default if the record doesnt
     *                 not define one.
     */
    SenMLPack(const char *baseName, SenMLUnit baseUnit) : _bn(baseName), _bu(baseUnit), _bt(NAN){};

    /**
     * create a SenMLPack object.
     * @param baseName the string that will be prepended to all records in this pack.
     *                 Is used to represent the name of the device.
     * @param baseUnit the unit name that will be used by default if the record doesnt
     *                 not define one.
     * @param baseTime the time that will be added to each record. When specified, each
     *                 record that does not specify a time, will receive this time. When
     *                 the record does have a time, the baseTime of the pack is added to it,
     *                 so the time of the record becomes relative to that of the pack.
     */
    SenMLPack(const char *baseName, SenMLUnit baseUnit, double baseTime)
        : _bn(baseName), _bu(baseUnit), _bt(baseTime){};

    /**
     * create a SenMLPack object.
     * @param callback a function that will be called while parsing incomming data, when no
     *                 record can be found that matches any of the defined ones in the object.
     *                 The parameters of the callback must be:
     *                    const char* packName the name of the pack that the record belongs to.
     *                                         The data is for a child SenMLPack when this
     *                                         field is different then the name of the root pack.
     *                    const char* recordName the name of the record
     *                    const void* value a pointer to the memory blob that contains the actual value.
     *                    int size the size of the memory blobl
     *                    SenMLDataType dataType: defines how to interprete the memory blob (ex: pointer to integer,..)
     */
    SenMLPack(PACK_ACTUATOR_SIGNATURE) : _bn(NULL), _bu(SENML_UNIT_NONE), _bt(NAN), callback(callback){};

    /**
     * create a SenMLPack object.
     * @param baseName the string that will be prepended to all records in this pack.
     *                 Is used to represent the name of the device.
     * @param callback a function that will be called while parsing incomming data, when no
     *                 record can be found that matches any of the defined ones in the object.
     *                 The parameters of the callback must be:
     *                    const char* packName the name of the pack that the record belongs to.
     *                                         The data is for a child SenMLPack when this
     *                                         field is different then the name of the root pack.
     *                    const char* recordName the name of the record
     *                    const void* value a pointer to the memory blob that contains the actual value.
     *                    int size the size of the memory blobl
     *                    SenMLDataType dataType: defines how to interprete the memory blob (ex: pointer to integer,..)
     */
    SenMLPack(const char *baseName, PACK_ACTUATOR_SIGNATURE)
        : _bn(baseName), _bu(SENML_UNIT_NONE), _bt(NAN), callback(callback){};

    /**
     * create a SenMLPack object.
     * @param baseName the string that will be prepended to all records in this pack.
     *                 Is used to represent the name of the device.
     * @param baseUnit the unit name that will be used by default if the record doesnt
     *                 not define one.
     * @param callback a function that will be called while parsing incomming data, when no
     *                 record can be found that matches any of the defined ones in the object.
     *                 The parameters of the callback must be:
     *                    const char* packName the name of the pack that the record belongs to.
     *                                         The data is for a child SenMLPack when this
     *                                         field is different then the name of the root pack.
     *                    const char* recordName the name of the record
     *                    const void* value a pointer to the memory blob that contains the actual value.
     *                    int size the size of the memory blobl
     *                    SenMLDataType dataType: defines how to interprete the memory blob (ex: pointer to integer,..)
     */
    SenMLPack(const char *baseName, SenMLUnit baseUnit, PACK_ACTUATOR_SIGNATURE)
        : _bn(baseName), _bu(baseUnit), _bt(NAN), callback(callback){};

    /**
     * create a SenMLPack object.
     * @param baseName the string that will be prepended to all records in this pack.
     *                 Is used to represent the name of the device.
     * @param baseUnit the unit name that will be used by default if the record doesnt
     *                 not define one.
     * @param baseTime the time that will be added to each record. When specified, each
     *                 record that does not specify a time, will receive this time. When
     *                 the record does have a time, the baseTime of the pack is added to it,
     *                 so the time of the record becomes relative to that of the pack.
     * @param callback a function that will be called while parsing incomming data, when no
     *                 record can be found that matches any of the defined ones in the object.
     *                 The parameters of the callback must be:
     *                    const char* packName the name of the pack that the record belongs to.
     *                                         The data is for a child SenMLPack when this
     *                                         field is different then the name of the root pack.
     *                    const char* recordName the name of the record
     *                    const void* value a pointer to the memory blob that contains the actual value.
     *                    int size the size of the memory blobl
     *                    SenMLDataType dataType: defines how to interprete the memory blob (ex: pointer to integer,..)
     */
    SenMLPack(const char *baseName, SenMLUnit baseUnit, double baseTime, PACK_ACTUATOR_SIGNATURE)
        : _bn(baseName), _bu(baseUnit), _bt(baseTime), callback(callback){};

    /**
     * destroys the SenMLPack object.
     */
    ~SenMLPack(){};

    /**
     * assign a basename to the SenMLPack object. This represents the name of the device.
     * see the spec on [base fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info.
     * Every SenMLPack object must have a basename. This field will always be rendered in the output, even if the
     * string is empty.
     * @param name an immutable string that will be used to represent the name of the device. An internal
     * copy of the value will be made.
     * @returns none
     */
    void setBaseName(const char *name);

    /**
     * Get the base name. see the spec on [base
     * fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info.
     * @returns the name of the device as an immutable string.
     */
    const char *getBaseName();

    /**
     * Set the base unit that will be used as the default unit for all records that don't define their own unit.
     * see the spec on [base fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info.
     * Set to SENML_UNIT_NONE for ommiting the base unit from the output (default).
     * @param unit the unit to use as default. See SenMLUnit for all supported unit names.
     * @returns none
     */
    void setBaseUnit(SenMLUnit unit);

    /**
     * Get the base unit. see the spec on [base
     * fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info.
     * @returns a SenMLUnit enum value that is used as the default unit for records that don't define a unit of their
     * own.
     */
    inline SenMLUnit getBaseUnit() {
        return this->_bu;
    };

    /**
     * Set the base time. see the spec on [base
     * fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info.
     * @param time the value to use as base time. set bt to NaN if the field should not be included in the output.
     * @returns none
     */
    void setBaseTime(double time);

    /**
     * Set the base time without adjusting all it's records.
     * see the spec on [basefields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info.
     * @param time the value to use as base time. set bt to NaN if the field should not be included in the output.
     * @returns none
     */
    void setBaseTimeSilently(double time);

    /**
     * Get the base time. see the spec on [base
     * fields](https://tools.ietf.org/html/draft-ietf-core-senml-13#section-4.1) for more info.
     * @returns a double value that is used as the default unit for records that don't define a unit of their own.
     *          if no base time is set, NaN will be returned.
     */
    inline double getBaseTime() {
        return this->_bt;
    };

    /**
     * renders all the fields to json, without the starting and ending brackets.
     * Inheriters can extend this function if they want to add extra fields to the json output
     * note: this is public so that custom implementations for the record object can use other objects
     * internally and render to json using this function (ex: coordinatesRecord using 3 floatRecrods for lat, lon & alt.
     * @returns: None
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
    bool isActuator() {
        return true;
    }

    // when the user did not register a specific record actuator, this will be called.
    inline void actuate(const char *pack, const char *record, const void *value, int valueLength,
                        SenMLDataType dataType) {
        if (this->callback)
            this->callback(pack, record, value, valueLength, dataType);
    };

    virtual int getFieldLength();

  private:
    const char *_bn = NULL;
    SenMLUnit _bu;
    double _bt;

    PACK_ACTUATOR_SIGNATURE = NULL;
};

#endif // THINGSML_SENML_PACK
