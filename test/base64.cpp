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
 * tests to check the correct functioning of the builtin base64 encoder/decoder
 * 
 */
#include "../src/util/base64.h"
#include "gtest/gtest.h"

char to_b64_test(unsigned char d) {
  if (d < 26) {
    return 'A' + d;
  } else if (d < 52) {
    return 71 + d;
  } else if (d < 62) {
    return d - 4;
  } else if (d == 62) {
    return '+';
  } else if (d == 63) {
    return '/';
  }
  return 0;
}

TEST(base64, to_b64) {
    EXPECT_EQ('A', to_b64_test(0));
    EXPECT_EQ('Z', to_b64_test(25));
    EXPECT_EQ('a', to_b64_test(26));
    EXPECT_EQ('z', to_b64_test(51));
    EXPECT_EQ('0', to_b64_test(52));
    EXPECT_EQ('9', to_b64_test(61));
    EXPECT_EQ('+', to_b64_test(62));
    EXPECT_EQ('/', to_b64_test(63));
}

TEST(base64, encode_FC) {
    unsigned char input[] = {0xFC};
    char output[4] = {0};
    base64_encode(output, (char *) input, 1);

    EXPECT_STREQ("/A",output);
}


TEST(base64, encode_FF) {
    unsigned char input[] = {0xFF};
    char output[4] = {0};
    base64_encode(output, (char *) input, 1);

    EXPECT_STREQ("/w",output);
}

TEST(base64, encode_FFFF) {
    unsigned char input[] = {0xFF, 0xFF};
    char output[4] = {0};
    base64_encode(output, (char *) input, 2);

    EXPECT_STREQ("//8",output);
}

TEST(base64, decode_FC) {
    char input[] = "/A";
    char output[4] = {0};
    unsigned char expected_output[] = {0xFC, 0};
    base64_decode(output, (char *) input, 2);

    EXPECT_STREQ((char *)expected_output, output);
}


TEST(base64, decode_FF) {
    char input[] = "/w";
    char output[4] = {0};
    unsigned char expected_output[] = {0xFF, 0};
    base64_decode(output, (char *) input, 2);

    EXPECT_STREQ((char *)expected_output, output);
}

TEST(base64, decode_FFFF) {
    char input[] = "//8";
    char output[4] = {0};
    unsigned char expected_output[] = {0xFF, 0xFF, 0};
    base64_decode(output, (char *) input, 3);

    EXPECT_STREQ((char *)expected_output, output);
}