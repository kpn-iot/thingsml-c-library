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

#ifdef __MBED__
#include "mbed.h"
#include "sstream"
#include <string>
using namespace std;
#define String string
#elif defined(ARDUINO)
#include "Arduino.h"
#include <stream.h>
#else
#include <string>
using namespace std;
#define String string

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