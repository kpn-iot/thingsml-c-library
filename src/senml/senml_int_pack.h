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
 * pack (document) with int base values header
 */

#ifndef THINGSML_SENML_INT_PACK
#define THINGSML_SENML_INT_PACK

#include "senml_pack_t.h"

/**
 * An implimentation of the SenMLPack template that stores integer base value and base-sum.
 */
class SenMLIntPack : public SenMLPackTemplate<int> {
  public:
    SenMLIntPack(const char *baseName) : SenMLPackTemplate(baseName, SENML_UNIT_NONE, NAN){};
    SenMLIntPack(const char *baseName, SenMLUnit baseUnit) : SenMLPackTemplate(baseName, baseUnit, NAN){};
    SenMLIntPack(const char *baseName, SenMLUnit baseUnit, double baseTime)
        : SenMLPackTemplate(baseName, baseUnit, baseTime){};

    SenMLIntPack(PACK_ACTUATOR_SIGNATURE) : SenMLPackTemplate(NULL, SENML_UNIT_NONE, NAN, callback){};
    SenMLIntPack(const char *baseName, PACK_ACTUATOR_SIGNATURE)
        : SenMLPackTemplate(baseName, SENML_UNIT_NONE, NAN, callback){};
    SenMLIntPack(const char *baseName, SenMLUnit baseUnit, PACK_ACTUATOR_SIGNATURE)
        : SenMLPackTemplate(baseName, baseUnit, NAN, callback){};
    SenMLIntPack(const char *baseName, SenMLUnit baseUnit, double baseTime, PACK_ACTUATOR_SIGNATURE)
        : SenMLPackTemplate(baseName, baseUnit, baseTime, callback){};

    ~SenMLIntPack(){};

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
    virtual void setupStreamCtx(Stream *dest, SenMLStreamMethod format);
    virtual void setupStreamCtx(char *dest, int length, SenMLStreamMethod format);

  private:
    int getAdjustedValue();
};

#endif // THINGSML_SENML_INT_PACK
