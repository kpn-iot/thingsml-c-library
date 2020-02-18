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
 * tests to check the correct functioning of our http helper.
 * 
 */
#include "../src/thingsml_http.h"
#include <string.h>
#include <iostream>
#include "gtest/gtest.h" 

TEST(thingsml_http, single_device_json) {
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

    char output[500] = {0};

    int len = ThingsML::httpPost(output, 500, "MYKEY", "1.1.1.1", "/bla", doc);

    EXPECT_EQ(309, len);
    EXPECT_EQ(len, strlen(output));
    EXPECT_STREQ("POST /bla HTTP/1.1\nHost: 1.1.1.1\nContent-Type: application/json\nThings-Message-Token: ca385fee74e177be286d3b2bc5825fb1189ecb188fb47edb8ad5bbdd5e7836f0\nContent-Length: 137\n\n[{\"bn\":\"device-name\",\"i_\":-23,\"vd\":\"aG9paG9pAA\"},{\"i_\":-19,\"vb\":true},{\"i_\":-18,\"v\":1.500000},{\"i_\":-24,\"v\":10},{\"i_\":-15,\"vs\":\"haihai\"}]", output);
}

TEST(senml_http, single_device_json)
{
    SenMLPack device("device-name");

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

    device.add(binrec);
    device.add(boolrec);
    device.add(floatrec);
    device.add(intrec);
    device.add(strrec);

    char output[500] = {0};
    int len = ThingsML::httpPost(output, 500, "MYKEY2", "1.1.1.1", "/bla", device);

    EXPECT_EQ(338, len);
    EXPECT_EQ(len, strlen(output));
    EXPECT_STREQ("POST /bla HTTP/1.1\nHost: 1.1.1.1\nContent-Type: application/json\nThings-Message-Token: 5cf81ec14696642db3faf701f281a89a025987738ed3edec4956b45f66680c7b\nContent-Length: 166\n\n[{\"bn\":\"device-name\",\"n\":\"humidity\",\"vd\":\"aG9paG9pAA\"},{\"n\":\"power\",\"vb\":true},{\"n\":\"pressure\",\"v\":1.500000},{\"n\":\"temperature\",\"v\":10},{\"n\":\"breadth\",\"vs\":\"haihai\"}]", output);
}


// TEST(thingsml_http, single_device_cbor) {
//     SenMLPack doc("device-name");

//     SenMLBinaryRecord binrec(THINGSML_HUMIDITY);
//     unsigned char data[] = "hoihoi";
//     binrec.set(data, 7);

//     SenMLBoolRecord boolrec(THINGSML_POWER);
//     boolrec.set(true);

//     SenMLDoubleRecord floatrec(THINGSML_PRESSURE);
//     floatrec.set(1.5);

//     SenMLIntRecord intrec(THINGSML_TEMPERATURE);
//     intrec.set(10);

//     SenMLStringRecord strrec(THINGSML_BREADTH);
//     const char str[] = "haihai";
//     strrec.set(str);


//     doc.add(binrec);
//     doc.add(boolrec);
//     doc.add(floatrec);
//     doc.add(intrec);
//     doc.add(strrec);

//     char output[500] = {0};

//     int len = ThingsML::httpPostCbor(output, 500, "MYKEY", "1.1.1.1", "/bla", doc);

//     EXPECT_EQ(249, len);
//     EXPECT_EQ(len, strlen(output));
//     EXPECT_STREQ("POST /bla HTTP/1.1\nHost: 1.1.1.1\nThings-Message-Token: bb2db1cb6965e669d9031f9414a015e5ba7a612f197d56e80019154e61434b0d\nContent-Length: 108\n\n85A3216B6465766963652D6E616D6517360847686F69686F6900A2173204F5A2173102F93E00A21737020AA2172E0366686169686169", output);
// }

// TEST(senml_http, single_device_cbor)
// {
//     SenMLPack device("device-name");

//     SenMLBinaryRecord binrec(SENML_NAME_HUMIDITY);
//     unsigned char data[] = "hoihoi";
//     binrec.set(data, 7);

//     SenMLBoolRecord boolrec(SENML_NAME_POWER);
//     boolrec.set(true);

//     SenMLDoubleRecord floatrec(SENML_NAME_PRESSURE);
//     floatrec.set(1.5);

//     SenMLIntRecord intrec(SENML_NAME_TEMPERATURE);
//     intrec.set(10);

//     SenMLStringRecord strrec(SENML_NAME_BREADTH);
//     const char str[] = "haihai";
//     strrec.set(str);

//     device.add(binrec);
//     device.add(boolrec);
//     device.add(floatrec);
//     device.add(intrec);
//     device.add(strrec);

//     char output[500] = {0};
//     int len = ThingsML::httpPostCbor(output, 500, "MYKEY2", "1.1.1.1", "/bla", device);

//     EXPECT_EQ(327, len);
//     EXPECT_EQ(len, strlen(output));
//     EXPECT_STREQ("POST /bla HTTP/1.1\nHost: 1.1.1.1\nThings-Message-Token: dd0b5745545408d129c6ec556e51c4d5c4abf5fbac1c47f343a67eabf1637a09\nContent-Length: 186\n\n85A3216B6465766963652D6E616D65006868756D69646974790847686F69686F6900A20065706F77657204F5A20068707265737375726502F93E00A2006B74656D7065726174757265020AA20067627265616474680366686169686169", output);
// }