/*  _  __  ____    _   _
 * | |/ / |  _ \  | \ | |
 * | ' /  | |_) | |  \| |
 * | . \  |  __/  | |\  |
 * |_|\_\ |_|     |_| \_|
 *
 * (c) 2021 KPN
 * License: MIT License.
 * Author: Joseph Verburg, Jan Bogaerts
 * 
 * 
 * ThingsML helper for http firmware downloads
 * 
 */
#include "thingsml_http_firmware.h"
#include <string.h>

#define MAX_STR_LENGTH 1024

static String makeString(const char * buff, int subLength) {
    char * copy = new char[subLength + 1];
    memcpy(copy, buff, subLength);
    copy[subLength] = '\0';
    String str(copy);
    free(copy);
    return str;
}

static int hexToInt(char c) {
    int value = 0;
    if(c >= '0' && c <= '9')
        return  (c - '0');
    else if (c >= 'A' && c <= 'F') 
        return (10 + (c - 'A'));
    else if (c >= 'a' && c <= 'f')
        return (10 + (c - 'a'));
    else 
        return -1;
}

namespace ThingsML {


    size_t HttpFirmwareDownload::getFirmwarePartRequest(int partIndex, char buffer[], int bufferLength,char host[]) {
        if (_token.get() == nullptr || _url.get() == nullptr) {
            return -1;
        }
        
        int tokenLength = strnlen(_token.get(), MAX_STR_LENGTH);
        const char * path = getUrlPath();
        int pathLength = strnlen(path, MAX_STR_LENGTH);
        int hostLength = strnlen(host, MAX_STR_LENGTH);
        if (tokenLength <= 0 || pathLength <= 0) {
            return -1;
        }

        char startRangeString[9] = {0}; 
        char endRangeString[9] = {0}; 

        int startRangeLength = sprintf(startRangeString, "%u", partIndex * getPartSize());
        int endRangeLength = sprintf(endRangeString, "%u", (partIndex + 1) * getPartSize() - 1);

        int total = tokenLength + pathLength + hostLength + startRangeLength + endRangeLength + 65;
        if (total >= bufferLength) {
            return -1;
        }
        char * buff = buffer; // Local position in buffer.

        // GET /path HTTP/1.1      
        memcpy(buff, "GET ", 4);
        buff += 4;
        memcpy(buff, path, pathLength);
        buff += pathLength;
        memcpy(buff, " HTTP/1.1\r\n", 11);
        buff += 11;

        // Host: domain.com
        memcpy(buff, "Host: ", 6);
        buff += 6;
        memcpy(buff, host, hostLength);
        buff += hostLength;
        memcpy(buff, "\r\n", 2);
        buff += 2;

        // Authorization: Bearer asdasdsadas
        memcpy(buff, "Authorization: Bearer ", 22);
        buff += 22;
        memcpy(buff, _token.get(), tokenLength);
        buff += tokenLength;
        memcpy(buff, "\r\n", 2);
        buff += 2;

        // Content-Range: 0-1024
        memcpy(buff, "Range: bytes=", 13);
        buff += 13;
        memcpy(buff, startRangeString, startRangeLength);
        buff += startRangeLength;
        memcpy(buff, "-", 1);
        buff += 1;
        memcpy(buff, endRangeString, endRangeLength);
        buff += endRangeLength;
        memcpy(buff, "\r\n", 2);
        buff += 2;

        memcpy(buff, "\r\n", 2);
        buff += 2;

        buff[0] = '\0';

        return total;
    }

    bool HttpFirmwareDownload::parseFirmwareMessage(char message[], int messageLength) {
        _url.set(nullptr);
        _token.set(nullptr);
        if (!isFirmwareMessage(message, messageLength)) {
            return false;
        }
        int start = getHttpBodyStart(message, messageLength);
        _firmwareMessage.fromJson(&message[start]);

        return _url.get() != nullptr && _token.get() != nullptr;
    }

    const char * HttpFirmwareDownload::getUrlPath() {
        if (_url.get() == nullptr) {
            return nullptr;
        }
        const char * hostnameStart = strstr(_url.get(), "://");
        if (hostnameStart == nullptr) {
            return nullptr;
        }
        return strstr(hostnameStart + 3, "/");
    }

    String HttpFirmwareDownload::getHost() {
        if (_url.get() == nullptr) {
            return String();
        }
        const char * hostnameStart = strstr(_url.get(), "://");
        if (hostnameStart == nullptr) {
            return String();
        }
        hostnameStart = &hostnameStart[3]; // skip ://
        const char * portStart = strstr(hostnameStart, ":");
        const char * hostnameEnd = strstr(hostnameStart, "/");
        const char * queryStart = strstr(hostnameStart, "?");
        if (portStart != nullptr) {
            return makeString(hostnameStart, portStart - hostnameStart);
        }
        if (hostnameEnd != nullptr) {
            return makeString(hostnameStart, hostnameEnd - hostnameStart);
        }
        if (queryStart != nullptr) {
            return makeString(hostnameStart, queryStart - hostnameStart);
        }
        return String(hostnameStart);
    }

    int HttpFirmwareDownload::getPort() {
        if (_url.get() == nullptr) {
            return 0;
        }
        const char * hostnameStart = strstr(_url.get(), "://");
        if (hostnameStart == nullptr) {
            return 0;
        }
        hostnameStart = &hostnameStart[3]; // skip ://
        const char * portStart = strstr(hostnameStart, ":");
        if (portStart != nullptr) {
            int port = 0;
            for (int i = 1; i < 6; i ++) { // max 5 digits: 65535
                if (
                    portStart[i] - '0' < 0 ||
                    portStart[i] - '0' > 10
                ) {
                    break;
                }
                port *= 10;
                port += portStart[i] - '0';
            }
            return port;
        }
        return strstr(_url.get(), "https://") != nullptr ? 443 : 80;
    }

    bool HttpFirmwareDownload::isFirmwareMessage(char message[], int messageLength) {
        int start = getHttpBodyStart(message, messageLength);
        if (start >= (messageLength - 2)) { // Body not long enough to be valid
            return false;
        }
        if (message[start] != '[') { // Must start with json array
            return false;
        }
        if (strstr_n(&message[start], messageLength - start, "token", 5) == -1) {
            return false;
        }
        if (strstr_n(&message[start], messageLength - start, "url", 3) == -1) {
            return false;
        }
        return true;
    }

    int HttpFirmwareDownload::getTotalSizeFromResponse(char message[], int messageLength) {
        const char * ContentRange = "Content-Range:bytes";
        const int length = 19;
        int pos = 0;
        int i = 0;
        for(; i < messageLength; i += 1) {
            if (ContentRange[pos] == message[i]) {
                pos++;
                if (pos == length) {
                    break;
                }
            } else if(message[i] != ' ') {
                pos = 0;
            }
        }
        if (pos != length) {
            return 0;
        }
        for(i = i + 1;i < messageLength; i += 1) {
            // Found the start of the total size!
            if (message[i] == '/') {
                break;
            }
            // May only contain space, - and 0-9
            if (message[i] != ' ' &&
                message[i] != '-' &&
                !(
                    (message[i] - '0') >= 0 &&
                    (message[i] - '0') < 10
                )) {
                return 0;
            }
        }
        if (message[i] != '/') {
            return 0;
        }
        int totalSize = 0;
        for(i = i + 1;i < messageLength; i += 1) {
            if (
                message[i] - '0' < 0 ||
                message[i] - '0' > 10
            ) {
                break;
            }
            totalSize *= 10;
            totalSize += message[i] - '0';
        }
        return totalSize;
    }


    uint32_t HttpFirmwareDownload::getCRC32FromResponse(char message[], int messageLength) {
        const char * ContentRange = "Digest:crc32=";
        const int length = 13;
        int pos = 0;
        int i = 0;
        for(; i < messageLength; i += 1) {
            if (ContentRange[pos] == message[i]) {
                pos++;
                if (pos == length) {
                    break;
                }
            } else if(message[i] != ' ') {
                pos = 0;
            }
        }
        if (pos != length) {
            return 0;
        }
        uint32_t value = 0;
        for(i = i + 1;i < messageLength; i += 1) {
            int val = hexToInt(message[i]);
            if (val == -1) {
                break;
            }
            value = value << 4;
            value += val;
        }
        return value;
    }

    /*
        From https://create.stephan-brumme.com/crc32/#tableless
    */
    uint32_t HttpFirmwareDownload::calculateCRC32FromBody(char body[], int bodyLength) {
        uint32_t crc = 0xFFFFFFFF;
        const uint8_t* current = (const uint8_t*) body;
        while (bodyLength-- != 0)
        {
            uint8_t s = uint8_t(crc) ^ *current++;
            uint32_t low = (s ^ (s << 6)) & 0xFF;
            uint32_t a   = (low * ((1 << 23) + (1 << 14) + (1 << 2)));
            crc = (crc >> 8) ^
                (low * ((1 << 24) + (1 << 16) + (1 << 8))) ^
                a ^
                (a >> 1) ^
                (low * ((1 << 20) + (1 << 12)           )) ^
                (low << 19) ^
                (low << 17) ^
                (low >>  2);
        }
        return ~crc; // same as crc ^ 0xFFFFFFFF
    }
}