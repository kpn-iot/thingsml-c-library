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
 * felper functions
 */

#include "senml_helpers.h"
#include <string.h>

#ifdef ESP32
#include <Arduino.h> //needed for sprintf
#endif

#ifdef ARDUINO
#include "WString.h"
#endif

// global reference to the stream and stream configuration. This should save us memory
// by not having to pass these values continuously on the stack.
StreamContext *_streamCtx = NULL;
static const char hexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


int printByte(uint8_t value) {
    if (_streamCtx->dataAsBlob) {
        if (_streamCtx->format == SENML_RAW) {
            // if we reached the end of the buffer, stop rendering otherwise we overwrite some other mem which is
            // not good.
            if (_streamCtx->data.blob.curPos >= _streamCtx->data.blob.length) {
                return 0;
            }
            _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = value;
        } else if (_streamCtx->format == SENML_HEX) {
            // if we reached the end of the buffer, stop rendering otherwise we overwrite some other mem which is
            // not good.
            if ((_streamCtx->data.blob.curPos + 1) >= _streamCtx->data.blob.length) {
                return 0;
            }
            _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = hexTable[(value >> 4) & 0x0F];
            _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = hexTable[(value) & 0x0F];
        }
    } else {
        if (_streamCtx->format == SENML_RAW) {
#ifdef __MBED__
            _streamCtx->data.stream->putc(value);
#else
            _streamCtx->data.stream->print(value);
#endif
        } else if (_streamCtx->format == SENML_HEX) {
#ifdef __MBED__
            _streamCtx->data.stream->putc(hexTable[value >> 4]);
            _streamCtx->data.stream->putc(hexTable[value & 0x0F]);
#else
            _streamCtx->data.stream->print(hexTable[value >> 4]);
            _streamCtx->data.stream->print(hexTable[value & 0x0F]);
#endif
        }
    }
    return 1;
}

int printInt(int i) {
#if defined(ARDUINO)
    String temp(i);
    return printText(temp.c_str(), temp.length());
#else
    string str = to_string(i);
    return printText(str.c_str(), str.length());
#endif
}

int printDouble(double f, unsigned int digits) {
#if defined(ARDUINO)
    String temp(f, digits);
#else 
    string temp = to_string(f);
#endif
#if defined(ARDUINO) || defined(__MBED__)
    const char * s = temp.c_str();
    int length = temp.length();

    for (int i = length - 1; i > 0; i--) { // remove unwanted trailing 0
        if (s[i] != '0') {
            length = i;
            // if we end on something like x.  then add a last 0, so that it becomes x.0
            if (s[i] == '.') {
                length++;
            }
            break;
        }
    }
    return printText(s, length + 1);
#else
    return printText(temp.c_str(), temp.length());
#endif
}

int printBinaryAsBase64(const unsigned char *data, unsigned int length) {
#ifdef ESP32
    String encoded = base64::encode((uint8_t *)data, length);
    return printText(encoded.c_str(), encoded.length());
#else
    int encodedLen = base64_enc_len(length);
    char *encoded = (char *)malloc(encodedLen);

    // note input is consumed in this step: it will be empty afterwards
    base64_encode(encoded, (char *)data, length);
    printText(encoded, encodedLen);
    free(encoded);
    return encodedLen;
#endif
}

int printUnit(SenMLUnit unit) {
    if (unit == SENML_UNIT_NONE) {
        return 0;
    }
#ifdef ARDUINO
    strcpy_P(pgmBuff, (char *)pgm_read_word(&(senml_units_names[unit])));
    return printText(pgmBuff, strlen(pgmBuff));
#else
    return printText(senml_units_names[unit], strlen(senml_units_names[unit]));
#endif
}

int printText(const char *const value, int length) {
    // const unsigned char * val = (unsigned char*) value;
    if (_streamCtx->dataAsBlob) {
        if (_streamCtx->format == SENML_RAW) {
            for (int i = 0; i < length; i++) {
                // if we reached the end of the buffer, stop rendering otherwise we overwrite some other mem which is
                // not good.
                if (_streamCtx->data.blob.curPos >= _streamCtx->data.blob.length) {
                    return i;
                }
                _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = value[i];
            }
        } else if (_streamCtx->format == SENML_HEX) {
            for (int i = 0; i < length; i++) {
                // if we reached the end of the buffer, stop rendering otherwise we overwrite some other mem which is
                // not good.
                if ((_streamCtx->data.blob.curPos + 1) >= _streamCtx->data.blob.length) {
                    return i;
                }
                _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = hexTable[(value[i] >> 4) & 0x0F];
                _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = hexTable[(value[i]) & 0x0F];
            }
        }
    } else {
        if (_streamCtx->format == SENML_RAW) {
#ifdef __MBED__
            for (int i = 0; i < length; i++)
                _streamCtx->data.stream->putc(value[i]);
#else
            _streamCtx->data.stream->write(value, length);
#endif
        } else if (_streamCtx->format == SENML_HEX) {
            for (int i = 0; i < length; i++) {
#ifdef __MBED__
                _streamCtx->data.stream->putc(hexTable[value[i] >> 4]);
                _streamCtx->data.stream->putc(hexTable[value[i] & 0x0F]);
#else
                _streamCtx->data.stream->print(hexTable[value[i] >> 4]);
                _streamCtx->data.stream->print(hexTable[value[i] & 0x0F]);
#endif
            }
        }
    }
    return length;
}

#ifdef ARDUINO
int printText(const __FlashStringHelper *input, int length) {
    PGM_P value = reinterpret_cast<PGM_P>(input);
    if (_streamCtx->dataAsBlob) {
        if (_streamCtx->format == SENML_RAW) {
            for (int i = 0; i < length; i++) {
                // if we reached the end of the buffer, stop rendering otherwise we overwrite some other mem which is
                // not good.
                if (_streamCtx->data.blob.curPos >= _streamCtx->data.blob.length) {
                    return i;
                }
                char val = pgm_read_byte_near(value + i);
                _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = val;
            }
        } else if (_streamCtx->format == SENML_HEX) {
            for (int i = 0; i < length; i++) {
                // if we reached the end of the buffer, stop rendering otherwise we overwrite some other mem which is
                // not good.
                if ((_streamCtx->data.blob.curPos + 1) >= _streamCtx->data.blob.length) {
                    return i;
                }
                char val = pgm_read_byte_near(value + i);
                _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = hexTable[(val >> 4) & 0x0F];
                _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++] = hexTable[(val)&0x0F];
            }
        }
    } else {
        if (_streamCtx->format == SENML_RAW) {
            for (int i = 0; i < length; i++) {
                char val = pgm_read_byte_near(value + i);
                _streamCtx->data.stream->print(val);
            }
        } else if (_streamCtx->format == SENML_HEX) {
            for (int i = 0; i < length; i++) {
                char val = pgm_read_byte_near(value + i);
                _streamCtx->data.stream->print(hexTable[val >> 4]);
                _streamCtx->data.stream->print(hexTable[val & 0x0F]);
            }
        }
    }
    return length;
}
#endif

bool canPrint(int length) {
    if (_streamCtx->dataAsBlob) {
        return (_streamCtx->data.blob.curPos + length) < _streamCtx->data.blob.length;
    }
    return true;
}


static bool peeked = false; // if we peek the stream in HEX format, we actually need to read 2 bytes, so the peek
                            // doesn't work, but we need to read the actual value.
static int peekVal = 0;     // these values are removed by the compiler if no cbor is used.

int readChar() {
    if (peeked == true) {
        peeked = false;
        return peekVal;
    }
    int res;
    if (_streamCtx->dataAsBlob) {
        if (_streamCtx->data.blob.curPos <
            _streamCtx->data.blob.length) // peekchar has to return -1 if there is no more data, so check for this.
            res = _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++];
        else
            return -1;
        if (_streamCtx->format == SENML_HEX) {
            int resB = _streamCtx->data.blob.data[_streamCtx->data.blob.curPos++];
            res = (res > '9') ? (res & ~0x20) - 'A' + 10 : (res - '0');
            resB = (resB > '9') ? (resB & ~0x20) - 'A' + 10 : (resB - '0');
            res = (res * 16) + resB;
        }
    } else { // data is comming from the stream, this already returns -1 if there is no data.
        if (_streamCtx->format == SENML_RAW) {
#ifdef __MBED__
            res = _streamCtx->data.stream->getc();
#else
            res = _streamCtx->data.stream->available()
                      ? _streamCtx->data.stream->read()
                      : -1; // arduino stream, check if something is available, if not, we can't read anymore.
#endif
        } else {
            int resB;
#ifdef __MBED__
            res = _streamCtx->data.stream->getc();
            resB = _streamCtx->data.stream->getc();
#else

            if (_streamCtx->data.stream->available()) {
                res = _streamCtx->data.stream->read();
            } else {
                return -1;
            }
            if (_streamCtx->data.stream->available()) {
                resB = _streamCtx->data.stream->read();
            } else {
                return -1;
            }
#endif
            res = (res > '9') ? (res & ~0x20) - 'A' + 10 : (res - '0');
            resB = (resB > '9') ? (resB & ~0x20) - 'A' + 10 : (resB - '0');
            res = (res * 16) + resB;
        }
    }
    return res;
};

int peekChar() {
    // if already peeked, return the currently buffered value.
    if (peeked == true) {
        return peekVal;
    }
    peekVal = readChar();
    // if no more data, don't try to buffer it, some new data might arrive later on
    if (peekVal != -1) {
        peeked = true;
    }
    return peekVal;
};

void flush() {
    peeked = false;
};
