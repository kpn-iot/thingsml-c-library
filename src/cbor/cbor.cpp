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
 * cbor parsing and rendering
 */


#include "cbor.h"
#include "../senml/senml_helpers.h"
#include <string.h>


#ifndef htons
#define htons(x) ( ((x)<<8 & 0xFF00) | ((x)>>8 & 0x00FF) )
#endif // !htons

#ifndef  htonl
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#endif // ! htonl

#ifndef  htonll
#define htonll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))
#endif // ! htonll


#ifndef  ntohl
#define ntohl(x) ( ((x) & 0xFF000000UL) >> 24 | \
                   ((x) & 0x00FF0000UL) >> 8 | \
                   ((x) & 0x0000FF00UL) << 8 | \
                   ((x) & 0x000000FFUL) << 24 )
#endif // ! ntohl




/**
 * Convert float @p x to host format
 */
inline float binary_to_float(uint32_t x)
{
    union u {
        float f;
        uint32_t i;
    };
    u data;
    data.i = ntohl(x);
    return data.f;
}

inline uint32_t float_to_binary(float x) {
    union u {
        float f;
        uint32_t i;
    };
    u data;
    data.f = x;
    return htonl(data.i);
}

/**
 * Convert double @p x to network format
 */
static uint64_t double_to_binary(double x)
{
    union u {
        double d;
        uint64_t i;
    };
    u data;
    data.d = x;
    return htonll(data.i);
}

/**
 * Convert double @p x to host format
 */
inline double binary_to_double(uint64_t x)
{
    union u {
        double d;
        uint64_t i;
    };
    u data;
    data.i = htonll(x);
    return data.d;
}

inline uint16_t half_float_to_binary(float val) {
    union u {
        uint32_t ui;
        float f;
    };
    u d;
    d.f = val;
    /*
        Converting 1.5 example
        f32
        0 (sign)
        01111111 (exponent) 
        1000 0000 0000 0000 0000 000 (fraction)
        f16
        0
        01111
        1000 0000 00
    */
    uint32_t t1 = d.ui & 0x007FFFFF; // Fraction
    uint32_t t2 = d.ui & 0x80000000; // Sign bit
    uint32_t t3 = d.ui & 0x7F800000; // Exponent

    t1 >>= 13; // Scale fraction down (this might drop some precision)
    t2 >>= 16; // Move sign bit into position
    
    if (t3 != 0) {          // Preserve special numbers
        t3 >>= 13;          // Move into position
        t3 -= (112) << 10;  // Adjust for bias difference (-127 + 15)
        t3 &= 0x00007C00;   // Clamp value into place
    }

    t1 |= t2;
    t1 |= t3;
    return (uint16_t) (t1 & 0xFFFF);
}

inline float binary_to_half_float(uint16_t val) {
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;

    t1 = val & 0x7FFF; // Fraction
    t2 = val & 0x8000; // Sign bit
    t3 = val & 0x7C00; // Exponent
    
    t1 <<= 13;          // Align mantissa on MSB
    t2 <<= 16;          // Shift sign bit into position

    t1 += 0x38000000;   // Adjust bias

    t1 = (t3 == 0 ? 0 : t1); // Denormals-as-zero
    t1 |= t2;           // Re-insert sign bit

    union u {
        uint32_t ui;
        float f;
    };
    u d;
    d.ui = t1;
    return d.f;
}

inline float ntohd_avr(uint64_t x)
{
    union u {
        unsigned char dn[8];
        uint64_t i;
    };
    u data;
    data.i = htonll(x);

   union {
       float f;
       unsigned char b[4];
       uint32_t ui;
   } fn;    
   int expd = ((data.dn[7] & 127) << 4) + ((data.dn[6] & 240) >> 4);
   int expf = expd ? (expd - 1024) + 128 : 0;
   
   fn.b[3] = (data.dn[7] & 128) + (expf >> 1);
   fn.b[2] = ((expf & 1) << 7) + ((data.dn[6] & 15) << 3) + ((data.dn[5] & 0xe0) >> 5);
   fn.b[1] = ((data.dn[5] & 0x1f) << 3) + ((data.dn[4] & 0xe0) >> 5);
   fn.b[0] = ((data.dn[4] & 0x1f) << 3) + ((data.dn[3] & 0xe0) >> 5);
  return fn.f;
}


bool is_lossless_to_half_float(double value) {
    uint16_t fltInt16 = half_float_to_binary(value);
    float val = binary_to_half_float(fltInt16);
    return val == value;
}

bool is_lossless_to_float(double value) {
    return ((float)value)  == value;
}

/**
 * Source: CBOR RFC reference implementation
 */
double decode_float_half(unsigned char *halfp)
{
    uint16_t data = (halfp[0] << 8) | halfp[1];
    return binary_to_half_float(data);
}

/**
 * Return additional info field value for input value @p val
 *
 * @return Byte with the additional info bits set
 */
static unsigned char uint_additional_info(uint64_t val)
{
    if (val < CBOR_UINT8_FOLLOWS) {
        return val;
    }
    else if (val <= 0xff) {
        return CBOR_UINT8_FOLLOWS;
    }
    else if (val <= 0xffff) {
        return CBOR_UINT16_FOLLOWS;
    }
    else if (val <= 0xffffffffL) {
        return CBOR_UINT32_FOLLOWS;
    }

    return CBOR_UINT64_FOLLOWS;
}

/**
 * Return the number of bytes that would follow the additional info field @p additional_info
 *
 * @param additional_info Must be in the range [CBOR_UINT8_FOLLOWS, CBOR_UINT64_FOLLOWS]
 */
static unsigned char uint_bytes_follow(unsigned char additional_info)
{
    if (additional_info < CBOR_UINT8_FOLLOWS || additional_info > CBOR_UINT64_FOLLOWS) {
        return 0;
    }

    const unsigned char BYTES_FOLLOW[] = {1, 2, 4, 8};
    return BYTES_FOLLOW[additional_info - CBOR_UINT8_FOLLOWS];
}

static size_t encode_int(unsigned char major_type, uint64_t val)
{
    unsigned char additional_info = uint_additional_info(val);
    unsigned char bytes_follow = uint_bytes_follow(additional_info);
    unsigned char value = (major_type | additional_info);
    printText( (const char*)&value, 1);

    for (int i = bytes_follow - 1; i >= 0; --i) {
        value = (val >> (8 * i)) & 0xff;
        printText((const char*)&value, 1);
    }

    return bytes_follow + 1;
}

size_t decode_int(uint64_t *val)
{

    *val = 0; /* clear val first */

    unsigned char in = readChar();
    unsigned char bytes_follow = uint_bytes_follow(in & CBOR_INFO_MASK);

    switch (bytes_follow) {
        case 0:
            *val = (in & CBOR_INFO_MASK);
            break;

        case 1:
            *val = readChar();
            break;

        case 2:
            uint16_t data;
            readChars((unsigned char*)&data, 2);
            *val = htons(data);
            break;

        case 4:
            uint32_t data32;
            readChars((unsigned char*)&data32, 4);
            *val = htonl(data32);
            break;

        default:
            uint64_t data64;
            readChars((unsigned char*)&data64, 8);
            *val = htonll(data64);
            break;
    }

    return bytes_follow + 1;
}

static size_t encode_bytes(unsigned char major_type, const char *data, size_t length)
{
    uint_bytes_follow(uint_additional_info(length) + 1);
    size_t bytes_start = encode_int(major_type, (uint64_t) length);

    if (!bytes_start) {
        return 0;
    }

    printText(data, length);
    return (bytes_start + length);
}

size_t cbor_serialize_array(size_t array_length)
{
    /* serialize number of array items */
    return encode_int(CBOR_ARRAY, array_length);
}

size_t cbor_serialize_map(size_t map_length)
{
    /* serialize number of item key-value pairs */
    return encode_int(CBOR_MAP, map_length);
}

size_t cbor_serialize_int(int val)
{
    if (val >= 0) {
        /* Major type 0: an unsigned integer */
        return encode_int(CBOR_UINT, val);
    }
    else {
        /* Major type 1: an negative integer */
        return encode_int(CBOR_NEGINT, -1 - val);
    }
}


size_t cbor_serialize_unicode_string(const char * const val)
{
    return encode_bytes(CBOR_TEXT, val, strlen(val));
}

size_t cbor_serialize_half_float(float val) {
    uint16_t fltInt16 = half_float_to_binary(val);
    unsigned char value = CBOR_FLOAT16;
    printText((const char*)&value, 1);
    fltInt16 = (fltInt16 & 0xFF) << 8 | fltInt16 >> 8;
    printText( (const char*)&fltInt16, 2);
    return 3;
}

size_t cbor_serialize_float(float val) {
    unsigned char value = CBOR_FLOAT32;
    printText((const char*)&value, 1);
    uint32_t encoded_val = float_to_binary(val);
    printText( (const char*)&encoded_val, 4);
    return 5;
}

size_t cbor_serialize_double(double val)
{
    unsigned char value = CBOR_FLOAT64;
    printText((const char*)&value, 1);
    uint64_t encoded_val = double_to_binary(val);
    printText( (const char*)&encoded_val, 8);
    return 9;
}

size_t cbor_deserialize_float_half(float *val)
{
    if (CBOR_TYPE != CBOR_7 || !val) {
        return 0;
    }

    unsigned char dataType = readChar();
    if (dataType == CBOR_FLOAT16) {
        uint16_t data;
        readChars((unsigned char*)&data, 2);
        *val = (float)decode_float_half((unsigned char*)&data);
        return 3;
    }

    return 0;
}

size_t cbor_deserialize_float(float *val)
{
    if (CBOR_TYPE != CBOR_7 || !val) {
        return 0;
    }

    unsigned char dataType = readChar();

    if (dataType == CBOR_FLOAT32) {
        uint32_t data;
        readChars((unsigned char*)&data, 4);
        *val = binary_to_float(data);
        return 5;
    }
    return 0;
}


size_t cbor_deserialize_double(double *val)
{
    if (CBOR_TYPE != CBOR_7 || !val) {
        return 0;
    }

    unsigned char dataType = readChar();

    if (dataType == CBOR_FLOAT64) {
        uint64_t data;
        readChars((unsigned char*)&data, 8);
        if(sizeof(double) == 8)                                 //this is the default on most systems
        {
            *val = binary_to_double(data);
        }
        else if(sizeof(double) == 4)                            //8 bit processors such as avr don't support 8 byte floating point values, only 4 bytes, so need special conversion
        {
            *val = ntohd_avr(data);
        }
        return 9;
    }

    return 0;
}

size_t cbor_deserialize_int64_t(int64_t *val)
{
    unsigned char type = CBOR_TYPE;
    if ((type != CBOR_UINT && type != CBOR_NEGINT) || !val) {
        return 0;
    }

    uint64_t buf;
    size_t read_bytes = decode_int(&buf);

    if (type == CBOR_UINT) 
        *val = buf;                         /* resolve as CBOR_UINT */
    else 
        *val = -1 - buf;                    /* resolve as CBOR_NEGINT */
    return read_bytes;
}

size_t cbor_deserialize_uint64_t(uint64_t *val)
{
    if (CBOR_TYPE != CBOR_UINT || !val) {
        return 0;
    }
    return decode_int(val);
}

size_t cbor_serialize_bool(bool val)
{
    unsigned char value = (val ? CBOR_TRUE : CBOR_FALSE);
    printText((const char*)&value , 1);
    return 1;
}

size_t cbor_serialize_byte_string(const char *val, int length)
{
    return encode_bytes(CBOR_BYTES, val, length);
}








