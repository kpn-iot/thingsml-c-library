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
 * pack (document) base class for packs that have base values
 */

#ifndef THINGSML_SENML_PACK_TEMPLATE
#define THINGSML_SENML_PACK_TEMPLATE

#include "senml_pack.h"

/**
 * A template class that can be used to create new SenMLPack types that store a base-value and/or base-sum
 * with a basic data type (no structs or classes).
 * When you create a new class, you should always implement the following functions in order
 * for the new class to operate correctly: fieldsToJson() and fieldsToCbor(). These functions are responsible
 * for rendering both base-value and base-sum. This class does not implement any rendering.
 * See previous implementations such as SenMLIntPack for inspiration.
 */
template <class T> class SenMLPackTemplate : public SenMLPack {
  public:
    SenMLPackTemplate(const char *baseName) : SenMLPack(baseName, SENML_UNIT_NONE, NAN){};
    SenMLPackTemplate(const char *baseName, SenMLUnit baseUnit) : SenMLPack(baseName, baseUnit, NAN){};
    SenMLPackTemplate(const char *baseName, SenMLUnit baseUnit, double baseTime)
        : SenMLPack(baseName, baseUnit, baseTime){};

    SenMLPackTemplate(PACK_ACTUATOR_SIGNATURE) : SenMLPack(NULL, SENML_UNIT_NONE, NAN, callback){};
    SenMLPackTemplate(const char *baseName, PACK_ACTUATOR_SIGNATURE)
        : SenMLPack(baseName, SENML_UNIT_NONE, NAN, callback){};
    SenMLPackTemplate(const char *baseName, SenMLUnit baseUnit, PACK_ACTUATOR_SIGNATURE)
        : SenMLPack(baseName, baseUnit, NAN, callback){};
    SenMLPackTemplate(const char *baseName, SenMLUnit baseUnit, double baseTime, PACK_ACTUATOR_SIGNATURE)
        : SenMLPack(baseName, baseUnit, baseTime, callback){};

    ~SenMLPackTemplate(){};

    /**
     * Get the base-sum assigned to this pack object.
     * @returns: the base-sum.
     */
    T getBaseSum() {
        return _sum;
    };

    /**
     * Store the base-sum in the pack object.
     * @returns: true (returns a value to support possible future extentions)
     */
    bool setBaseSum(T value) {
        _sum = value;
        return true;
    };

    /**
     * Get the base-value assigned to this pack object.
     * @returns: the base-value.
     */
    T getBaseValue() {
        return _value;
    };

    /**
     * Store the base-value in the pack object.
     * @returns: true (returns a value to support possible future extentions)
     */
    bool setBaseValue(T value) {
        _value = value;
        return true;
    };

  protected:
  private:
    T _sum;
    T _value;
};

#endif // THINGSML_SENML_PACK_TEMPLATE
