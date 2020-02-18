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
 * unit and record names
 * 
 */

#ifndef THINGSML_SENML_ENUMS
#define THINGSML_SENML_ENUMS

/**
 * Lists all the data types that a value can have in senml.
 * This is primarely used for actuator callbacks on SenMLPack objects when no appropirate SenMLRecord could be found for
 * the acutator command.
 */
enum SenMLDataType { // json data types:
    SENML_TYPE_NR,
    SENML_TYPE_STRING,
    SENML_TYPE_BOOL,
    SENML_TYPE_DATA,
    // cbor data types, which have already converted the data value
    CBOR_TYPE_UINT,
    CBOR_TYPE_INT,
    CBOR_TYPE_STRING,
    CBOR_TYPE_FLOAT,
    CBOR_TYPE_DOUBLE,
    CBOR_TYPE_BOOL,
    CBOR_TYPE_DATA
};

#ifdef ARDUINO
extern char pgmBuff[];
#endif
extern const char *const senml_units_names[];

/**
 * Determines how data should be read/rendered to the stream: in the original format
 * or converted to hex values. The latter is used for serial communication with a modem such as a lora modem.
 */
enum SenMLStreamMethod { SENML_HEX, SENML_RAW };

enum ThingsMLMeasurementIndex {
    THINGSML_NO_INDEX = -100,
    THINGSML_TEMPERATURE = -24,
    THINGSML_HUMIDITY = -23,
    THINGSML_LATITUDE = -22,
    THINGSML_LONGITUDE = -21,
    THINGSML_ALTITUDE = -20,
    THINGSML_POWER = -19,
    THINGSML_PRESSURE = -18,
    THINGSML_ANGLE = -17,
    THINGSML_LENGTH = -16,
    THINGSML_BREADTH = -15,
    THINGSML_HEIGHT = -14,
    THINGSML_WEIGHT = -13,
    THINGSML_THICKNESS = -12,
    THINGSML_DISTANCE = -11,
    THINGSML_AREA = -10,
    THINGSML_VOLUME = -9,
    THINGSML_VELOCITY = -8,
    THINGSML_ELECTRIC_CURRENT = -7,
    THINGSML_ELECTRIC_POTENTIAL = -6,
    THINGSML_ELECTRIC_RESISTANCE = -5,
    THINGSML_ILLUMINANCE = -4,
    THINGSML_ACCELERATION_X = -3,
    THINGSML_ACCELERATION_Y = -2,
    THINGSML_ACCELERATION_Z = -1,
    THINGSML_HEADING = 0,
    THINGSML_CO_CONCENTRATION = 1,
    THINGSML_CO2_CONCENTRATION = 2,
    THINGSML_SOUND = 3,
    THINGSML_FREQUENCY = 4,
    THINGSML_BATTERY_LEVEL = 5,
    THINGSML_BATTERY_VOLTAGE = 6,
    THINGSML_RADIUS = 7,
    THINGSML_BATTERY_LEVEL_LOW = 8,
    THINGSML_COMPASS_X = 9,
    THINGSML_COMPASS_Y = 10,
    THINGSML_COMPASS_Z = 11
};

/**
 * Lists all the units of measurement that are supported by SenML.
 */
enum SenMLUnit {
    SENML_UNIT_NONE,
    SENML_UNIT_METER,
    SENML_UNIT_KILOGRAM,
    SENML_UNIT_GRAM,
    SENML_UNIT_SECOND,
    SENML_UNIT_AMPERE,
    SENML_UNIT_KELVIN,
    SENML_UNIT_CANDELA,
    SENML_UNIT_MOLE,
    SENML_UNIT_HERTZ,
    SENML_UNIT_RADIAN,
    SENML_UNIT_STERADIAN,
    SENML_UNIT_NEWTON,
    SENML_UNIT_PASCAL,
    SENML_UNIT_JOULE,
    SENML_UNIT_WATT,
    SENML_UNIT_COULOMB,
    SENML_UNIT_VOLT,
    SENML_UNIT_FARAD,
    SENML_UNIT_OHM,
    SENML_UNIT_SIEMENS,
    SENML_UNIT_WEBER,
    SENML_UNIT_TESLA,
    SENML_UNIT_HENRY,
    SENML_UNIT_DEGREES_CELSIUS,
    SENML_UNIT_LUMEN,
    SENML_UNIT_LUX,
    SENML_UNIT_BECQUEREL,
    SENML_UNIT_GRAY,
    SENML_UNIT_SIEVERT,
    SENML_UNIT_KATAL,
    SENML_UNIT_SQUARE_METER,
    SENML_UNIT_CUBIC_METER,
    SENML_UNIT_LITER,
    SENML_UNIT_VELOCITY,
    SENML_UNIT_ACCELERATION,
    SENML_UNIT_CUBIC_METER_PER_SECOND,
    SENML_UNIT_LITER_PER_SECOND,
    SENML_UNIT_WATT_PER_SQUARE_METER,
    SENML_UNIT_CANDELA_PER_SQUARE_METER,
    SENML_UNIT_BIT,
    SENML_UNIT_BIT_PER_SECOND,
    SENML_UNIT_DEGREES_LATITUDE,
    SENML_UNIT_DEGREES_LONGITUDE,
    SENML_UNIT_PH,
    SENML_UNIT_DECIBEL,
    SENML_UNIT_DECIBEL_RELATIVE_TO_1_W,
    SENML_UNIT_BEL,
    SENML_UNIT_COUNTER,
    SENML_UNIT_RATIO,
    SENML_UNIT_RELATIVE_HUMIDITY,
    SENML_UNIT_PERCENTAGE_REMAINING_BATTERY_LEVEL,
    SENML_UNIT_SECONDS_REMAINING_BATTERY_LEVEL,
    SENML_UNIT_EVENT_RATE_PER_SECOND,
    SENML_UNIT_EVENT_RATE_PER_MINUTE,
    SENML_UNIT_BPM,
    SENML_UNIT_BEATS,
    SENML_UNIT_SIEMENS_PER_METER
};

#define SENML_NAME_TEMPERATURE "temperature"
#define SENML_NAME_HUMIDITY "humidity"
#define SENML_NAME_LATTITUDE "latitude"
#define SENML_NAME_LONGITUDE "longitude"
#define SENML_NAME_ALTITUDE "altitude"
#define SENML_NAME_POWER "power"
#define SENML_NAME_PRESSURE "pressure"
#define SENML_NAME_ANGLE "angle"
#define SENML_NAME_LENGTH "length"
#define SENML_NAME_BREADTH "breadth"
#define SENML_NAME_HEIGHT "height"
#define SENML_NAME_WEIGHT "weight"
#define SENML_NAME_THICKNESS "thickness"
#define SENML_NAME_DISTANCE "distance"
#define SENML_NAME_AREA "area"
#define SENML_NAME_VOLUME "volume"
#define SENML_NAME_VELOCITY "velocity"
#define SENML_NAME_ELECTRIC_CURRENT "electricCurrent"
#define SENML_NAME_ELECTRIC_POTENTIAL "electricPotential"
#define SENML_NAME_ELECTRIC_RESISTANCE "electricResistance"
#define SENML_NAME_ILLUMINANCE "illuminance"
#define SENML_NAME_ACCELERATION_X "accelerationX"
#define SENML_NAME_ACCELERATION_Y "accelerationY"
#define SENML_NAME_ACCELERATION_Z "accelerationZ"
#define SENML_NAME_HEADING "heading"
#define SENML_NAME_CO_CONCENTRATION "COConcentration"
#define SENML_NAME_CO2_CONCENTRATION "CO2Concentration"
#define SENML_NAME_SOUND "sound"
#define SENML_NAME_FREQUENCY "frequency"
#define SENML_NAME_BATTERY_LEVEL "batteryLevel"
#define SENML_NAME_BATTERY_VOLTAGE "batteryVoltage"
#define SENML_NAME_RADIUS "radius"
#define SENML_NAME_BATTERY_LEVEL_LOW "batteryLevelLow"
#define SENML_NAME_COMPASS_X "compassX"
#define SENML_NAME_COMPASS_Y "compassY"
#define SENML_NAME_COMPASS_Z "compassZ"

#endif // !THINGSML_SENML_ENUMS
