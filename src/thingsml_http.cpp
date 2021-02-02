
#include "thingsml_http.h"

namespace ThingsML {
    size_t httpPost(char buffer[], int bufferSize, const char key[], const char host[], const char path[], SenMLBasePack &body) {
        int keySize = strlen(key);
        int hostLen = strlen(host);
        int pathLen = strlen(path);
        char * buff = buffer;
        char * token = NULL;
        if (bufferSize < (hostLen + pathLen + 127)) {
            return -1;
        }
        int bodyLen = body.toJson(buffer, bufferSize);
        char contentLength[5] = {0};
        int contentLengthLen = sprintf(contentLength, "%u", bodyLen);
        int total = hostLen + pathLen + bodyLen + contentLengthLen + 127 + 31;
        if (bufferSize < total) {
            return -1;
        }

        // POST /path HTTP/1.1
        memcpy(buff, "POST ", 5);
        buff += 5;
        memcpy(buff, path, pathLen);
        buff += pathLen;
        memcpy(buff, " HTTP/1.1\n", 10);
        buff += 10;

        // Host: domain.com
        memcpy(buff, "Host: ", 6);
        buff += 6;
        memcpy(buff, host, hostLen);
        buff += hostLen;
        memcpy(buff, "\n", 1);
        buff += 1;

        memcpy(buff, "Content-Type: application/json", 30);
        buff += 30;
        memcpy(buff, "\n", 1);
        buff += 1;

        // Things-Message-Token: asdasdsad
        memcpy(buff, "Things-Message-Token: ", 22);
        buff += 22;
        token = buff;
        buff += 64; // Make space for the future token
        memcpy(buff, "\n", 1);
        buff += 1;

        // Content-Length: 1337
        memcpy(buff, "Content-Length: ", 16);
        buff += 16;
        memcpy(buff, contentLength, contentLengthLen);
        buff += contentLengthLen;
        memcpy(buff, "\n\n", 2);
        buff += 2;

        body.toJson(buff, bufferSize - total + bodyLen);

        Sha256 hash;
        hash.init();
        hash.write(buff, bodyLen);
        hash.write(key, keySize);
        hash.result(token, 64);

        return total;
    }

    int getHttpBodyStart(const char response[], int responseLength) {
        int pos = 0;
        for(int i = 0; i < responseLength; i += 1) {
            if (response[i] == '\n') {
                pos += 1;
                if (pos == 2) {
                    return i + 1;
                }
            } else if (response[i] != ' ' && response[i] != '\r') { // Ignore white spaces
                pos = 0;
            }
        }
        return 0;
    }

    int strstr(const char data[], int dataLength, const char needle[], int needleLength) {
        if (needleLength > dataLength) {
            return -1;
        }
        int pos = 0;
        for(int i = 0; i < dataLength; i += 1) {
            if (data[i] == needle[pos]) {
                pos += 1;
                if (pos == needleLength) {
                    return i - needleLength;
                }
            } else { 
                pos = 0;
            }
        }
        return -1;
    }
}