#include <thingsml_http.h>
#include <Sodaq_R4X.h> 
#include <Sodaq_wdt.h> 
#include <Sodaq_LSM303AGR.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Wire.h>

/*
 * *****************
 * NOTICE
 * *****************
 * 
 * This board requires additional libraries!
 * 
 * 1. Under File->Preferences->Additional Boards Manager URLs input:
 * http://downloads.sodaq.net/package_sodaq_samd_index.json
 * 
 * 2. Then under Tools->Board->Boards manager download:
 * SODAQ SAMD Boards
 * 
 * 2.1 Don't forget to select the board Sodaq Sara under Tools->Board :')
 * 
 * 3. Then under Tools->Manage libraries install the following 3 libraries:
 * Sodaq_LSM303AGR
 * Sodaq_R4X
 * Sodaq_wdt
 * 
 * 4. Configure details below.
 */

/*
 * Configuration
 */
#define DEVICE_URN      "urn:dev:IMEI:--fill-me--:"
#define DEVICE_KEY      "--fill-me--"

#define PINNUMBER       "" // Normally not needed

#define APN        "kpnthings2.m2m"
#define APN_LOGIN      "" // Normally not needed
#define APN_PASSWORD   "" // Normally not needed

#define HTTP_HOST       "m.m"
#define HTTP_IP         "10.151.236.157"
#define HTTP_PATH       "/ingestion/m2m/senml/v1"
#define HTTP_PORT       80



#define CONSOLE_STREAM   SerialUSB
#define MODEM_STREAM     Serial1

#define GPS_ADR 0x42
  
#define CURRENT_OPERATOR AUTOMATIC_OPERATOR
#define CURRENT_URAT     SODAQ_R4X_LTEM_URAT
#define CURRENT_MNO_PROFILE MNOProfiles::STANDARD_EUROPE

static Sodaq_R4X r4x;
static Sodaq_LSM303AGR AccMeter;
static Sodaq_SARA_R4XX_OnOff saraR4xxOnOff;
static bool isReady;
static bool isOff;

SenMLPack device(DEVICE_URN);
SenMLIntRecord temperature(SENML_NAME_TEMPERATURE, SENML_UNIT_DEGREES_CELSIUS);
SenMLDoubleRecord latitude(SENML_NAME_LATITUDE, SENML_UNIT_DEGREES_LATITUDE);
SenMLDoubleRecord longitude(SENML_NAME_LONGITUDE, SENML_UNIT_DEGREES_LONGITUDE);

#define STARTUP_DELAY 5000
#ifndef NBIOT_BANDMASK
#define NBIOT_BANDMASK BAND_MASK_UNCHANGED
#endif
unsigned long previousMillis = 0;
const long interval = 60000;

#define BUFFER_SIZE 1024
char buff[BUFFER_SIZE] = {0};

struct GPSInfo {
  bool hasFix = false;
  double latitude = 0;
  double longitude = 0;
};

struct GPSInfo readGPS() {
  memset(buff, 0, BUFFER_SIZE);
  uint16_t count = 0;
  Wire.beginTransmission(GPS_ADR);
  Wire.write((uint8_t)0xFD);
  Wire.endTransmission(false);
  Wire.requestFrom(GPS_ADR, 2);
  count = (uint16_t)(Wire.read() << 8) | Wire.read();
  count = (count > BUFFER_SIZE) ? BUFFER_SIZE : count;
  GPSInfo info;
  if (count) {
    for (size_t i = 0; i < (count-1); i++) {
      Wire.requestFrom(GPS_ADR, 1, false);
      buff[i] = Wire.read();
    }
    Wire.requestFrom(GPS_ADR, 1);
    buff[count-1] = Wire.read();
    // http://navspark.mybigcommerce.com/content/NMEA_Format_v0.1.pdf
    char * line = strstr(buff, "$GNRMC");
  
    if (line != NULL) {
      char * next = strchr(line, ',');
      int i = 0;
      while(next != NULL) {
         next = strchr(next + 1, ',');
         if (i == 0) {
            info.hasFix = next[1] == 'A';
         } else if (i == 1 && info.hasFix) {
            char * end = strchr(next + 1, ',');
            info.latitude = strtod(next + 1, &end) / 100;
         } else if (i == 2 && info.hasFix) {
            info.latitude *= next[1] == 'N' ? 1 : -1;
         } else if (i == 3 && info.hasFix) {
            char * end = strchr(next + 1, ',');
            info.longitude = strtod(next + 1, &end) / 100;
         } else if (i == 4 && info.hasFix) {
            info.longitude *= next[1] == 'E' ? 1 : -1;
         } else if (i > 4) {
            break;
         }
         i++;
      }
    }
  } else {
     Wire.endTransmission();
  }

  return info;
}

void setup() {
  device.add(temperature);
  device.add(latitude);
  device.add(longitude);
  
  sodaq_wdt_safe_delay(STARTUP_DELAY);
  
  pinMode(GPS_ENABLE, OUTPUT);
  digitalWrite(GPS_ENABLE, 1);
  
  Wire.begin();

  AccMeter.rebootAccelerometer();
  delay(1000);

  // Enable the Accelerometer
  AccMeter.enableAccelerometer();

  while ((!CONSOLE_STREAM) && (millis() < 10000)) {
    // Wait max 10 sec for the CONSOLE_STREAM to open
  }
  CONSOLE_STREAM.begin(115200);
  CONSOLE_STREAM.println("Console open");
  MODEM_STREAM.begin(r4x.getDefaultBaudrate());

  r4x.setDiag(CONSOLE_STREAM);
  r4x.init(&saraR4xxOnOff, MODEM_STREAM);

  CONSOLE_STREAM.println("Attempting initial gps fix");
  for (int i = 0; i < 100; i += 1) {
    GPSInfo info = readGPS(); 
    if (info.hasFix) {
      CONSOLE_STREAM.println("Initial gps fix succesfull");
      break;
    }
    delay(100);
  }
  
}

void loop() {
  CONSOLE_STREAM.println("Turning on modem...");
  r4x.on();
  CONSOLE_STREAM.println("Connecting to network...");
  isReady = r4x.connect(APN, CURRENT_URAT, CURRENT_MNO_PROFILE, CURRENT_OPERATOR, BAND_MASK_UNCHANGED, NBIOT_BANDMASK);
  CONSOLE_STREAM.println(isReady ? "Network connected" : "Network connection failed");
  
  if (isReady) {
    postHTTP();
  }
  isOff = r4x.off();
  CONSOLE_STREAM.println(isOff ? "Modem off" : "Power off failed");  
  CONSOLE_STREAM.println("------------");
  sodaq_wdt_safe_delay(interval);
}

int8_t getBoardTemperature() {
  int8_t temp = AccMeter.getTemperature();
  return temp;
}

void postHTTP() {
  temperature.set(AccMeter.getTemperature());
  GPSInfo info = readGPS();
  int i = 0;
  while(!info.hasFix && i < 100) {
    info = readGPS();
    if (info.hasFix) {
      break;
    }
    delay(100);
    i++;
  }
  if (info.hasFix) {
    latitude.set(info.latitude);
    longitude.set(info.longitude);
  }
  int len = ThingsML::httpPost(buff, BUFFER_SIZE, DEVICE_KEY, HTTP_HOST, HTTP_PATH, device);

  CONSOLE_STREAM.println("Sending message...");
  uint8_t socketId = r4x.socketCreate(0, TCP);
  r4x.socketConnect(socketId, HTTP_IP, HTTP_PORT);

  r4x.socketWrite(socketId, (uint8_t *) buff, len);
  r4x.socketWaitForRead(socketId);
  CONSOLE_STREAM.println("Receiving message...");
  int receiveLength = r4x.socketRead(socketId, (uint8_t *) buff, BUFFER_SIZE);

  r4x.socketClose(socketId);

  CONSOLE_STREAM.print("Message response length: ");
  CONSOLE_STREAM.println(receiveLength);

  if (receiveLength > 0) {
    buff[receiveLength] = 0; // Null terminate the string
    CONSOLE_STREAM.println("Message response:");
    CONSOLE_STREAM.println(buff);
  }
  CONSOLE_STREAM.println("Message sending finished.");
}