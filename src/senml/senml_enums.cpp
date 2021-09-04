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
 * Enum values, defined this way to optimize ram usage on arduino
 * 
 */

#include "senml_enums.h"


#ifdef ARDUINO
#if defined ESP32 || defined ESP8266
#include <pgmspace.h>
#else
#include <avr\pgmspace.h>
#endif
char pgmBuff[9] = {0};
#endif


const char str1[] 
#ifdef ARDUINO
PROGMEM
#endif
= ""; 
const char str2[] 
#ifdef ARDUINO
PROGMEM
#endif
= "m"; 
const char str3[] 
#ifdef ARDUINO
PROGMEM
#endif
= "kg"; 
const char str4[] 
#ifdef ARDUINO
PROGMEM
#endif
= "g"; 
const char str5[] 
#ifdef ARDUINO
PROGMEM
#endif
= "s"; 
const char str6[] 
#ifdef ARDUINO
PROGMEM
#endif
= "A"; 
const char str7[] 
#ifdef ARDUINO
PROGMEM
#endif
= "K"; 
const char str8[] 
#ifdef ARDUINO
PROGMEM
#endif
= "cd"; 
const char str9[] 
#ifdef ARDUINO
PROGMEM
#endif
= "mol"; 
const char str10[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Hz"; 
const char str11[]
#ifdef ARDUINO
PROGMEM
#endif
= "rad"; 
const char str12[]
#ifdef ARDUINO
PROGMEM
#endif
= "sr"; 
const char str13[]
#ifdef ARDUINO
PROGMEM
#endif
= "N"; 
const char str14[]
#ifdef ARDUINO
PROGMEM
#endif
= "Pa"; 
const char str15[]
#ifdef ARDUINO
PROGMEM
#endif
= "J"; 
const char str16[]
#ifdef ARDUINO
PROGMEM
#endif
= "W"; 
const char str17[]
#ifdef ARDUINO
PROGMEM
#endif
= "C"; 
const char str18[] 
#ifdef ARDUINO
PROGMEM
#endif
= "V";
const char str19[] 
#ifdef ARDUINO
PROGMEM
#endif
= "F";
const char str20[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Ohm";
const char str21[] 
#ifdef ARDUINO
PROGMEM
#endif
= "S";
const char str22[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Wb";
const char str23[] 
#ifdef ARDUINO
PROGMEM
#endif
= "T";
const char str24[] 
#ifdef ARDUINO
PROGMEM
#endif
= "H";
const char str25[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Cel";
const char str26[] 
#ifdef ARDUINO
PROGMEM
#endif
= "lm";
const char str27[] 
#ifdef ARDUINO
PROGMEM
#endif
= "lx";
const char str28[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Bq";
const char str29[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Gy";
const char str30[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Sv";
const char str31[]
#ifdef ARDUINO
PROGMEM
#endif
= "kat";
const char str32[] 
#ifdef ARDUINO
PROGMEM
#endif
= "m2";
const char str33[] 
#ifdef ARDUINO
PROGMEM
#endif
= "m3";
const char str34[] 
#ifdef ARDUINO
PROGMEM
#endif
= "l";
const char str35[] 
#ifdef ARDUINO
PROGMEM
#endif
= "m/s";
const char str36[] 
#ifdef ARDUINO
PROGMEM
#endif
= "m/s2";
const char str37[] 
#ifdef ARDUINO
PROGMEM
#endif
= "m3/s";
const char str38[] 
#ifdef ARDUINO
PROGMEM
#endif
= "l/s";
const char str39[] 
#ifdef ARDUINO
PROGMEM
#endif
= "W/m2";
const char str40[] 
#ifdef ARDUINO
PROGMEM
#endif
= "cd/m2";
const char str41[] 
#ifdef ARDUINO
PROGMEM
#endif
= "bit";
const char str42[] 
#ifdef ARDUINO
PROGMEM
#endif
= "bit/s";
const char str43[] 
#ifdef ARDUINO
PROGMEM
#endif
= "lat";
const char str44[] 
#ifdef ARDUINO
PROGMEM
#endif
= "lon";
const char str45[] 
#ifdef ARDUINO
PROGMEM
#endif
= "pH";
const char str46[] 
#ifdef ARDUINO
PROGMEM
#endif
= "db";
const char str47[] 
#ifdef ARDUINO
PROGMEM
#endif
= "dBW";
const char str48[] 
#ifdef ARDUINO
PROGMEM
#endif
= "Bspl";
const char str49[] 
#ifdef ARDUINO
PROGMEM
#endif
= "count";
const char str50[] 
#ifdef ARDUINO
PROGMEM
#endif
= "//";
const char str51[] 
#ifdef ARDUINO
PROGMEM
#endif
= "%RH";
const char str52[] 
#ifdef ARDUINO
PROGMEM
#endif
= "%EL";
const char str53[] 
#ifdef ARDUINO
PROGMEM
#endif
= "EL";
const char str54[] 
#ifdef ARDUINO
PROGMEM
#endif
= "1/s";
const char str55[] 
#ifdef ARDUINO
PROGMEM
#endif
= "1/min";
const char str56[] 
#ifdef ARDUINO
PROGMEM
#endif
= "beat/min";
const char str57[] 
#ifdef ARDUINO
PROGMEM
#endif
= "beats";
const char str58[] 
#ifdef ARDUINO
PROGMEM
#endif
= "S/m";
const char str59[] 
#ifdef ARDUINO
PROGMEM
#endif
= "ppm";


const char * const senml_units_names[]
#ifdef ARDUINO
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
