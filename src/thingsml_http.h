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
 * 
 * ThingsML helper for http requests
 * 
 */
#ifndef THINGSML_HTTP
#define THINGSML_HTTP

#include "thingsml.h"
#include <string.h>
#include <stdio.h>
#include "util/sha256.h"

namespace ThingsML {
    /**
     * Generates a complete http POST request ready to be sent over an tcp socket.
     * 
     * @param buffer output buffer
     * @param bufferSize the size of the output buffer
     * @param key the secret as configured for the device
     * @param host the http host for the http Host: header
     * @param path the http path for the http start
     * 
     * @return the size of the body or -1 if the buffer is too small.
     */
    size_t httpPost(char buffer[], int bufferSize, const char key[], const char host[], const char path[], SenMLBasePack &body);

    /**
     * Finds where the http headers end and the body begins.
     * Will return the beginning if the end of the http headers could not be found.
     * 
     * @returns the index of the body
     */
    int getHttpBodyStart(const char response[], int responseLength);


    /**
     * Fixed length strstr implementation
     * 
     * @param data the data to search
     * @param dataLength the length of data
     * @param needle the needle to search for
     * @param needleLength the length of the needle
     * 
     * @return the index of the start of the needle or -1 if the needle could not be found
     */
    int strstr_n(const char data[], int dataLength, const char needle[], int needleLength);
    
    // size_t httpPostCbor(char buffer[], int bufferSize, char key[], char host[], char path[], SenMLBasePack &body) {
    //     int keySize = strlen(key);
    //     int hostLen = strlen(host);
    //     int pathLen = strlen(path);
    //     char * buff = buffer;
    //     char * token = NULL;
    //     if (bufferSize < (hostLen + pathLen + 127)) {
    //         return -1;
    //     }
    //     int bodyLen = body.toCbor(buffer, bufferSize, SENML_HEX) * 2; // Hex duplicates
    //     char contentLength[5] = {0};
    //     int contentLengthLen = sprintf(contentLength, "%u", bodyLen);
    //     int total = hostLen + pathLen + bodyLen + contentLengthLen + 127;
    //     if (bufferSize < total) {
    //         return -1;
    //     }
        
    //     // POST /path HTTP/1.1
    //     memcpy(buff, "POST ", 5);
    //     buff += 5;
    //     memcpy(buff, path, pathLen);
    //     buff += pathLen;
    //     memcpy(buff, " HTTP/1.1\n", 10);
    //     buff += 10;

    //     // Host: domain.com
    //     memcpy(buff, "Host: ", 6);
    //     buff += 6;
    //     memcpy(buff, host, hostLen);
    //     buff += hostLen;
    //     memcpy(buff, "\n", 1);
    //     buff += 1;

    //     // Things-Message-Token: asdasdsad
    //     memcpy(buff, "Things-Message-Token: ", 22);
    //     buff += 22;
    //     token = buff;
    //     buff += 64; // Make space for the future token
    //     memcpy(buff, "\n", 1);
    //     buff += 1;

    //     // Content-Length: 1337
    //     memcpy(buff, "Content-Length: ", 16);
    //     buff += 16;
    //     memcpy(buff, contentLength, contentLengthLen);
    //     buff += contentLengthLen;
    //     memcpy(buff, "\n\n", 2);
    //     buff += 2;

    //     body.toCbor(buff, bufferSize - (buff - buffer), SENML_HEX);

    //     Sha256 hash;
    //     hash.init();
    //     hash.write(buff, bodyLen);
    //     hash.write(key, keySize);
    //     hash.result(token, 64);

    //     return total;
    // }
}

#endif // !THINGSML_HTTP