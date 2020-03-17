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
 * tests to check the correct functioning of our senml/thingsml decoder.
 * 
 */
#include "../src/thingsml.h"
#include <string.h>
#include <iostream>
#include "gtest/gtest.h" 

TEST(senml, decode_simple_json) {
    SenMLPack device("Device name");
    SenMLDoubleActuator temperature("temperature");
    device.add(temperature);

    device.fromJson("{\"n\": \"temperature\",\"v\": 10.5}");

    EXPECT_EQ(10.5, temperature.get());
}

TEST(thingsml, decode_simple_json) {
    SenMLPack device("Device name");
    SenMLDoubleActuator temperature(THINGSML_TEMPERATURE);
    device.add(temperature);

    device.fromJson("{\"i_\": -24,\"v\": 10.5}");

    EXPECT_EQ(10.5, temperature.get());
}

TEST(thingsml, decode_extended_json) {
    SenMLPack device("device-name");

    SenMLBinaryActuator binrec(THINGSML_HUMIDITY);
    SenMLBoolActuator boolrec(THINGSML_POWER);
    SenMLDoubleActuator doublerec(THINGSML_PRESSURE);
    SenMLIntActuator intrec(THINGSML_TEMPERATURE);
    SenMLStringActuator strrec(THINGSML_BREADTH);


    device.add(binrec);
    device.add(boolrec);
    device.add(doublerec);
    device.add(intrec);
    device.add(strrec);

    EXPECT_EQ(false, boolrec.get());

    device.fromJson("[{\"i_\": -24,\"v\": 10}, {\"i_\": -18,\"v\": 10.5}, {\"i_\": -19,\"vb\": true}, {\"i_\": -15,\"vs\": \"true\"}, {\"i_\": -23,\"vd\":\"aG9paG9pAA\"}]");

    EXPECT_EQ(10, intrec.get());
    EXPECT_EQ(10.5, doublerec.get());
    EXPECT_EQ(true, boolrec.get());
    EXPECT_STREQ("true", strrec.get());
    EXPECT_STREQ("hoihoi", (char *) binrec.get());

    device.fromJson("[{\"i_\": -24,\"v\": 11}, {\"i_\": -18,\"v\": 12.5}, {\"i_\": -19,\"vb\": false}, {\"i_\": -15,\"vs\": \"false\"}, {\"i_\": -23,\"vd\":\"aGFp\"}]");
    EXPECT_EQ(11, intrec.get());
    EXPECT_EQ(12.5, doublerec.get());
    EXPECT_EQ(false, boolrec.get());
    EXPECT_STREQ("false", strrec.get());
    EXPECT_STREQ("hai", (char *) binrec.get());
}


TEST(thingsml, decode_simple_cbor) {
    SenMLPack device("Device name");
    SenMLDoubleActuator temperature(THINGSML_TEMPERATURE);
    device.add(temperature);

    device.fromCbor("81A2173702F94940", 16, SENML_HEX);

    EXPECT_EQ(10.5, temperature.get());
}

TEST(thingsml, decode_extended_cbor) {
    SenMLPack device("device-name");

    SenMLBinaryActuator binrec(THINGSML_HUMIDITY);
    SenMLBoolActuator boolrec(THINGSML_POWER);
    SenMLDoubleActuator doublerec(THINGSML_PRESSURE);
    SenMLIntActuator intrec(THINGSML_TEMPERATURE);
    SenMLStringActuator strrec(THINGSML_BREADTH);


    device.add(binrec);
    device.add(boolrec);
    device.add(doublerec);
    device.add(intrec);
    device.add(strrec);

    EXPECT_EQ(false, boolrec.get());

    device.fromCbor("85A3216B6465766963652D6E616D6517360847686F69686F6900A2173204F5A2173102F94940A21737020AA2172E036474727565", 108, SENML_HEX);

    EXPECT_EQ(10, intrec.get());
    EXPECT_EQ(10.5, doublerec.get());
    EXPECT_EQ(true, boolrec.get());
    EXPECT_STREQ("true", strrec.get());
    EXPECT_STREQ("hoihoi", (char *) binrec.get());
}