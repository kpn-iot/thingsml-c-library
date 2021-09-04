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
 * cbor parsing and rendering header
 */


#ifndef SENMLCBOR
#define SENMLCBOR

#ifdef ARDUINO
    #include <Stream.h>
#endif
#include <stddef.h>
#include <stdint.h>

#define SENML_BVER_LABEL    -1
#define SENML_CBOR_BN_LABEL -2
#define SENML_CBOR_BT_LABEL -3
#define SENML_CBOR_BU_LABEL -4
#define SENML_CBOR_BV_LABEL -5
#define SENML_CBOR_BS_LABEL -16
#define SENML_CBOR_N_LABEL   0
#define SENML_CBOR_U_LABEL   1
#define SENML_CBOR_V_LABEL   2
#define SENML_CBOR_VS_LABEL  3
#define SENML_CBOR_VB_LABEL  4
#define SENML_CBOR_S_LABEL   5
#define SENML_CBOR_T_LABEL   6
#define SENML_CBOR_UT_LABEL  7
#define SENML_CBOR_VD_LABEL  8
#define THINGSML_CBOR_I_LABEL   23

#define CBOR_TYPE_MASK          0xE0    /* top 3 bits */
#define CBOR_INFO_MASK          0x1F    /* low 5 bits */

/* Major types (cf. section 2.1) */
/* Major type 0: Unsigned integers */
#define CBOR_UINT8_FOLLOWS      24      /* 0x18 */
#define CBOR_UINT16_FOLLOWS     25      /* 0x19 */
#define CBOR_UINT32_FOLLOWS     26      /* 0x1a */
#define CBOR_UINT64_FOLLOWS     27      /* 0x1b */

#define CBOR_BYTE_FOLLOWS       24      /* indicator that the next byte is part of this item */

/* Jump Table for Initial Byte (cf. table 5) */
#define CBOR_UINT       0x00            /* type 0 */
#define CBOR_NEGINT     0x20            /* type 1 */
#define CBOR_BYTES      0x40            /* type 2 */
#define CBOR_TEXT       0x60            /* type 3 */
#define CBOR_ARRAY      0x80            /* type 4 */
#define CBOR_MAP        0xA0            /* type 5 */
#define CBOR_TAG        0xC0            /* type 6 */
#define CBOR_7          0xE0            /* type 7 (float and other types) */

#define CBOR_VAR_FOLLOWS        31      /* 0x1f */

/* Major type 6: Semantic tagging */
#define CBOR_DATETIME_STRING_FOLLOWS        0
#define CBOR_DATETIME_EPOCH_FOLLOWS         1

/* Major type 7: Float and other types */
#define CBOR_FALSE      (CBOR_7 | 20)
#define CBOR_TRUE       (CBOR_7 | 21)
#define CBOR_NULL       (CBOR_7 | 22)
#define CBOR_UNDEFINED  (CBOR_7 | 23)
/* CBOR_BYTE_FOLLOWS == 24 */
#define CBOR_FLOAT16    (CBOR_7 | 25)
#define CBOR_FLOAT32    (CBOR_7 | 26)
#define CBOR_FLOAT64    (CBOR_7 | 27)
#define CBOR_BREAK      (CBOR_7 | 31)

#define CBOR_TYPE (peekChar() & CBOR_TYPE_MASK)



bool is_lossless_to_half_float(double value);

bool is_lossless_to_float(double value);

/**
 * @brief Serialize array of length @p array_length
 *
 * Basic usage:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.c}
 * cbor_serialize_array(2); // array of length 2 follows
 * cbor_serialize_int(1)); // write item 1
 * cbor_serialize_int(2)); // write item 2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @note You have to make sure to serialize the correct amount of items.
 * If you exceed the length @p array_length, items will just be appened as normal
 *
 * @param[in]  array_length Length of the array of items which follows
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_array(size_t array_length);


/**
 * @brief Serialize map of length @p map_length
 *
 * Basic usage:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.c}
 * cbor_serialize_map(2); // map of length 2 follows
 * cbor_serialize_int(1)); // write key 1
 * cbor_serialize_byte_string("1")); // write value 1
 * cbor_serialize_int(2)); // write key 2
 * cbor_serialize_byte_string("2")); // write value 2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @param map_length Length of the map of items which follows
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_map(size_t map_length);


/**
 * @brief Serializes an integer
 *
 * @param[in] val       The integer to serialize
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_int(int val);


/**
 * @brief Serializes a unicode string.
 *
 *                      string
 * @param[out] val      The zero-terminated unicode string to serialize.
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_unicode_string(const char *val);


/**
 * @brief Serializes a double precision floating point value
 *
 * @param[in] val       The double to serialize
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_double(double val);


/**
 * @brief Serializes a single precision floating point value
 *
 * @param[in] val       The float to serialize
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_float(float val);



/**
 * @brief Serializes a half precision floating point value
 *
 * @param[in] val       The float to serialize
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_half_float(float val);

/**
 * @brief Serializes a boolean value
 *
 * @param[in] val       The boolean value to serialize
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_bool(bool val);

/**
 * @brief Serializes a byte string
 *
 * @param[in] val       The byte string start pointer
 * @param[in] length    The length of the string
 *
 * @return Number of bytes written to stream @p stream
 */
size_t cbor_serialize_byte_string(const char *val, int length);


//read integer
size_t decode_int(uint64_t *val);

/**
 * @brief Deserialize signed 64 bit values from stream to @p val
 *
 * @param[out] val   Pointer to destination array
 *
 * @return Number of bytes read from @p stream
 */
size_t cbor_deserialize_int64_t(int64_t *val);

/**
 * @brief Deserialize unsigned 64 bit values from @p stream to @p val

 * @param[out] val   Pointer to destination array
 *
 * @return Number of bytes read from @p stream
 */
size_t cbor_deserialize_uint64_t(uint64_t *val);


/**
 *  @brief check that the char at the current position is a break char
 * 
 * @return 1 if all ok, 0 if no break char was found.
 */
size_t cbor_at_break();

size_t cbor_deserialize_float_half(float *val);
size_t cbor_deserialize_float(float *val);
size_t cbor_deserialize_double(double *val);

#endif // SENMLCBOR







