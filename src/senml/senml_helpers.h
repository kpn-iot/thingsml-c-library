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
 * helper functions header
 */

#ifndef THINGSML_SENML_HELPERS
#define THINGSML_SENML_HELPERS

#include "defaults.h"
#include "senml_enums.h"
#include <math.h>

#ifndef SENML_MAX_DOUBLE_PRECISION
#define SENML_MAX_DOUBLE_PRECISION 4
#endif // !SENML_MAX_DOUBLE_PRECISION

/**
 * Internal helper struct that stores the base value to be used during rendering
 */
typedef union BaseData_t {
    uint64_t baseUint;
    int64_t baseInt;
    double baseDouble;
} BaseData;

/**
 * Internal helper struct that stores information needed during the rendering process
 */
typedef struct SenmlMemoryData_t {
    char *data;    // pointer to the buffer that constitutes the input/output buffer
    int curPos;    // current position in the buffer.
    int length;    // length of the memory buffer.
} SenmlMemoryData; // a record used when the input for the parser or output for the renderer is a data blob stored in
                   // memory

/**
 *  Internal helper struct that stores information needed during the rendering process
 */
typedef union SenmlData_t {
    Stream *stream;
    SenmlMemoryData blob;
} SenmlData; // choose between input/outpu from a stream object (ex: direct input from uart) or from buffered data.

/**
 * Internal helper struct that stores information needed during the rendering process
 */
typedef struct StreamContext_t {
    bool dataAsBlob; // when true, data is from a memory blob, otherwise it comes from/goes to a stream object (ex:
                     // direct from/to uart)
    SenmlData data; // the data source to be parsed or destination to render to. can be from a stream or from data
                    // buffered in memory
    SenMLStreamMethod format;
    BaseData baseValue; // filled in when records need to adjust while rendering data.
    BaseData baseSum;
    SenMLDataType baseDataType; // so we know which data type to use for baseValue and baseSum
} StreamContext;

/**
 * Internal data field used for the rendering process.
 */
extern StreamContext *_streamCtx;

/**
 * Helper function for the generation process
 * write a uint8_t as byte to the stream (_streamCtx).
 * @param value the value to print
 */
int printByte(uint8_t value);

/**
 * Helper function for the generation process
 * write a int as string to the stream (_streamCtx).
 * @param i the value to print
 */
int printInt(int i);

/**
 * Helper function for the generation process
 * write a double as string to the stream (_streamCtx).
 * @param f the value to print
 * @param digits the nr of digits that should be printed after the comma.
 */
int printDouble(double f, unsigned int digits);

/**
 * Helper function for the generation process
 * convert the data array to base64 string and write to stream (_streamCtx)
 * @param data pointer to the binary data blob that needs to be rendered as base64 to the stream.
 * @param length the length of the data blob
 */
int printBinaryAsBase64(const unsigned char *data, unsigned int length);

/**
 * Helper function for the generation process
 * convert the unit to string and write to stream (_streamCtx).
 * @param unit the value to write as text to the stream.
 */
int printUnit(SenMLUnit unit);

/**
 * Helper function for the generation process
 * Writes the specified text to the stream (_streamCtx).
 * @param value the value to write
 * @param len the length of the value.
 */
int printText(const char *value, int length);

/**
 * Helper function for the generation process
 * Determines if the underlying stream has enough space to write length bytes (_streamCtx).
 * 
 * always returns true for non buffer backed streams
 */
bool canPrint(int length);

#ifdef ARDUINO
int printText(const __FlashStringHelper *value, int length);
#else
#define F(str) str
#endif

/**
 * Helper function for the parsing process
 * read a character from the current data stream (_streamCtx)
 * takes into account that the stream might contain hex values
 */
int readChar();

/**
 * Helper function for the parsing process
 * peek a character from the current data stream.
 * takes into account that the stream might contain hex values.
 */
int peekChar();

/**
 * Helper function for the parsing process
 * flush and reset all input for the current data stream.
 */
void flush();

/**
 * Helper function for the parsing process
 * Reads the specified nr of characters from the stream (_streamCtx)
 * takes into account that the stream might contain hex values
 * @param buffer the buffer to store the values in.
 * @param len the nr of characters to read from the stream into the buffer.
 *        If the input contains hex values, the actual nr of characters read from the
 *        stream is double this value.
 */
inline void readChars(unsigned char *buffer, int len) {
    for (int i = 0; i < len; i++) {
        buffer[i] = readChar();
    }
}

/**
 * Helper function for the parsing process
 * Checks if there is data available on the stream.
 * Warning: THIS CAN BE PROBLEMATIC ON MBED SYSTEMS>
 */
inline bool charAvailable() {
    if (_streamCtx->dataAsBlob) {
        return _streamCtx->data.blob.curPos < _streamCtx->data.blob.length;
    } else {
#ifdef __MBED__
        return _streamCtx->data.stream->readable() != 0;
#else
        return _streamCtx->data.stream->available() != 0;
#endif
    }
}

#endif // THINGSML_SENML_HELPERS
