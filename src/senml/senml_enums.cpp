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
 * Enum values, defined this way to optimize ram usage on low ram arduino devices
 * 
 */

#include "senml_enums.h"

#ifdef THINGSML_LOW_RAM_DEVICE
#include <avr/pgmspace.h>
char pgmBuff[9] = {0};
#endif


const char str1[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= ""; 
const char str2[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "m"; 
const char str3[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "kg"; 
const char str4[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "g"; 
const char str5[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "s"; 
const char str6[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "A"; 
const char str7[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "K"; 
const char str8[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "cd"; 
const char str9[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "mol"; 
const char str10[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Hz"; 
const char str11[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "rad"; 
const char str12[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "sr"; 
const char str13[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "N"; 
const char str14[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Pa"; 
const char str15[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "J"; 
const char str16[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "W"; 
const char str17[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "C"; 
const char str18[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "V";
const char str19[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "F";
const char str20[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Ohm";
const char str21[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "S";
const char str22[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Wb";
const char str23[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "T";
const char str24[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "H";
const char str25[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Cel";
const char str26[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "lm";
const char str27[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "lx";
const char str28[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Bq";
const char str29[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Gy";
const char str30[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Sv";
const char str31[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "kat";
const char str32[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "m2";
const char str33[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "m3";
const char str34[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "l";
const char str35[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "m/s";
const char str36[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "m/s2";
const char str37[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "m3/s";
const char str38[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "l/s";
const char str39[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "W/m2";
const char str40[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "cd/m2";
const char str41[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "bit";
const char str42[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "bit/s";
const char str43[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "lat";
const char str44[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "lon";
const char str45[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "pH";
const char str46[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "dB";
const char str47[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "dBW";
const char str48[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "Bspl";
const char str49[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "count";
const char str50[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "//";
const char str51[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "%RH";
const char str52[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "%EL";
const char str53[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "EL";
const char str54[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "1/s";
const char str55[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "1/min";
const char str56[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "beat/min";
const char str57[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "beats";
const char str58[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "S/m";
const char str59[] 
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
= "ppm";


const char * const senml_units_names[]
#ifdef THINGSML_LOW_RAM_DEVICE
PROGMEM
#endif
 = { 
    str1,
    str2,
    str3,
    str4,
    str5,
    str6,
    str7,
    str8,
    str9,
    str10,
    str11,
    str12,
    str13,
    str14,
    str15,
    str16,
    str17,
    str18,
    str19,
    str20,
    str21,
    str22,
    str23,
    str24,
    str25,
    str26,
    str27,
    str28,
    str29,
    str30,
    str31,
    str32,
    str33,
    str34,
    str35,
    str36,
    str37,
    str38,
    str39,
    str40,
    str41,
    str42,
    str43,
    str44,
    str45,
    str46,
    str47,
    str48,
    str49,
    str50,
    str51,
    str52,
    str53,
    str54,
    str55,
    str56,
    str57,
    str58,
    str59
};