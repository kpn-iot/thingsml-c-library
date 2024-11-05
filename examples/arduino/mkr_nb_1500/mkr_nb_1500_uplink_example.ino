#include <thingsml_http.h>
#include <MKRNB.h>
#include <TemperatureZero.h>      //used to read internal temperature
/*
 * Enable or disable debug
 */
#define DEBUG true
/*
 * Enable or disable debug
 */
#if DEBUG == true
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
/*
 * Begin Configuration
 */
#define INTERVAL_TIME   120000   //Interval time in ms
#define INTERVAL_ENABLE false    //set to true to send message @INTERVAL_TIME. This interval sending doesn't always work (problem in Arduino library or modem firmware?)
#define DEVICE_URN      "urn:dev:IMEI:copy here your IMEI:"
#define DEVICE_KEY      "copy here your shared secret"
/*
 * End Configuration
 */
#define PINNUMBER       ""
#define HTTP_HOST       "m.m"
#define HTTP_IP         "10.151.236.157"
#define HTTP_PATH       "/ingestion/m2m/senml/v1"
#define HTTP_PORT       80

const int INTERVALTIME = 120000;

SenMLPack device(DEVICE_URN);
SenMLDoubleRecord temperature(SENML_NAME_TEMPERATURE, SENML_UNIT_DEGREES_CELSIUS);

TemperatureZero TempZero = TemperatureZero();

NBClient client;
GPRS gprs;
NB nbAccess(true);  //include a 'true' parameter to enable debug

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  TempZero.init();
  device.add(temperature);
  debugln("Starting Arduino MKR NB 1500");
  // connection state
  bool connected = false;

  // After starting the modem with nbAccess.begin()
  // attach to the LTE-M network
  debugln("Connecting to network...");
  while (!connected) {
    if ((nbAccess.begin(PINNUMBER) == NB_READY) &&
        (gprs.attachGPRS() == GPRS_READY)) {
      connected = true;
    } else {
      debugln("Not connected");
      delay(1000);
    }
  }
  debugln("Network connected!");

  if (client.connect(HTTP_IP, HTTP_PORT)) {
    debugln("Connected, to server");
  }
  else {
    debugln("Connection failed");
  }
}

char buff[400] = {0};

void loop() {

  if (client.connected())
  {
    debugln();
    debugln("Client is connected.");
  }
  else
  {
    debugln("Connect, to server");
    client.connect(HTTP_IP, HTTP_PORT); 
    debugln("Connected, to server");
  }
 
  debug("Read sensor: ");
  temperature.set(int(TempZero.readInternalTemperature())+0.5); // Measures the internal uncalibrated core temperature
  debug("Connecting to ");
  debug(HTTP_HOST);
  debug(HTTP_PATH);
  debugln();
  int len = ThingsML::httpPost(buff, 400, DEVICE_KEY, HTTP_HOST, HTTP_PATH, device);
  client.write((uint8_t *) buff, len);
  debugln(buff);
  debugln("data sent, reading...");
  delay(4000);
  client.endWrite(); //////
  //client.flush();
  while(client.available()) {
    Serial.print((char)client.read());
  }
  #if DEBUG && INTERVAL_ENABLE
    Serial.println("Client closed, sleeping for " + String(INTERVAL_TIME/1000) + " s");
  #endif
  digitalWrite(LED_BUILTIN, LOW);
  #if INTERVAL_ENABLE
    delay(INTERVAL_TIME);
  #else
    Serial.println("Client closed");
    while(1);     
  #endif
}
