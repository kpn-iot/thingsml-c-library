// This file is part of cryptosuite.                                     //
//                                                                       //
// cryptosuite is free software: you can redistribute it and/or modify   //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation, either version 3 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// cryptosuite is distributed in the hope that it will be useful,        //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details.                          //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with cryptosuite.  If not, see <http://www.gnu.org/licenses/>.  //
//                                                                       //
#ifndef Sha256_h
#define Sha256_h

#include <inttypes.h>

#define HASH_LENGTH 32
#define BLOCK_LENGTH 64

class Sha256 {

    union Buffer {
        uint8_t b[BLOCK_LENGTH];
        uint32_t w[BLOCK_LENGTH / 4];
    };

    union State {
        uint8_t b[HASH_LENGTH];
        uint32_t w[HASH_LENGTH / 4];
    };

  public:
    static void hash(const char input[], int inputLength, char output[], int outputLength);
    static void hash(const uint8_t input[], int inputLength, uint8_t output[], int outputLength);

    void init();
    void initHmac(const uint8_t * key, int keyLength);

    void write(const uint8_t);
    void write(const uint8_t in[], int inLength);
    void write(const char in[], int inLength) {
        write((uint8_t *)in, inLength);
    };

    uint8_t *result();
    uint8_t *resultHmac();

    /**
     * If outlength > 32 output is in hex.
     * If outlength <= 32 output is in raw bytes.
     */
    void result(uint8_t out[], int outLength);
    void result(char out[], int outLength) {
        result((uint8_t *)out, outLength);
    };

    // Reset to initial state, but preserve key material.
    void reset();

  private:
    void hashBlock();
    void padBlock();
    void push(uint8_t data);

    uint32_t byteCount;

    uint8_t keyBuffer[BLOCK_LENGTH];
    uint8_t innerHash[HASH_LENGTH];

    State state;
    Buffer buffer;
    uint8_t bufferOffset;
};

#endif
