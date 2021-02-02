#include <thingsml_http_firmware.h>
#include <MKRGSM.h>
#include <SPI.h>
#include <SD.h>
#include <SDU.h>
/*
 * Begin Configuration
 * 
 * REQUIRED: SD shield with working FAT32 formatted (micro) sd card installed
 * failure to have this will prevent this sketch from booting
 * 
 * Be carefull when updateing firmwares, we are not responsible from bricking your device.
 * 
 */
#define DEVICE_URN      "urn:dev:IMEI:--fill-me--:"
#define DEVICE_KEY      "--fill-me--"

/*
 * APN information is provided with the sim card
 * If you lost the information the following options can be tried:
 *  - leave empty
 *  - "kpnthings.iot" (for KPN Things M2M)
 *  - "kpnthings2.m2m" (for KPN Things M2M+)
*/
#define APN             "kpnthings2.m2m"

/*
 * Firmware filename, by default NOT "UPDATE.bin".
 * This allows easy testing without actually using the file
 * Recommend to check the file before applying during initial testing
 * This can be done by using the windows "fc -b <file1> <file2>" command
 * 
 * An valid file can be exported using the Arduino IDE -> Sketch -> Export compiled binary
 * This generates the .bin file in the folder where also the sketch (this file) resides.
 * Make sure you use the normal file WITHOUT bootloader in the name
 * 
 */
#define FIRMWARE_FILE "firmware.dat"


/*
 * End Configuration
 */

#define BUFFER_SIZE 4096
// Part size should be at least 256 bytes smaller then BUFFER_SIZE to leave room for Http headers
#define PART_SIZE 2048 

#define PINNUMBER       ""
#define GPRS_LOGIN      ""
#define GPRS_PASSWORD   ""

#define HTTP_HOST       "m.m"
#define HTTP_IP         "10.151.236.136"
#define HTTP_PATH       "/ingestion/m2m/senml/v1"
#define HTTP_PORT       80


#define DEBUG true

SenMLPack device(DEVICE_URN);
SenMLDoubleRecord temperature(SENML_NAME_TEMPERATURE, SENML_UNIT_DEGREES_CELSIUS);
SenMLStringRecord firmware(SENML_NAME_FIRMWARE, SENML_UNIT_NONE, "1.0.0");

GSMClient client;
GPRS gprs;
GSM gsmAccess;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  #if DEBUG
  Serial.println("Sketch compile date and time: " + String(__DATE__) + " " + String(__TIME__));
  Serial.print("Initializing SD card...");
  #endif

  if (!SD.begin(4)) {
    #if DEBUG
    Serial.println("initialization failed!");
    #endif
    while (1);
  }
  #if DEBUG
  Serial.println("initialization done.");
  #endif
 
  device.add(temperature);
  device.add(firmware);
  #if DEBUG
  Serial.println("Starting Arduino MKR 1400 GSM");
  #endif
  // connection state
  bool connected = false;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  #if DEBUG
  Serial.println("Connecting to network...");
  #endif
  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      #if DEBUG      
      Serial.println("Network connection failed, retrying...");
      #endif
      delay(1000);
    }
  }
  #if DEBUG
  Serial.println("Network connected!");
  #endif
}

float sensorValue = 1.5;
char buff[BUFFER_SIZE] = {0};

ThingsML::HttpFirmwareDownload firmwareDownload(DEVICE_URN, PART_SIZE);
File firmwareFile;

size_t getFirmwarePart(int partId, char buff[], int bufferSize) {
  memset(buff, 0, bufferSize);
  String host = firmwareDownload.getHost();
    Serial.println(host + ":" + firmwareDownload.getPort());
    int size = firmwareDownload.getFirmwarePartRequest(partId, buff, bufferSize, HTTP_HOST);
    if (!client.connect(host.c_str(), firmwareDownload.getPort())) {
        #if DEBUG  
        Serial.println("Connection failed");
        #endif
        delay(5000);
        return -1;
    }
    #if DEBUG
    Serial.println("Connected, sending data...");
    #endif
    client.write((uint8_t *) buff, size);
    #if DEBUG
    Serial.println(buff);
    Serial.println("data sent, reading...");
    #endif
    delay(4000);
    int i = 0;
    while(client.available()) {
      i += client.read((uint8_t *) &buff[i], bufferSize - i - 1);
      delay(500);
    }
    buff[i] = '\0';  
    client.stop();
    #if DEBUG
    if (i > 0) {
      for(int o = 0; o < i; o += 1) {
        Serial.print(buff[o]);
      }
      Serial.println("");
    }
    #endif
    return i;
}

bool storePart(int partIndex, char response[], int responseLength, int firmwareLength) {
  int start = ThingsML::getHttpBodyStart(response, responseLength);
  int size = responseLength - start;
  int partSize = (partIndex + 1) * firmwareDownload.getPartSize() > firmwareLength ? 
    firmwareLength - partIndex * firmwareDownload.getPartSize() :
    firmwareDownload.getPartSize();

  if (size != partSize) {
    return false;
  }
  if (ThingsML::HttpFirmwareDownload::getCRC32FromResponse(response, responseLength) !=
    ThingsML::HttpFirmwareDownload::calculateCRC32FromBody(&response[start], size)) {
      #if DEBUG
      Serial.println("Part " + String(partIndex) + " " + 
        String(ThingsML::HttpFirmwareDownload::getCRC32FromResponse(response, responseLength), HEX) + 
        " != " + String(ThingsML::HttpFirmwareDownload::calculateCRC32FromBody(&response[start], size), HEX)  + " crc mismatch");
      #endif
      return false;
  }
  #if DEBUG
  Serial.println("Part " + String(partIndex) + " downloaded");
  #endif
  firmwareFile.seek(partIndex * firmwareDownload.getPartSize());
  firmwareFile.write(&response[start], size);

  return true;
}

void parseMessage(char data[], int length) {
  // Step 1: Parse the message if it is a firmware message
  if (firmwareDownload.parseFirmwareMessage(data, length)) {
    // Step 2: Download the first part, this also gives us the total size an validates the url & token.
    int responseLength = getFirmwarePart(0, buff, BUFFER_SIZE);
    if (responseLength <= 0) {
      #if DEBUG
      Serial.println("Unable to download firmware part");
      #endif
      return;
    }
    int firmwareLength = ThingsML::HttpFirmwareDownload::getTotalSizeFromResponse(buff, responseLength);
    if (firmwareLength <= 0) {
      #if DEBUG
      Serial.println("Unable to determine the total size");
      #endif
      return;
    }
    // Step 3: Setup the file on the sd card.
    if (SD.exists(FIRMWARE_FILE)) {
      SD.remove(FIRMWARE_FILE);
    }
    firmwareFile = SD.open(FIRMWARE_FILE, O_READ | O_WRITE | O_CREAT);
    for(int i = 0; i < firmwareLength; i += 1) {
      firmwareFile.write((uint8_t) 0x00);
    }
    firmwareFile.flush();
    
    // Also store the first part :-)
    if (!storePart(0, buff, responseLength, firmwareLength)) {
      firmwareFile.close();
      return;
    }
    // Step 4: Download all the other parts.
    int totalParts = firmwareDownload.getNumParts(firmwareLength);
    for (int i = 1; i < totalParts; i += 1) {
      int o = 0;
      while(o++ < 3) {
        responseLength = getFirmwarePart(i, buff, BUFFER_SIZE);
        if (storePart(i, buff, responseLength, firmwareLength)) {
          break;
        }
      }
      if (o == 3) {
        firmwareFile.close();
        return;
      }
    }
    firmwareFile.close();
    // Step 5: Reboot the system and let the SDU bootloader do it's thing.
    NVIC_SystemReset();
  }
}

void sendMessage() {
  #if DEBUG
  Serial.print("Read sensor: ");
  Serial.println(sensorValue);
  #endif
  temperature.set(31.42); // Hot in here

  #if DEBUG
  Serial.print("Connecting to ");
  Serial.print(HTTP_HOST);
  Serial.print(HTTP_PATH);
  Serial.println();
  #endif

  if (!client.connect(HTTP_IP, HTTP_PORT)) {
    #if DEBUG  
    Serial.println("Connection failed");
    #endif
    delay(5000);
    return;
  }
  #if DEBUG
  Serial.println("Connected, sending data...");
  #endif
  int len = ThingsML::httpPost(buff, 400, DEVICE_KEY, HTTP_HOST, HTTP_PATH, device);
  client.write((uint8_t *) buff, len);
  #if DEBUG
  Serial.println(buff);
  Serial.println("data sent, reading...");
  #endif
  delay(4000);
  int i = 0;
  while(client.available()) {
    i += client.read((uint8_t *) &buff[i], BUFFER_SIZE - i - 1);
    delay(100);
  }
  buff[i] = '\0'; // Properly zero terminate so that Serial.println() can nicely print.
  client.stop();

  if (i > 0) {
    #if DEBUG
    Serial.println(buff);
    #endif
    parseMessage(buff, i);
  }
  
  #if DEBUG
  Serial.println("Client closed, sleeping for 60s");
  #endif
}

void loop() {
  sendMessage();
  delay(60000);
}