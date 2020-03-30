#include <thingsml.h>

#define DEBUG
#define SERIAL_BAUD_RATE 9600
#define LED_PIN 13

#define APPLICATION_PORT 1 // Port to assign the type of lora data (any port can be used between 1 and 223)

#define MICROCHIP_BAUD_RATE 57600
#define MICROCHIP_POWER_PORT 6 // Note that an earlier version of the Marvin doesn't support seperate power to RN2483
#define MICROCHIP_RESET_PORT 5

/**
 * Configuration
 */
const String DevEUI = "--fill--deveui--";
const String AppEUI = "--fill--appeui--";
const String AppKey = "--fill--appkey--";

bool ForceReconfigureMicrochip = false; // To always configure MAC

void print(String message) {
    Serial.println(message);
}

void initMicrochip() {
    String currentDevEui;
    int channelId;

    Serial1.begin(MICROCHIP_BAUD_RATE);

    pinMode(MICROCHIP_POWER_PORT, OUTPUT);
    digitalWrite(MICROCHIP_POWER_PORT, HIGH);

    // Disable reset pin
    pinMode(MICROCHIP_RESET_PORT, OUTPUT);
    digitalWrite(MICROCHIP_RESET_PORT, HIGH);

    // reconfigure LoRa module from memory: restore band, deveui, appeui, appkey, nwkskey, appskey, devaddr, ch list
    sendToMicrochip("sys reset");
    readLineBlockingFromMicrochip();

    // Check if the saved devaddr is different from the desired devaddr. If so, reconfigure.
    if (!ForceReconfigureMicrochip) {
        sendToMicrochip("mac get deveui");
        currentDevEui = readLineBlockingFromMicrochip();
        if (currentDevEui != DevEUI) {
            ForceReconfigureMicrochip = true;
        }
    }

    if (ForceReconfigureMicrochip) {
        sendToMicrochip("mac reset 868");
        readLineBlockingFromMicrochip();

        sendToMicrochip("mac set deveui " + DevEUI);
        readLineBlockingFromMicrochip();

        sendToMicrochip("mac set appeui " + AppEUI);
        readLineBlockingFromMicrochip();

        sendToMicrochip("mac set appkey " + AppKey);
        readLineBlockingFromMicrochip();

        // default SF12 on RX2
        sendToMicrochip("mac set rx2 0 869525000");
        readLineBlockingFromMicrochip();

        sendToMicrochip("mac save");
        readLineBlockingFromMicrochip();
    } else {
#ifdef DEBUG
        print("Configuration correct!");
#endif
    }

    // full power (14dBm)
    sendToMicrochip("mac set pwridx 1");
    readLineBlockingFromMicrochip();

    // default SF7
    sendToMicrochip("mac set dr 5");
    // For SF12
    // sendToMicrochip("mac set dr 1");
    readLineBlockingFromMicrochip();

    // ADR on
    sendToMicrochip("mac set adr on");
    readLineBlockingFromMicrochip();

    bool joinSuccess = false;
    unsigned int joinTries = 0;
    while (!joinSuccess) {
        if ((joinTries++) > 0) {
#ifdef DEBUG
            print("Join failed. Retry in 30 seconds.");
#endif
            delay(30000);
        }

        sendToMicrochip("mac join otaa");
        if (readLineBlockingFromMicrochip() == "ok") {
            joinSuccess = (readLineBlockingFromMicrochip() == "accepted");
        } else {
            joinSuccess = false;
        }
    }

#ifdef DEBUG
    print("Microchip configured");
#endif
}

void sendToMicrochip(String cmd) {
    sendToMicrochip(cmd, true);
}

void sendToMicrochip(String cmd, bool doEcho) {
    Serial1.println(cmd);
#ifdef DEBUG
    if (doEcho) {
        print(" > " + cmd);
    }
#endif
}

bool sendPayload(bool confirmed, int applicationPort, byte *payload, byte payloadLength) {
    String str = "";
    char c[2];
    for (int i = 0; i < payloadLength; i++) {
        sprintf(c, "%02x", payload[i]);
        str += c;
    }
    return sendPayload(confirmed, applicationPort, str);
}

/*
 * Send a payload
 *
 * @param confirmed - wether the frame should be confirmed, or unconfirmed
 * @param applicationPort - the application port on which to send the payload
 * @param payload - the payload to be send, in hexadecimal representation
 *
 * @return whether the payload has successfully been transmitted
 */
bool sendPayload(bool confirmed, int applicationPort, String payload) {
    String type, str, response;

    type = (confirmed) ? "cnf" : "uncnf";
    str = "mac tx " + type + " " + String(applicationPort) + " " + payload;
    sendToMicrochip(str);
    if (readLineBlockingFromMicrochip() == "ok") {
        response = readLineBlockingFromMicrochip();
        if (response == "mac_err" || response == "invalid_data_len") {
            return false;
        }
        return true;
    }
    return false;
}

/*
 * Default to echo the string received
 */
String readLineBlockingFromMicrochip() {
    return readLineBlockingFromMicrochip(true);
}

String readLineBlockingFromMicrochip(bool doEcho) {
    char c;
    String in;

    while (!Serial1.available())
        ;
    while (Serial1.available()) {
        c = Serial1.read();
        if (c != '\n' && c != '\r') {
            in += c;
        }

        if (c == '\n') {
            break;
        }
        delay(1);
    }
#ifdef DEBUG
    if (doEcho) {
        print(" < " + in);
    }
#endif
    return in;
}

SenMLPack device;
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);

void setup() {
    pinMode(LED_PIN, OUTPUT); // Initialize LED port
    digitalWrite(LED_PIN, HIGH);

#ifdef DEBUG
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial)
        ;
    print("Serial port initialised");
#endif

    device.add(temperature);

    initMicrochip();
    digitalWrite(LED_PIN, LOW);
}

char buf[100] = {0};

void loop() {
    device.toJson(buf, 100, SENML_HEX);
    sendPayload(false, 1, buf);
    delay(300000);
}
