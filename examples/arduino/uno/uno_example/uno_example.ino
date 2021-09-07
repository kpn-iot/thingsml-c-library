// Simple example that prints thingsml to serial output.
#include <thingsml.h>

#define SERIAL_BAUD_RATE 9600

void print(String message) {
    Serial.println(message);
}


SenMLPack device;
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
char buf[100] = {0};

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    device.add(temperature);

    temperature.set(21.2);

    device.toJson(buf, 100);

    print(buf);
}

void loop() {
}
