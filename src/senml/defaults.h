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
 * Some defaults to make code run on multiple platforms.
 * 
 */

#ifndef THINGS_SENML_DEFAULTS
#define THINGS_SENML_DEFAULTS

// For low ram devices (less then 4KiB, this saves about 400 bytes)
#ifndef THINGSML_LOW_RAM_DEVICE
    // See http://electronics4dogs.blogspot.com/2011/01/arduino-predefined-constants.html
    #if defined(ARDUINO) && defined(__AVR_ATmega168__) // Arduino Decimilia and older
        #define THINGSML_LOW_RAM_DEVICE 1
    #elif defined(ARDUINO) && defined(__AVR_ATmega32U4__) // Arduino Leonardo
        #define THINGSML_LOW_RAM_DEVICE 1
    #elif defined(ARDUINO) && defined(__AVR_ATmega328P__) // Arduino Duemilanove and Uno
        #define THINGSML_LOW_RAM_DEVICE 1
    #endif
#endif

#ifdef __MBED__

#include "mbed.h"
#include "sstream"
#include <string>

#define String std::string

#elif defined(ARDUINO)

#include "Arduino.h"
#include <Stream.h>

#else

#include <string>

#define String std::string

#include <iostream>
class Stream {
  public:
    unsigned char read() {
        return 0;
    }
    void println(String string) {
        std::cout << string;
    }
    int available() {
        return 0;
    }
    void print(char p) {
        std::cout << p;
    }
    void write(const char p[], int len) {
        std::cout << p;
    }
};
#endif

#include "../util/base64.h"

#endif
