/*  _  __  ____    _   _
 * | |/ / |  _ \  | \ | |
 * | ' /  | |_) | |  \| |
 * | . \  |  __/  | |\  |
 * |_|\_\ |_|     |_| \_|
 *
 * (c) 2021 KPN
 * License: MIT License.
 * Author: Joseph Verburg
 * 
 * tests to check the correct functioning of our http helper.
 * 
 */
#include "../src/thingsml_http_firmware.h"
#include <string.h>
#include <iostream>
#include "gtest/gtest.h" 

#define VALID_FIRMWARE_MESSAGE "HTTP/1.1 200 OK\r\nDate: Mon, 11 Jan 2021 15:15:22 GMT\r\nServer: Apache/2\r\nContent-Length: 302\r\nVary: Accept-Encoding,User-Agent\r\nContent-Type: text/plain\r\n\r\n[{\"bn\":\"urn:dev:IMEI:0123456789abcdef:\"},{\"n\":\"url\",\"vs\":\"http://123.123.123.123/f/v1/12345678-1234-5678-1234-567812345678\"},{\"n\":\"token\",\"vs\":\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c\"}]"
#define VALID_FIRMWARE_MESSAGE2 "HTTP/1.1 200 OK\r\nDate: Mon, 11 Jan 2021 15:15:22 GMT\r\nServer: Apache/2\r\nContent-Length: 303\r\nVary: Accept-Encoding,User-Agent\r\nContent-Type: text/plain\r\n\r\n[{\"bn\":\"urn:dev:IMEI:0123456789abcdef:\"},{\"n\":\"url\",\"vs\":\"https://123.123.123.345/f/v1/12345678-1234-5678-1234-567812345678\"},{\"n\":\"token\",\"vs\":\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c\"}]"
#define VALID_FIRMWARE_MESSAGE3 "HTTP/1.1 200 OK\r\nDate: Mon, 11 Jan 2021 15:15:22 GMT\r\nServer: Apache/2\r\nContent-Length: 305\r\nVary: Accept-Encoding,User-Agent\r\nContent-Type: text/plain\r\n\r\n[{\"bn\":\"urn:dev:IMEI:0123456789abcdef:\"},{\"n\":\"url\",\"vs\":\"http://123.123.123.567:81/f/v1/12345678-1234-5678-1234-567812345678\"},{\"n\":\"token\",\"vs\":\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c\"}]"
#define VALID_MESSAGE "HTTP/1.1 200 OK\r\nDate: Mon, 11 Jan 2021 15:15:22 GMT\r\nServer: Apache/2\r\nContent-Length: 301\r\nVary: Accept-Encoding,User-Agent\r\nContent-Type: text/plain\r\n\r\n[{\"bn\":\"urn:dev:IMEI:0123456789abcdef:\"},{\"n\":\"url\",\"vs\":\"http://123.123.123.123/f/v1/12345678-1234-5678-1234-567812345678\"},{\"n\":\"tokeen\",\"vs\":\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c\"}]"
#define VALID_RESPONSE "HTTP/1.1 206 Partial Content\r\nDate: Wed, 13 Jan 2021 15:17:58 GMT\r\nServer: Apache/2\r\nLast-Modified: Sat, 14 Nov 2020 10:26:19 GMT\r\nAccept-Ranges: bytes\r\nContent-Length: 101\r\nContent-Range: bytes 0-100/1544736\r\nContent-Type: text/plain\r\n\r\nMon, 06 Jan 2020 12:43:00 +0100\r\n{\"PATH\":\"/usr/local/bin:/usr/bin:/bin\",\"TEMP\":\"/tmp\",\"TMP\":\"/"

TEST(thingsml_firmware, is_firmware_message) {
    const char * message = VALID_FIRMWARE_MESSAGE;
    EXPECT_TRUE(ThingsML::HttpFirmwareDownload::isFirmwareMessage(message, strlen(message)));
    message = VALID_MESSAGE;
    EXPECT_FALSE(ThingsML::HttpFirmwareDownload::isFirmwareMessage(message, strlen(message)));
}

TEST(thingsml_firmware, parse_firmware_message) {
    const char * message = VALID_FIRMWARE_MESSAGE;
    ThingsML::HttpFirmwareDownload download("urn:dev:IMEI:0123456789abcdef:");
    download.parseFirmwareMessage(message, strlen(message));
}

TEST(thingsml_firmware, generate_firmware_request) {
    const char * message = VALID_FIRMWARE_MESSAGE;
    ThingsML::HttpFirmwareDownload download("urn:dev:IMEI:0123456789abcdef:");
    download.parseFirmwareMessage(message, strlen(message));

    char buff[1024] = {0};
    int size = download.getFirmwarePartRequest(0, buff, 1024, "m.m");
    EXPECT_EQ(269, strlen(buff));
    EXPECT_EQ(size, strlen(buff));
    EXPECT_STREQ("GET /f/v1/12345678-1234-5678-1234-567812345678 HTTP/1.1\r\nHost: m.m\r\nAuthorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c\r\nRange: bytes=0-255\r\n\r\n", buff);

    memset(buff, 0, 1024);

    size = download.getFirmwarePartRequest(4, buff, 1024, "m.m");
    EXPECT_EQ(273, strlen(buff));
    EXPECT_EQ(size, strlen(buff));
    EXPECT_STREQ("GET /f/v1/12345678-1234-5678-1234-567812345678 HTTP/1.1\r\nHost: m.m\r\nAuthorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c\r\nRange: bytes=1024-1279\r\n\r\n", buff);
}

TEST(thingsml_firmware, get_firmware_size) {
    ThingsML::HttpFirmwareDownload download("urn:dev:IMEI:0123456789abcdef:");

    int size = download.getTotalSizeFromResponse(VALID_RESPONSE, strlen(VALID_RESPONSE));
    EXPECT_EQ(1544736, size);

    size = download.getTotalSizeFromResponse(VALID_MESSAGE, strlen(VALID_MESSAGE));
    EXPECT_EQ(0, size);
}

TEST(thingsml_firmware, get_host) {
    ThingsML::HttpFirmwareDownload download("urn:dev:IMEI:0123456789abcdef:");
    
    download.parseFirmwareMessage(VALID_FIRMWARE_MESSAGE, strlen(VALID_FIRMWARE_MESSAGE));
    String host = download.getHost();
    EXPECT_STREQ("123.123.123.123", host.c_str());

    download.parseFirmwareMessage(VALID_FIRMWARE_MESSAGE2, strlen(VALID_FIRMWARE_MESSAGE2));
    host = download.getHost();
    EXPECT_STREQ("123.123.123.345", host.c_str());
    
    download.parseFirmwareMessage(VALID_FIRMWARE_MESSAGE3, strlen(VALID_FIRMWARE_MESSAGE3));
    host = download.getHost();
    EXPECT_STREQ("123.123.123.567", host.c_str());
}

TEST(thingsml_firmware, get_port) {
    ThingsML::HttpFirmwareDownload download("urn:dev:IMEI:0123456789abcdef:");
    
    download.parseFirmwareMessage(VALID_FIRMWARE_MESSAGE, strlen(VALID_FIRMWARE_MESSAGE));
    EXPECT_EQ(80, download.getPort());
    
    download.parseFirmwareMessage(VALID_FIRMWARE_MESSAGE2, strlen(VALID_FIRMWARE_MESSAGE2));
    EXPECT_EQ(443, download.getPort());

    download.parseFirmwareMessage(VALID_FIRMWARE_MESSAGE3, strlen(VALID_FIRMWARE_MESSAGE3));
    EXPECT_EQ(81, download.getPort());

    download.parseFirmwareMessage(VALID_MESSAGE, strlen(VALID_MESSAGE));
    EXPECT_EQ(0, download.getPort());
}

#define BODY "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent et pharetra velit. Donec sed magna dui. Etiam augue dui, accumsan ac lacus vel, sollicitudin finibus augue. Phasellus fermentum sem mauris, ut feugiat lorem mollis eu. In hac habitasse plate"
#define RESPONSE "HTTP/1.1 200\r\nDate: Fri, 29 Jan 2021 12:56:15 GMT\r\nContent-Length: 256\r\nConnection: keep-alive\r\nDigest: crc32=710bbd48\r\nContent-Range: bytes 0-255/3483\r\n\r\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent et pharetra velit. Donec sed magna dui. Etiam augue dui, accumsan ac lacus vel, sollicitudin finibus augue. Phasellus fermentum sem mauris, ut feugiat lorem mollis eu. In hac habitasse plate"

TEST(thingsml_firmware, crc32_get) {
    uint32_t value = ThingsML::HttpFirmwareDownload::getCRC32FromResponse(RESPONSE, strlen(RESPONSE));
    EXPECT_EQ(1896594760, value); 
    EXPECT_EQ(0x710BBD48, value);
}

TEST(thingsml_firmware, crc32_calculate) {
    uint32_t value = ThingsML::HttpFirmwareDownload::calculateCRC32FromBody(BODY, strlen(BODY));
    EXPECT_EQ(1896594760, value);
    EXPECT_EQ(0x710BBD48, value);
}