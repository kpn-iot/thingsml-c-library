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
 * tests to check the correct functioning of our builtin sha256 code.
 * 
 */
#include "../src/util/sha256.h"
#include "gtest/gtest.h"

TEST(sha256, small) {
    char hoi[] = "abc";
    char output[65] = {0};

    Sha256::hash(hoi, 3, output, 64);
    
    EXPECT_STREQ("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad", output);
}

TEST(sha256, block) {
    char hoi[] = "ba7816bf8f01cfea414140de5dae2aaa";
    char output[65] = {0};
    Sha256::hash(hoi, 32, output, 64);
    
    EXPECT_STREQ("4794396dae1ad3389086288471f4d6e099cfa83bb73ec735789fd94922484dcb", output);
}

TEST(sha256, two_blocks) {
    char hoi[] = "4794396dae1ad3389086288471f4d6e099cfa83bb73ec735789fd94922484dcb";
    char output[65] = {0};
    Sha256::hash(hoi, 64, output, 64);
    
    EXPECT_STREQ("08967e2cc2c68133ff06470a45ce244c2b46437aae9b4365b58fb16e0c910c22", output);
}

TEST(sha256, longdata) {
    char hoi[] = "4794396dae1ad39086288471f4d6e099cfa83bb73ec735789fd94922484dcb4794396dae1ad3389086288471f4d6e0973ec735789fd94922484dcb4794396dae1ad3389086288471f4d6";
    char output[65] = {0};
    Sha256::hash(hoi, 148, output, 64);
    
    EXPECT_STREQ("91a2d330e14fcd22ba4f8b3fe67f5f011fef565050fa56e2cc9493ae589166aa", output);
}