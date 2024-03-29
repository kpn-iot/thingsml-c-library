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
#ifndef THINGSML_HTTP_FIRMWARE
#define THINGSML_HTTP_FIRMWARE

#include "thingsml_http.h"
#include <string.h>
#include <stdio.h>

namespace ThingsML {
    class HttpFirmwareDownload {
        public:
        
        HttpFirmwareDownload(const char baseName[]): _firmwareMessage(baseName), _url(SENML_NAME_URL), _token(SENML_NAME_TOKEN), _partSize(256) {
            _firmwareMessage.add(_url);
            _firmwareMessage.add(_token);
        };
        HttpFirmwareDownload(const char baseName[], int partSize): _firmwareMessage(baseName), _url(SENML_NAME_URL), _token(SENML_NAME_TOKEN), _partSize(partSize) {
            _firmwareMessage.add(_url);
            _firmwareMessage.add(_token);
        };

        /**
         * @param partSize sets the currently configured part size in bytes
         */
        void setPartSize(int partSize) {
            this->_partSize = partSize;
        }

        /**
         * @returns currently configured part size in bytes
         */
        int getPartSize() {
            return this->_partSize;
        }

        /**
         * @returns number of times currently configured part size fits into the total size (rounded up).
         */
        int getNumParts(int totalSize) {
            return (totalSize / _partSize) + (totalSize % _partSize == 0 ? 0 : 1);
        }

        /**
         * 
         * @returns size of the request in bytes
         */
        size_t getFirmwarePartRequest(int partIndex, char buffer[], int bufferLength, const char host[]);

        /**
         * Parse the firmware message
         * 
         * @returns true if the message was successfully parsed.
         */
        bool parseFirmwareMessage(const char message[], int messageLength);

        /**
         *  @returns true if the message contained in the data contains a firmware payload.
         */
        static bool isFirmwareMessage(const char message[], int messageLength);

        /**
         *  This function extracts the total byte size of the resource
         *  This can be used to know how many parts need to be requested.
         * 
         *  @returns the total amount of bytes the file contains
         */
        static int getTotalSizeFromResponse(const char message[], int messageLength);


        static uint32_t getCRC32FromResponse(const char message[], int messageLength);

        static uint32_t calculateCRC32FromBody(const char body[], int bodyLength);

        /**
         * The return value equal to nullptr indicates that a hostname could not be detected.
         * 
         * @returns the hostname or ip of the firmware file server
         */
        String getHost();

        /**
         * The return value 0 indicates that a port could not be detected.
         * 
         * @returns the port to connect to of the firmware file server
         */
        int getPort();

        private: 
            SenMLPack _firmwareMessage;
            SenMLStringActuator _url;
            SenMLStringActuator _token;
            int _partSize = 256;
            const char * getUrlPath();
    };
}

#endif