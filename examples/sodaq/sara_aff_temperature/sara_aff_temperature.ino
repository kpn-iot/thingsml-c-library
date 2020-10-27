#include <thingsml_http.h>
#include <Sodaq_R4X.h> 
#include <Sodaq_wdt.h> 
#include <Sodaq_LSM303AGR.h>

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

#ifndef NBIOT_BANDMASK
#define NBIOT_BANDMASK BAND_MASK_UNCHANGED
#endif

#define BUFFER_SIZE 1024
#define STARTUP_DELAY 5000

unsigned long previousMillis = 0;
const long interval = 60000;

void setup() {
  device.add(temperature);
  
  sodaq_wdt_safe_delay(STARTUP_DELAY);

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


char buff[BUFFER_SIZE] = {0};

void postHTTP() {
  temperature.set(AccMeter.getTemperature());
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