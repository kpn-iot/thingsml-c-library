/*  _  __  ____    _   _
 * | |/ / |  _ \  | \ | |
 * | ' /  | |_) | |  \| |
 * | . \  |  __/  | |\  |
 * |_|\_\ |_|     |_| \_|
 *
 * (c) 2020 KPN
 * License: MIT License.
 * Author: Joseph Verburg
 * 
 * test for checking the correct functioning of the builtin cbor encoder/decoder
 * 
 */
#include <string.h>
#include <iostream>

#include "../src/cbor/cbor.h"
#include "../src/senml/senml_helpers.h"
#include "gtest/gtest.h"

TEST(cbor, serialize_majortype0) {
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    cbor_serialize_int(5);
    EXPECT_STREQ("05", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(30);
    EXPECT_STREQ("181E", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(300);
    EXPECT_STREQ("19012C", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(300000);
    EXPECT_STREQ("1A000493E0", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(2147000000);
    EXPECT_STREQ("1A7FF89EC0", buf);
}

TEST(cbor, serialize_majortype1) {
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    cbor_serialize_int(-5);
    EXPECT_STREQ("24", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(-30);
    EXPECT_STREQ("381D", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(-300);
    EXPECT_STREQ("39012B", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(-300000);
    EXPECT_STREQ("3A000493DF", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_int(-2147000000);
    EXPECT_STREQ("3A7FF89EBF", buf);
}

TEST(cbor, serialize_majortype2) {
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    char str[] = "HOI";
    cbor_serialize_byte_string(str, 3);
    EXPECT_STREQ("43484F49", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_byte_string(str, 4); // include \0
    EXPECT_STREQ("44484F4900", buf);

    char longstr[] = "much longer such that the length does not fit in the normal space for length encoding";
    ctx.data.blob.curPos = 0;
    cbor_serialize_byte_string(longstr, 86);
    EXPECT_STREQ("58566D756368206C6F6E6765722073756368207468617420746865206C656E67746820646F6573206E6F742066697420696E20746865206E6F726D616C20737061636520666F72206C656E67746820656E636F64696E6700", buf);
}

TEST(cbor, serialize_majortype4) {
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    cbor_serialize_array(2);
    cbor_serialize_int(5);
    cbor_serialize_int(5);
    EXPECT_STREQ("820505", buf);

    ctx.data.blob.curPos = 0;
    cbor_serialize_array(25);
    for(int i = 0; i < 25; i++ ){
        cbor_serialize_int(5);
    }
    EXPECT_STREQ("981905050505050505050505050505050505050505050505050505", buf);
}

TEST(cbor, serialize_majortype5) {
    
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    cbor_serialize_map(2);
    cbor_serialize_int(0);
    cbor_serialize_int(5);
    cbor_serialize_int(1);
    cbor_serialize_int(5);
    EXPECT_STREQ("A200050105", buf);

    ctx.data.blob.curPos = 0;
    cbor_serialize_map(25);
    for (int i = 0; i < 25; i++) {
        cbor_serialize_int(i);
        cbor_serialize_int(5 * i);
    }
    EXPECT_STREQ("B81900000105020A030F041405181906181E07182308182809182D0A18320B18370C183C0D18410E18460F184B10185011185512185A13185F14186415186916186E17187318181878", buf);
}

TEST(cbor, serialize_majortype7halffloat) {
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    cbor_serialize_half_float(1.1);
    EXPECT_STREQ("F93C66", buf); // 1.099 due to truncation
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_half_float(1.5);
    EXPECT_STREQ("F93E00", buf); // 1.5 no truncation
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_half_float(20.4);
    EXPECT_STREQ("F94D19", buf); // 20.39 due to truncation

    ctx.data.blob.curPos = 0;
    cbor_serialize_half_float(300.1);
    EXPECT_STREQ("F95CB0", buf); // 300 due to truncation
}

TEST(cbor, serialize_majortype7float) {
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    cbor_serialize_float(1.1);
    EXPECT_STREQ("FA3F8CCCCD", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_float(300.1);
    EXPECT_STREQ("FA43960CCD", buf);
}

TEST(cbor, serialize_majortype7double) {
    StreamContext ctx;
    ctx.dataAsBlob = true;
    char buf[200] = {0};
    SenmlMemoryData memdata;
    memdata.data = buf;
    memdata.length = 200;
    memdata.curPos = 0;
    SenmlData data;
    data.blob = memdata;
    ctx.data = data;
    ctx.format = SENML_HEX;

    _streamCtx = &ctx;

    cbor_serialize_double(1.1);
    EXPECT_STREQ("FB3FF199999999999A", buf);
    
    ctx.data.blob.curPos = 0;
    cbor_serialize_double(300.1);
    EXPECT_STREQ("FB4072C1999999999A", buf);
}

TEST(cbor, is_lossless) {
    EXPECT_EQ(true, is_lossless_to_half_float(0.5));
    EXPECT_EQ(true, is_lossless_to_half_float(0.001953125));
    EXPECT_EQ(false, is_lossless_to_half_float(0.00000011920928955078125));
    EXPECT_EQ(false, is_lossless_to_half_float(300.1));
    // EXPECT_EQ(true, is_lossless_to_half_float(1.00048828125‬));

    EXPECT_EQ(true, is_lossless_to_float(0.5));
    EXPECT_EQ(true, is_lossless_to_float(0.001953125));
    EXPECT_EQ(true, is_lossless_to_float(0.00000011920928955078125));
    EXPECT_EQ(false, is_lossless_to_float(300.1));
}