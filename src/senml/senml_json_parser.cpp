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
 * parse json
 */

#include "senml_json_parser.h"
#include "senml_jsonlistener.h"
#include "senml_helpers.h"
#include <string.h>

#define NONE 0
#define BN_VALUE 1
#define BU_VALUE 2
#define N_VALUE 3
#define V_VALUE 4
#define VB_VALUE 5
#define VD_VALUE 6
#define VS_VALUE 7
#define BV_VALUE 8
#define I_VALUE 9

#ifdef __MBED__
#define KEY1 key[1]
#elif defined(ARDUINO)
#define KEY1 key.charAt(1)
#else
#define KEY1 key[1]
#endif

void SenMLJsonListener::key(String key) {
    this->expected = NONE;
    if (key.length() > 0) { // using faster char compare instead of string compare
#ifdef __MBED__
        char first = key[0];
#elif defined(ARDUINO)
        char first = key.charAt(0);
#else
        char first = key[0];
#endif
        switch (first) {
            case 'i':
                if (key.length() == 2) {
                    switch (KEY1) {
                        case '_':
                            this->expected = I_VALUE;
                            break;
                    }
                }
                break;
            case 'b':
                if (key.length() == 2) {
                    switch (KEY1) {
                        case 'n':
                            this->expected = BN_VALUE;
                            break;
                        case 'u':
                            this->expected = BU_VALUE;
                            break;
                        case 'v':
                            this->expected = BV_VALUE;
                            break;
                    }
                }
                break;
            case 'n':
                this->expected = N_VALUE;
                break;
            case 'v':
                if (key.length() == 2) {
                    switch (KEY1) {
                        case 'b':
                            this->expected = VB_VALUE;
                            break;
                        case 'd':
                            this->expected = VD_VALUE;
                            break;
                        case 's':
                            this->expected = VS_VALUE;
                            break;
                    }
                } else if (key.length() == 1) {
                    this->expected = V_VALUE;
                }
                break;
        }
    }
}

void SenMLJsonListener::value(String value) {
    double dblVal;
    int intVal;
    bool boolVal;
    switch (this->expected) {
        case BV_VALUE:
            this->baseValue = atof(value.c_str());
            break;
        case BN_VALUE:
            this->setCurrentPack(value);
            break;
        case BU_VALUE:
            break;
        case N_VALUE:
            this->setCurrentRecord(value);
            break;
        case V_VALUE:
            dblVal = atof(value.c_str()) + this->baseValue;
            this->setValue(&dblVal, sizeof(double), SENML_TYPE_NR);
            break;
        case VB_VALUE:
            boolVal = strcmp(value.c_str(), "true") == 0;
            this->setValue(&boolVal, sizeof(bool), SENML_TYPE_BOOL);
            break;
        case VD_VALUE:
            this->setValue(value.c_str(), value.length(), SENML_TYPE_DATA);
            break;
        case VS_VALUE:
            this->setValue(value.c_str(), value.length(), SENML_TYPE_STRING);
            break;
        case I_VALUE:
            intVal = atoi(value.c_str());
            this->setCurrentRecord(intVal);
            break;
    }
}

void SenMLJsonListener::setValue(const void *value, int length, SenMLDataType dataType) {
    if (this->curRec) {
        this->curRec->actuate(value, length, dataType);
    } else {
        SenMLBasePack *pack = this->curPack;
        if (!pack) {
            pack = this->root;
        }
        if (pack && pack->isActuator()) {
            ((SenMLPack *)pack)->actuate(this->curPackName.c_str(), this->curRecName.c_str(), value, length, dataType);
        }
    }
}