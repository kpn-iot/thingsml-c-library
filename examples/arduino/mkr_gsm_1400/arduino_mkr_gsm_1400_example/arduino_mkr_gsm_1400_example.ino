#include <thingsml_http.h>
#include <MKRGSM.h>

/*
 * Begin Configuration
 */
#define DEVICE_URN      "urn:dev:IMEI:--fill-imei--:"
#define DEVICE_KEY      "--fill-me--"

/*
 * APN information is provided with the sim card
 * If you lost the information the following options can be tried:
 *  - leave empty
 *  - "kpnthings.iot" (for KPN Things M2M)
 *  - "kpnthings2.m2m" (for KPN Things M2M+)
*/
#define APN             "--fill-me--"
/*
 * End Configuration
 */


#define PINNUMBER       ""
#define GPRS_LOGIN      ""
#define GPRS_PASSWORD   ""

#define HTTP_HOST       "m.m"
#define HTTP_IP         "10.151.236.157"
#define HTTP_PATH       "/ingestion/m2m/senml/v1"
#define HTTP_PORT       80


#define DEBUG true

SenMLPack device(DEVICE_URN);
SenMLDoubleRecord temperature(SENML_NAME_TEMPERATURE, SENML_UNIT_DEGREES_CELSIUS);

GSMClient client;
GPRS gprs;
GSM gsmAccess;

void setup() {
  Serial.begin(9600);
  while (!Serial);
 
  device.add(temperature);
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
char buff[400] = {0};

void loop() {
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
  while(client.available()) {
    Serial.print((char)client.read());
  }
  client.stop();
#if DEBUG
  Serial.println("Client closed, sleeping for 30s");
#endif
  delay(30000);
}