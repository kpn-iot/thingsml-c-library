#include <MKRWAN.h>
#include <thingsml.h>

/**
 * Configuration
 */
#define APPEUI "--fill-me--"
#define APPKEY "--fill-me--"
#define DEBUG true

SenMLPack device;
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);

LoRaModem modem;

void setup() {
    Serial.begin(9600);
    while (!Serial)
        ;

    device.add(temperature);

    // change this to your regional band (eg. US915, AS923, ...)
    if (!modem.begin(EU868)) {

#if DEBUG
        Serial.println("Failed to start module");
#endif
        while (1) {
        }
    };

#if DEBUG
    Serial.print("Your module version is: ");
    Serial.println(modem.version());
    Serial.print("Your device EUI is: ");
    Serial.println(modem.deviceEUI());
#endif

    modem.setPort(1);

    int connected = modem.joinOTAA(APPEUI, APPKEY);
    if (!connected) {
#if DEBUG
        Serial.println("Something went wrong; are you indoor? Move near a window and retry");
#endif
        while (1) {
        }
    }
#if DEBUG
    Serial.println("Connected!");
#endif
}

void loop() {
#if DEBUG
    Serial.println("Sending message...");
#endif
    temperature.set(31.42); // Hot in here!

    modem.beginPacket();
    device.toCbor(modem);
    int err = modem.endPacket(false);
    if (err > 0) {
#if DEBUG
        Serial.println("Message sent correctly!");
#endif
    } else {
#if DEBUG
        Serial.println("Error sending message :(");
        Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
        Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
        Serial.print("Error code: ");
        Serial.println(err);
#endif
    }
    delay(1000);
    if (modem.available()) {
        char rcv[64];
        int i = 0;
        while (modem.available()) {
            rcv[i++] = (char)modem.read();
        }
        Serial.print("Received: ");
        for (unsigned int j = 0; j < i; j++) {
            Serial.print(rcv[j] >> 4, HEX);
            Serial.print(rcv[j] & 0xF, HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
    delay(300000); // Wait 5 minutes
}