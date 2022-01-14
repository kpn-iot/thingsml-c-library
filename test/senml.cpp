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
 * tests to check the correct functioning of our senml/thingsml encoder in json and cbor.
 * 
 */
#include "../src/thingsml.h"
#include <string.h>
#include <iostream>
#include "gtest/gtest.h" 

TEST(thingsml, single_device_cbor) {
    SenMLPack doc("device-name");

    SenMLBinaryRecord binrec(THINGSML_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(THINGSML_POWER);
    boolrec.set(true);

    SenMLDoubleRecord doublerec(THINGSML_PRESSURE);
    doublerec.set(1.5);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(THINGSML_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);


    doc.add(binrec);
    doc.add(boolrec);
    doc.add(doublerec);
    doc.add(intrec);
    doc.add(strrec);
    char output[200] = {0};

    int len = doc.toCbor(output, 200, SENML_HEX);
    EXPECT_EQ(54, len);
    EXPECT_STREQ("85A3216B6465766963652D6E616D6517360847686F69686F6900A2173204F5A2173102F93E00A21737020AA2172E0366686169686169", output);
}


TEST(thingsml, single_device_json) {
    SenMLPack doc("device-name");

    SenMLBinaryRecord binrec(THINGSML_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(THINGSML_POWER);
    boolrec.set(true);

    SenMLDoubleRecord floatrec(THINGSML_PRESSURE);
    floatrec.set(1.5);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(THINGSML_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);


    doc.add(binrec);
    doc.add(boolrec);
    doc.add(floatrec);
    doc.add(intrec);
    doc.add(strrec);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(137, len);
    EXPECT_STREQ("[{\"bn\":\"device-name\",\"i_\":-23,\"vd\":\"aG9paG9pAA\"},{\"i_\":-19,\"vb\":true},{\"i_\":-18,\"v\":1.500000},{\"i_\":-24,\"v\":10},{\"i_\":-15,\"vs\":\"haihai\"}]", output);
}

TEST(senml, single_device_cbor)
{
    SenMLPack doc("device-name");

    SenMLBinaryRecord binrec(SENML_NAME_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(SENML_NAME_POWER);
    boolrec.set(true);

    SenMLDoubleRecord floatrec(SENML_NAME_PRESSURE);
    floatrec.set(1.5);

    SenMLIntRecord intrec(SENML_NAME_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(SENML_NAME_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);


    doc.add(binrec);
    doc.add(boolrec);
    doc.add(floatrec);
    doc.add(intrec);
    doc.add(strrec);
    char output[200] = {0};

    int len = doc.toCbor(output, 200, SENML_HEX);
    EXPECT_EQ(93, len);
    EXPECT_STREQ("85A3216B6465766963652D6E616D65006868756D69646974790847686F69686F6900A20065706F77657204F5A20068707265737375726502F93E00A2006B74656D7065726174757265020AA20067627265616474680366686169686169", output);
}

TEST(senml, single_device_json)
{
    SenMLPack doc("device-name");

    SenMLBinaryRecord binrec(SENML_NAME_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(SENML_NAME_POWER);
    boolrec.set(true);

    SenMLDoubleRecord floatrec(SENML_NAME_PRESSURE);
    floatrec.set(1.5);

    SenMLIntRecord intrec(SENML_NAME_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(SENML_NAME_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);


    doc.add(binrec);
    doc.add(boolrec);
    doc.add(floatrec);
    doc.add(intrec);
    doc.add(strrec);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(166, len);
    EXPECT_STREQ("[{\"bn\":\"device-name\",\"n\":\"humidity\",\"vd\":\"aG9paG9pAA\"},{\"n\":\"power\",\"vb\":true},{\"n\":\"pressure\",\"v\":1.500000},{\"n\":\"temperature\",\"v\":10},{\"n\":\"breadth\",\"vs\":\"haihai\"}]", output);
}


TEST(thingsml, single_double_case) {
    SenMLPack device("urn:dev:DEVEUI:0059AC00001B0095:");
    SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);

    device.add(temperature);

    temperature.set(1.5);

    char buf[500] = {0};
    int len = device.toCbor(buf, 500, SENML_HEX);
    EXPECT_EQ(43, len);
    EXPECT_STREQ("81A321782075726E3A6465763A4445564555493A303035394143303030303142303039353A173702F93E00", buf);
}


TEST(thingsml, empty_name) {
    SenMLPack device;
    SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);

    device.add(temperature);

    temperature.set(1.5);

    char buf[500] = {0};
    int len = device.toCbor(buf, 500, SENML_HEX);
    EXPECT_EQ(8, len);
    EXPECT_STREQ("81A2173702F93E00", buf);
}


TEST(senml, invalid_json_empty_name) {
    SenMLPack doc;

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    doc.add(intrec);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(19, len);
    EXPECT_STREQ("[{\"i_\":-24,\"v\":10}]", output);
}


TEST(senml, invalid_json_only_base_unit) {
    SenMLPack doc;
    
    doc.setBaseUnit(SENML_UNIT_DEGREES_CELSIUS);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    doc.add(intrec);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(30, len);
    EXPECT_STREQ("[{\"bu\":\"Cel\",\"i_\":-24,\"v\":10}]", output);
}


TEST(senml, invalid_json_only_base_time) {
    SenMLPack doc;

    doc.setBaseTime(1);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    doc.add(intrec);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(33, len);
    EXPECT_STREQ("[{\"bt\":1.000000,\"i_\":-24,\"v\":10}]", output);
}

TEST(senml, invalid_json_only_update_time) {
    SenMLPack doc;
    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);
    doc.add(intrec);
    intrec.setUpdateTime(10);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(34, len);
    EXPECT_STREQ("[{\"i_\":-24,\"ut\":10.000000,\"v\":10}]", output);

    doc.setBaseTime(10);
    intrec.setUpdateTime(20);

    len = doc.toJson(output, 200);
    EXPECT_EQ(49, len);
    EXPECT_STREQ("[{\"bt\":10.000000,\"i_\":-24,\"ut\":10.000000,\"v\":10}]", output);
}

TEST(senml, simple_pack_json) {
    SenMLSimplePack doc;

    SenMLBinaryRecord binrec(SENML_NAME_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(SENML_NAME_POWER);
    boolrec.set(true);

    SenMLDoubleRecord floatrec(SENML_NAME_PRESSURE);
    floatrec.set(1.5);

    SenMLIntRecord intrec(SENML_NAME_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(SENML_NAME_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);


    doc.add(binrec);
    doc.add(boolrec);
    doc.add(floatrec);
    doc.add(intrec);
    doc.add(strrec);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(147, len);
    EXPECT_STREQ("[{\"n\":\"humidity\",\"vd\":\"aG9paG9pAA\"},{\"n\":\"power\",\"vb\":true},{\"n\":\"pressure\",\"v\":1.500000},{\"n\":\"temperature\",\"v\":10},{\"n\":\"breadth\",\"vs\":\"haihai\"}]", output);

}


TEST(senml, simple_pack_cbor) {
    SenMLSimplePack doc;

    SenMLBinaryRecord binrec(SENML_NAME_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(SENML_NAME_POWER);
    boolrec.set(true);

    SenMLDoubleRecord floatrec(SENML_NAME_PRESSURE);
    floatrec.set(1.5);

    SenMLIntRecord intrec(SENML_NAME_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(SENML_NAME_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);

    doc.add(binrec);
    doc.add(boolrec);
    doc.add(floatrec);
    doc.add(intrec);
    doc.add(strrec);

    char output[200] = {0};
    int len = doc.toCbor(output, 200, SENML_HEX);
    EXPECT_EQ(80, len);
    EXPECT_STREQ("85A2006868756D69646974790847686F69686F6900A20065706F77657204F5A20068707265737375726502F93E00A2006B74656D7065726174757265020AA20067627265616474680366686169686169", output);
}

TEST(thingsml, simple_pack_json) {
    SenMLSimplePack doc;

    SenMLBinaryRecord binrec(THINGSML_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(THINGSML_POWER);
    boolrec.set(true);

    SenMLDoubleRecord floatrec(THINGSML_PRESSURE);
    floatrec.set(1.5);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(THINGSML_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);

    doc.add(binrec);
    doc.add(boolrec);
    doc.add(floatrec);
    doc.add(intrec);
    doc.add(strrec);

    char output[200] = {0};
    int len = doc.toJson(output, 200);
    EXPECT_EQ(118, len);
    EXPECT_STREQ("[{\"i_\":-23,\"vd\":\"aG9paG9pAA\"},{\"i_\":-19,\"vb\":true},{\"i_\":-18,\"v\":1.500000},{\"i_\":-24,\"v\":10},{\"i_\":-15,\"vs\":\"haihai\"}]", output);
}


TEST(thingsml, simple_pack_cbor) {
    SenMLSimplePack doc; 
    
    SenMLBinaryRecord binrec(THINGSML_HUMIDITY);
    unsigned char data[] = "hoihoi";
    binrec.set(data, 7);

    SenMLBoolRecord boolrec(THINGSML_POWER);
    boolrec.set(true);

    SenMLDoubleRecord floatrec(THINGSML_PRESSURE);
    floatrec.set(1.5);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    SenMLStringRecord strrec(THINGSML_BREADTH);
    const char str[] = "haihai";
    strrec.set(str);


    doc.add(binrec);
    doc.add(boolrec);
    doc.add(floatrec);
    doc.add(intrec);
    doc.add(strrec);

    char output[200] = {0};
    int len = doc.toCbor(output, 200, SENML_HEX);
    EXPECT_EQ(41, len);
    EXPECT_STREQ("85A217360847686F69686F6900A2173204F5A2173102F93E00A21737020AA2172E0366686169686169", output);
}

TEST(thingsml, single_device_cbor_extended) {
    SenMLPack doc;

    doc.setBaseTime(10);
    doc.setBaseUnit(SENML_UNIT_BIT);

    SenMLBinaryRecord binrec(THINGSML_HUMIDITY);
    unsigned char data[] = "hoihoi";
    doc.add(binrec);
    binrec.set(data, 7, 11);

    SenMLBoolRecord boolrec(THINGSML_POWER);
    doc.add(boolrec);
    boolrec.set(true, 12);

    SenMLDoubleRecord doublerec(THINGSML_PRESSURE);
    doc.add(doublerec);
    doublerec.set(1.3, 13);

    SenMLDoubleRecord doublerec2(THINGSML_ACCELERATION_X);
    doc.add(doublerec2);
    doublerec2.set(0.00000011920928955078125, 14);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    doc.add(intrec);
    intrec.set(10, 15.5);

    SenMLStringRecord strrec(THINGSML_BREADTH);
    doc.add(strrec);
    const char str[] = "haihai";
    strrec.set(str, 16.3);


    char output[200] = {0};

    int len = doc.toCbor(output, 200, SENML_HEX);
    EXPECT_EQ(95, len);
    EXPECT_STREQ("86A5236362697422F94900173606F93C000847686F69686F6900A3173206F9400004F5A3173106F9420002FB3FF4CCCCCCCCCCCDA3172206F9440002FA34000000A3173706F94580020AA3172E06FB40193333333333340366686169686169", output);
}


TEST(senml, proper_zero_terminate_json) {
    SenMLPack doc;

    doc.setBaseTime(1);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    SenMLIntRecord intrec2(THINGSML_TEMPERATURE);
    intrec2.set(15);

    doc.add(intrec);
    doc.add(intrec2);

    char output[200] = {0};
    doc.toJson(output, 200);

    doc.clear();
    doc.add(intrec);

    int len = doc.toJson(output, 200);
    EXPECT_EQ(33, len);
    EXPECT_STREQ("[{\"bt\":1.000000,\"i_\":-24,\"v\":10}]", output);
}

TEST(senml, proper_zero_terminate_cbor) {
    SenMLPack doc;

    doc.setBaseTime(1);

    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    intrec.set(10);

    SenMLIntRecord intrec2(THINGSML_TEMPERATURE);
    intrec2.set(15);

    doc.add(intrec);
    doc.add(intrec2);

    char output[200] = {0};
    doc.toCbor(output, 200, SENML_HEX);

    doc.clear();
    doc.add(intrec);

    int len = doc.toCbor(output, 200, SENML_HEX);
    EXPECT_EQ(10, len);
    EXPECT_STREQ("81A322F93C001737020A", output);
}

TEST(senml, count_pack) {
    SenMLPack doc;
    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    SenMLIntRecord intrec2(THINGSML_TEMPERATURE);

    doc.add(intrec);
    doc.add(intrec2);

    EXPECT_EQ(2, doc.getCount());
}


TEST(senml, count_pack_nested) {
    SenMLPack doc;
    SenMLPack doc2;
    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    SenMLIntRecord intrec2(THINGSML_TEMPERATURE);
    SenMLIntRecord intrec3(THINGSML_TEMPERATURE);
    SenMLIntRecord intrec4(THINGSML_TEMPERATURE);

    doc.add(intrec);
    doc.add(intrec2);
    doc.add(doc2);
    doc2.add(intrec3);
    doc2.add(intrec4);

    EXPECT_EQ(4, doc.getCount());
}

TEST(senml, count_pack_nested_empty) {
    SenMLPack doc;
    SenMLPack doc2;
    SenMLIntRecord intrec(THINGSML_TEMPERATURE);
    SenMLIntRecord intrec2(THINGSML_TEMPERATURE);

    doc.add(intrec);
    doc.add(intrec2);
    doc.add(doc2);

    EXPECT_EQ(3, doc.getCount());
}