#include <SoftwareSerial.h> // for UART communication with the Quectel BG96 module
#include <thingsml_http.h>

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


#define HTTP_HOST       "m.m"
#define HTTP_IP         "10.151.236.157"
#define HTTP_PATH       "/ingestion/m2m/senml/v1"
#define HTTP_PORT       80


#define DEBUG true

SenMLPack device(DEVICE_URN);
SenMLDoubleRecord temperature(SENML_NAME_TEMPERATURE, SENML_UNIT_DEGREES_CELSIUS);
char buff[400] = {0};

SoftwareSerial BG96(7, 8);
const int PWR = 6;
const int RST = 5;
int x, y, z;
char RXchar;
int receive_done = 0;
String Data = "";


void setup() {
    Serial.begin(9600);
    while (!Serial) {
    }

    Serial.write("starting script\r\n");

    pinMode(PWR, OUTPUT);
    pinMode(RST, OUTPUT);

    Init();

    Serial.write("\r\n");
    Serial.write("start of script\r\n");

    /////////////////BASIC DATA/////////////////
    BG96.write("AT\r");
    ShowSerialData();
    BG96.write("AT+GMM\r");
    ShowSerialData();
    BG96.write("AT+GMR\r");
    ShowSerialData();
    BG96.write("AT+QAPPVER\r");
    ShowSerialData();

    //////////////////NETWORK SETTINGS/////////////////////
    BG96.write("AT+CFUN=0\r");
    ShowSerialData();
    BG96.write("AT+QCFG=\"BAND\",1,80000,1,1\r");
    ShowSerialData();
    BG96.write("AT+QCFG=\"NWSCANMODE\",3,1\r");
    ShowSerialData();

    // Network type scan sequence
    // 00 Automatic (LTE Cat M1  LTE Cat NB1  GSM)
    // 01 GSM
    // 02 LTE Cat M1
    // 03 LTE Cat NB1
    BG96.write("AT+QCFG=\"NWSCANSEQ\",020102,1\r");
    ShowSerialData();

    // Select possible LTE CAT modes.
    // 0 LTE Cat M1
    // 1 LTE Cat NB1
    // 2 LTE Cat M1 and Cat NB1
    BG96.write("AT+QCFG=\"IOTOPMODE\",0,1\r");
    ShowSerialData();

    // Set Phone Functionality (0 = min, 1 = full, 4 = disable rf)
    BG96.write("AT+CFUN=1\r");
    ShowSerialData();

    /////////////////CONNECTING TO NETWORK///////////////////
    delay(200);
    BG96.write("AT+CIMI\r"); // Request International Mobile Subscriber Identity (IMSI)
    ShowSerialData();
    BG96.write(
        "AT+CGDCONT=1,\"IP\",\"" + APN + "\"\r"); // +CGDCONT:
                                                        // <cid>,<PDP_type>,<APN>,<PDP_addr>,<data_comp>,<head_comp>[…]
    ShowSerialData();
    delay(200);
    BG96.write("AT+COPS=1,2,\"20408\",8\r"); // +COPS: <mode>[,<format>[,<oper>][,0 = gsm, 8 = cat m1]]
    ShowSerialData();
    delay(1000);
    BG96.write("AT+CSQ\r");
    ShowSerialData();
    BG96.write("AT+CSQ\r");
    ShowSerialData();
    BG96.write("AT+QNWINFO\r");
    ShowSerialData();

    //////////////SENDING DATA//////////////////////////////
    BG96.write("AT+QIACT=1\r"); // Activate PDP context
    ShowSerialData();
    BG96.write("AT+QIACT?\r");
    ShowSerialData();
    // context,internal_id,TCP|UDP,REMOTE_IP,REMOTE_PORT,0,mode
    BG96.write("AT+QIOPEN=1,0,\"TCP\",\"" + HTTP_IP + "\",80,0,1\r");
    ShowSerialDataOpen();
    BG96.write("AT+QNWINFO\r");
    ShowSerialData();
    // internal_id, Data Length
    int len = ThingsML::httpPost(buff, 400, DEVICE_KEY, HTTP_HOST, HTTP_PATH, device);
    BG96.write("AT+QISEND=0," + String(len) + "\r"); 
    ShowSerialDataOpen();
    Serial.write("transmit data:");

    BG96.write(buff);

    sleep(500);
    BG96.write("AT+QIRD=0\r"); // Read data
    ShowSerialData();
    BG96.write("AT+QICLOSE=0\r"); // Close connection
    ShowSerialData();
    BG96.write("AT+QIDEACT=1\r"); // Deactivate PDP context
    ShowSerialData();
    delay(500);

    ///////////////ENTERING IN PSM////////////////////////
    BG96.write("AT+QCFG=\"psm/enter\",1\r"); // Enter power saving mode
    ShowSerialData();
    BG96.write("AT+CPSMS=1,,,\"10100001\",\"00000001\"\r"); // Configure power saving mode
    ShowSerialData();

    ShowSerialData();
}

void loop() {
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void Init() {
    BG96.begin(9600);
    Startup();

    BG96.write("AT+IPR=9600\r");
    delay(200);
    ShowSerialData1(1);

    BG96.end();
    delay(100);
    BG96.begin(9600);

    BG96.write("AT+CPSMS=0\r");
    ShowSerialData1(1);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void Startup() {
    BG96.write("AT\r");

    // if no response
    if (ShowSerialData1(1) == 0) {
        BG96.end();
        BG96.begin(115200);
        BG96.write("AT\r");

        // if no response
        if (ShowSerialData1(1) == 0) {
            Serial.write("TIMEOUT -> RST \n\r");
            digitalWrite(RST, HIGH);
            delay(200);
            digitalWrite(RST, LOW);
            Serial.write("RST done \n\r");

            // if no response
            if (ShowSerialData1(20) == 0) {
                Serial.write("TIMEOUT ->PWR\n\r");
                digitalWrite(PWR, HIGH);
                delay(500);
                digitalWrite(PWR, LOW);
                Serial.write("PWR done\n\r");

                ShowSerialData1(10);
            }
        }
    }
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

int ShowSerialData() {
    receive_done = 0;
    x = 0;

    while (receive_done == 0) {
        if (BG96.available() != 0) {
            RXchar = BG96.read();
            Serial.write(RXchar);
            Data.concat(RXchar);

            // ERROR
            if (Data[x - 2] == 'R' && Data[x - 1] == 'O' && Data[x] == 'R') {
                receive_done = 1;
                Data = "";
            }

            // OK
            if (Data[x - 1] == 'O' && Data[x] == 'K') {
                receive_done = 1;
                Data = "";
            }

            // RDY
            if (Data[x - 2] == 'R' && Data[x - 1] == 'D' && Data[x] == 'Y') {
                receive_done = 1;
                Data = "";
            }

            x++;
        }
    }
    Serial.write("\n");
    return receive_done;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void ShowSerialDataOpen() {
    receive_done = 0;
    x = 0;

    while (receive_done == 0) {
        if (BG96.available() != 0) {
            RXchar = BG96.read();
            Serial.write(RXchar);
            Data.concat(RXchar);

            // ERROR
            if (Data[x - 2] == 'R' && Data[x - 1] == 'O' && Data[x] == 'R') {
                receive_done = 1;
                Data = "";
            }

            // OPEN
            if (Data[x - 3] == '0' && Data[x - 2] == ',' && Data[x - 1] == '0' && Data[x] == '\r') {
                receive_done = 1;
                Data = "";
            }

            // POST
            if (Data[x - 3] == ',' && Data[x - 2] == '3' && Data[x - 1] == '4' && Data[x] == '1') {
                receive_done = 1;
                Data = "";
            }

            // POST
            if (Data[x - 3] == ',' && Data[x - 2] == '3' && Data[x - 1] == '4' && Data[x] == '1') {
                receive_done = 1;
                Data = "";
            }

            // POST
            if (Data[x - 3] == ',' && Data[x - 2] == '2' && Data[x - 1] == '0' && Data[x] == '0') {
                receive_done = 1;
                Data = "";
            }

            // CONNECT
            if (Data[x - 2] == 'E' && Data[x - 1] == 'C' && Data[x] == 'T') {
                receive_done = 1;
                Data = "";
            }

            // QIOPEN reponse
            if (Data[x - 2] == '0' && Data[x - 1] == ',' && Data[x] == '5') {
                receive_done = 1;
                Data = "";
            }

            x++;
        }
    }
    Serial.write("\n");
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

int ShowSerialData1(int value) {
    int val;
    x = 0;
    y = 0;
    z = 0;
    val = 0;
    receive_done = 0;

    while (receive_done == 0) {
        if (BG96.available() != 0) {
            RXchar = BG96.read();
            Data.concat(RXchar);

            if (Data[x - 1] == 'O' && Data[x] == 'K') // OPEN
            {
                val = 1;
                Data = "";
                receive_done = 1;
                Serial.write("reponse was OK\n\r");
            }

            if (Data[x - 2] == 'R' && Data[x - 1] == 'D' && Data[x] == 'Y') // OPEN
            {
                val = 1;
                Data = "";
                receive_done = 1;
                Serial.write("reponse was RDY\n\r");
            }

            x++;
        }

        if (y == 32767) {
            y = 0;
            if (z == value) {
                receive_done = 1;
                val = 0;
            }
            z++;
        }
        y++;
    }
    return val;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
