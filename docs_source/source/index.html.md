---
title: Library Reference

language_tabs: # must be one of https://git.io/vQNgJ
  - c--arduino: arduino
  - c--mbed: mbed

includes:

search: true
---

# Introduction

Welcome to the KPN ThingsML API documentation! You can use our library for the creation of [senml documents](https://tools.ietf.org/html/draft-ietf-core-senml-13) on embedded devices so you can transport and/or receive data in a uniform way to and from devices using a communication protocol of your choice.  
For an indepth look into what senml is and what it can mean for you, check out [this article](http://www.elastetic.com/wp/2018/05/20/senml-messages/).

# Key features

- Object oriented design. 
- Built in support for [senml's unit registry](https://tools.ietf.org/html/draft-ietf-core-senml-12#section-12.1) 
- Extensible for new data types
- Makes use of (but doesn't restrict to) ThingsML predefined list of record names via ThingsML indices.
- Direct support to read/write in json and cbor format.
- Automatically adjusts record data with respect to base time, base value & base sum.
- The library is optimized for devices with restricted memory requirements (runs on AVR systems with 1KB ram). 

# Supported platforms

The library has been tested on the following platforms with their respective devices:

- Arduino
    - Arduino leonardo (through [the marvin](https://www.kickstarter.com/projects/688158475/marvin-the-lora-development-board/description) lorawan board)
    - Arduino Uno
    - [Arduino MKR WAN 1300](https://store.arduino.cc/arduino-mkr-wan-1300-lora-connectivity-1414)
    - [Arduino MKR GSM 1400](https://store.arduino.cc/arduino-mkr-gsm-1400-1415)
- Mbed OS
    - [STM32 Nucleo-L152RE](https://www.st.com/en/evaluation-tools/nucleo-l152re.html)
- PC (windows/linux via [Bazel](https://bazel.build/))

# Getting Started

## Installation

<blockquote class="lang-specific c--arduino">
<p>Get it from <a href="https://github.com/kpn-iot/thingsml-c-library">github</a> </p>
</blockquote>

<blockquote class="lang-specific c--mbed">
<p>You can import it using this link: <a href="https://os.mbed.com/users/kpniot/code/thingsml/">mbed thingsml library</a>  (click on 'import into compiler) or directly from within the mbed online editor. Search for the library thingsml</p>
</blockquote>

- Get the library. You can find the language appropriate link to the right.
- Installation:
    - *arduino*: 
        - from Library manager (Tools->Manage Libraries...), search for 'ThingsML'
    - mbed: on the mbed online editor:
        - create or open a project
        - right click on the project and select 'import library'
        - if you select from url, use the <a href="https://os.mbed.com/users/kpniot/code/thingsml/">mbed thingsml library</a>
        - if you select: 'from import wizard':
            - search for 'thingsml'
            - double click on the search result which will start the import process.
    

## Usage for LoRaWAN

```c--arduino
#include <thingsml.h>
```

```c--mbed
#include <thingsml.h>
```

- First off, you need to include the library in your sketch or application. So put the example code (in the right side panel) somewhere near the top of your source. 

```c--arduino
SenMLPack device;
```

```c--mbed
SenMLPack device;
Serial pc(USBTX, USBRX);
```

- Next, you have to declare a variable of type 'SenMLPack', which will function as the document that contains all your measurement values. This can be a global or local to a function.

```c--arduino
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);

void setup() {
    Serial.begin(9600);
    device.add(temperature);
}

void loop(){
    int val = analogRead(A1);  
    temperature.set(val);                  
```

```c--mbed
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);

int main() {
    device.add(temperature);
    // check mypin object is initialized and connected to a pin
    if(mypin.is_connected()) {
        printf("mypin is connected and initialized! \n\r");
    }
    mypin.mode(PullNone);     
    while(1) {
        int val = mypin.read();
        temperature.set(val);              
```

- Measurement values are added to the document through objects of the type SenMlRecord. These can also be global, statically declared objects or they can be local to a function. These always need to have a value or sum, but usually you also give records a name and measurement unit. The library contains a set of predefined values for both records a name and measurement unit via the ThingsMLMeasurementIndex enum. Although you are free to pick any record name that you want, this is not the case for the units. This list is fixed via the SenMLUnit enum.

```c--arduino
    device.toJson(Serial);        //print to screen
}
```

```c--mbed
        device.toJson(pc);        //print to screen
        pc.printf("\n\r");
    }
}
```

- finally, you can render the senml json string and print it out. This can be either to a stream or you can render it to a memory buffer.

The resulting code snippet reads the value, stores this measurement in a senml record as temperature in degrees Celsius. The document object renders a json string to the Serial output.

## Usage for m2m

Usage for m2m devices (2G/Lte-m based) is very similair to LoRaWAN with the exception that the data is transmitted using TCP to an HTTP server.
To ease development ThingsML includes a HTTP Post builder, which includes functionality to calculate the needed authentication token. 

```c--arduino
#include <thingsml_http.h>
```

```c--mbed
#include <thingsml_http.h>
```
- To obtain this functionality a different header has to be included.

```c--arduino
SenMLPack device("urn:dev:IMEI:1234:"); // Device name is now required and cannot be omitted like in LoRa.
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
char output[500] = {0}; // 500 characters is big enough for most messages.

void setup() {
    Serial.begin(9600);
    device.add(temperature);
}

void loop(){
    int val = analogRead(A1);  
    temperature.set(val);
    
    int len = ThingsML::httpPost(output, 500, "MYKEY", "1.1.1.1", "/post/path", device);
    Serial.println(output);
}
```

```c--mbed
SenMLPack device("urn:dev:IMEI:1234:"); // Device name is now required and cannot be omitted like in LoRa.
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
Serial pc(USBTX, USBRX);
char output[500] = {0}; // 500 characters is big enough for most messages.

int main() {
    device.add(temperature);
    // check mypin object is initialized and connected to a pin
    if(mypin.is_connected()) {
        printf("mypin is connected and initialized! \n\r");
    }
    mypin.mode(PullNone);     
    while(1) {
        int val = mypin.read();
        temperature.set(val);  
        int len = ThingsML::httpPost(output, 500, "MYKEY", "1.1.1.1", "/post/path", device);
        pc.printf(output);
        pc.printf("\n\r");
    }
}   
```

- Also serilization is slightly different and is only possible using a buffer. This restrictions exists because of the required token. 

## Further usage
Further usage can be found by using the examples packaged with this library.
These can be found in the Arduino IDE under File>Examples>ThingsML.

# Details

The more in depth details of the library are explained below.

## Object oriented: class structure
![class diagram](library_design_c.png)   

The root class for all senml documents is called 'SenMlPack'.  It defines the base name, base unit and base time of the document.  This object can also contain 0, 1 or more SenMlRecords where each record represents a single measurement (or command for actuators).   
In order to declare a base value or base sum, you have to use one of SenMLPack's descendants with the correct data type.  
The library contains a pre-defined SenMlRecord class for the most common data types: string, boolean, double, float, integer and binary. But, you can extend this with your own types through the SenMLRecord template for basic data types such as longlong.  
A SenMLPack can contain all object types as children: anything that descends from SenMLBase can be a child. See [gateways](#gateways) for more info.

## Names and Units
The library defines an enum for all of senml's supported measurement units (as in 'kilogram', 'meter',...). This makes it easier to keep compliance with the [senml specifications](https://tools.ietf.org/html/draft-ietf-core-senml-13) so you don't have to worry about the exact unit symbols: the library takes care of this.  
Similarly, the library also provides an enum with all the ThingsML names that the ThingsML standard supports.  
Although it is possible that you assign your own name to a record, it is recommended to use ThingsML naming convention as this allows data to be addressed in a more semantic manner.  
According to the SenML specifications, all names are optional, so you don't have to declare a base name on the SenMLPack object nor a name for SenMLRecords. This makes it harder though to identify your data. 
In general, it is advisable to specify the name of the device as the base name and the name of the sensor as the record name. Alternatively, you can skip the base name and put both device and sensor name in the record, in this format: `device:sensor`.
The following ThingsML enums with their respective names and units are supported:

Enum Key    |	Name    |	Unit
-------------- | -------------- | --------------
THINGSML_TEMPERATURE   |	temperature |	Cel
THINGSML_HUMIDITY   |	humidity    |	%RH
THINGSML_LATITUDE   |	latitude    |	lat
THINGSML_LONGITUDE  |	longitude   |	lon
THINGSML_ALTITUDE   |	altitude    |	m
THINGSML_POWER  |	power   |	W
THINGSML_PRESSURE   |	pressure    |	Pa
THINGSML_ANGLE  |	angle   |	rad
THINGSML_LENGTH |	length  |	m
THINGSML_BREADTH    |	breadth |	m
THINGSML_HEIGHT |	height  |	m
THINGSML_WEIGHT |	weight  |	kg
THINGSML_THICKNESS  |	thickness   |	m
THINGSML_DISTANCE   |	distance    |	m
THINGSML_AREA   |	area    |	m2
THINGSML_VOLUME |	volume  |	m3
THINGSML_VELOCITY   |	velocity    |	m/s
THINGSML_ELECTRIC_CURRENT   |	electricCurrent |	A
THINGSML_ELECTRIC_POTENTIAL |	electricPotential   |	V
THINGSML_ELECTRIC_RESISTANCE    |	electricResistance  |	Ohm
THINGSML_ILLUMINANCE    |	illuminance |	lx
THINGSML_ACCELERATION_X |	accelerationX   |	m/s2
THINGSML_ACCELERATION_Y |	accelerationY   |	m/s2
THINGSML_ACCELERATION_Z |	accelerationZ   |	m/s2
THINGSML_HEADING    |	heading |	rad
THINGSML_CO_CONCENTRATION   |	COConcentration |	ppm
THINGSML_CO2_CONCENTRATION  |	CO2Concentration    |	ppm
THINGSML_SOUND  |	sound   |	db
THINGSML_FREQUENCY  |	frequency   |	Hz
THINGSML_BATTERY_LEVEL  |	batteryLevel    |	%EL
THINGSML_BATTERY_VOLTAGE    |	batteryVoltage  |	V
THINGSML_RADIUS |	radius  |	m
THINGSML_BATTERY_LEVEL_LOW  |	batteryLevelLow |	/
THINGSML_COMPASS_X  |	compassX    |	T
THINGSML_COMPASS_Y  |	compassY    |	T
THINGSML_COMPASS_Z  |	compassZ    |	T
THINGSML_READ_SWITCH    |	readSwitch  |	/
THINGSML_PRESENCE   |	presence    |	
THINGSML_COUNTER    |	counter |	


<aside class="info">For LoRa devices using KPN Things the device name can be omitted to reduce the payload size, however for M2M devices the name is required identification information and cannot be ommitted when using KPN Things.</aside>

## Associating records with a document

```c--arduino  
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
device.add(temperature);                      
```

```c--mbed
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
device.add(temperature);                      
```

You can add records to the document with the function `add`. This can be done statically (add once at start up and never remove ) for devices that will always send out the same document structure with the same records. Or, you can dynamically add records to the document as your application progresses. A common use case for this method is when the device does not have network connectivity at the moment that the measurement is taken, but instead, takes a number of measurements, and, when a connection is available, uploads all the measurements at once.  This method is also useful to minimize the number of communication packets that a device sends out by grouping multiple measurements into a single data packet.  

```c--arduino  
doc.clear();                      
```

```c--mbed
doc.clear();                        
```

For documents that work with a dynamically sized list of records, you can clear out the list once the data has been sent.  
Alternatively, when SenMlRecords go out of scope or are deleted, they remove themselves automatically from their root document.   

<aside class="warning">You are responsible for destroying objects. If you dynamically created a SenMLRecord, added it to the list and then cleared that list, you must still delete the object yourself in order to avoid memory leaks.</aside>

## Looping over the records

```c--arduino  
SenMLPack device("device_name");
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
device.add(temperature);                      

SenMLBase* item = device.getFirst();
while(item != NULL){
    Serial.println(((SenMLRecord*)item)->getName());
    item = item->getNext();
}
```

```c--mbed
SenMLPack device("device_name");
SenMLDoubleRecord temperature(THINGSML_TEMPERATURE);
device.add(temperature);           

SenMLBase* item = device.getFirst();
while(item != NULL){
    printf(((SenMLRecord*)item)->getName());
    item = item->getNext();
}
```

Internally a linked list is used to store the records that it manages. This helps in minimizing the usage of dynamically allocated memory (important for devices with little available ram).  
To walk over the child list of a SenMLPack, you can use the following functions:

- SenMLPack::getFirst() : retrieve the first item in the list
- SenMLBase::getNext()  : get the next item in the list. When this returns NULL, the end of the list has been reached.

## Gateways

```c--arduino  
SenMLPack doc("gateway");
SenMLPack dev1("dev1");
SenMLPack dev2("dev2");

doc.add(dev1);
doc.add(dev2);

SenMLFloatRecord rec1(THINGSML_TEMPERATURE);
dev1.add(rec1);                       

SenMLStringRecord rec2("text", SENML_UNIT_NONE, "working");
dev2.add(rec2); 
```

```c--mbed
SenMLPack doc("gateway");
SenMLPack dev1("dev1");
SenMLPack dev2("dev2");

doc.add(dev1);
doc.add(dev2);

SenMLFloatRecord rec1(THINGSML_TEMPERATURE);
dev1.add(rec1);                       

SenMLStringRecord rec2("text", SENML_UNIT_NONE, "working");
dev2.add(rec2);                       
```

It is possible to transmit/receive SenMLPack objects that contain other SenMLPack objects. This is used by gateways that work as an intermediate for devices that don't have a direct connection with the outside world or which can't speak senml and need a device that performs a translation between the protocol that they understand and senml.  
Creating such messages is pretty straight forward, just like you add SenMLRecords to a Pack, you can also add SenMLPack objects. A SenMLPack can contain both SenMLRecords and other SenMLPack objects at the same time. This means that the gateway can contain it's own sensor data, besides the information from the other devices.  
This works for sensor values that need to be sent out and for actuators.  

## Rendering

```c--arduino
SenMLPack device("device_name");
SenMLFloatRecord temperature(THINGSML_TEMPERATURE, 23.1);
device.add(temperature);                      

device.toJson(Serial);                                   //render as a json string to the stream

char buffer[120] = {0};   
device.toJson(buffer, sizeof(buffer));                    //render as a json string to a memory buffer
Serial.println(buffer);
 
device.toJson(buffer, sizeof(buffer), SENML_HEX);         //render as a hexified json string to a memory buffer
Serial.println(buffer);

device.toCbor(Serial);                                       //render it as a raw binary data blob directly to stream
device.toCbor(Serial, SENML_HEX);                            //directly rendering HEX values to stream
  
device.toCbor(buffer, sizeof(buffer), SENML_HEX);         //render cbor HEX values to memory 
Serial.println(buffer);
```

```c--mbed
Serial pc(USBTX, USBRX);
SenMLPack device("device_name");
SenMLFloatRecord temperature(THINGSML_TEMPERATURE);
device.add(temperature);                      

device.toJson(pc);                                        //render as a json string to the stream
pc.printf("\n\r");

char buffer[120] = {0};   
device.toJson(buffer, sizeof(buffer));                     //render as a json string to a memory buffer
pc.printf(buffer);
pc.printf("\n\r");

device.toJson(buffer, sizeof(buffer), SENML_HEX);          //render as a hexified json string to a memory buffer
pc.printf(buffer);
pc.printf("\n\r");

device.toCbor(pc);                                        //render it as a raw binary data blob directly to stream
pc.printf("\n\r \n\r");
device.toCbor(pc, SENML_HEX);                             //directly renering HEX values to stream
pc.printf("\n\r \n\r");

device.toCbor(buffer, sizeof(buffer), SENML_HEX);         //render cbor HEX values to memory 
pc.printf(buffer);
pc.printf("\n\r \n\r");
```

If you want to send out your sensor data, the code objects first need to be converted into a format that can easily be transported. This is usually in the form of a json string or binary cbor data.

![rendering steps](render_c.png)  
The rendering engine has the following features and characteristics:

- It can render directly to a stream or to a memory blob.  When rendering to a stream the system will use less memory since nothing is buffered but directly emitted to the stream. 
- The engine can produce:
    - json data, in the form of a string 
    - cbor data, which is a binary format
- The engine can add a final step in the rendering process where the raw data (string or binary) is converted into HEX format. This is useful while rendering directly to a stream that expects hex data, which is the case for many LoRa modules.
- for each value that has to be rendered, all stages are done before moving on to the next value: each value gets converted to json or cbor, hexified and rendered to the output before moving on to the next value. This is also done to save memory.

<aside class="info">HEX values can be formatted in many different ways. The library can currently produce a layout used by microchip lora modems, which is in the form of: 'ABF123'. If the format that you need isn't supported, you will have to perform this final step yourself: first render to a memory blob, then walk over each value and convert it into HEX in the way that you need.</aside>

## Decoding

```c--arduino  
void setTemp(int value){
    Serial.println("set the temp of the boiler to  %i \r\n", value);
}

void onActuator(const char* device, const char* record, const void* value, int valueLength, SenMLDataType dataType)
{
    Serial.println("for unknown records");
    printData(device, record, value, valueLength, dataType);
}

SenMLPack device("device_name", onActuator);
SenMLIntActuator temperature(THINGSML_TEMPERATURE, setTemp);

void setup(){
    Serial.begin(57600);
    senMLSetLogger(Serial);
    device.add(temperature);   
}

void loop(){
    const char * buffer = "[{\"i_\":-24,\"v\":23}]";
    device.fromJson(buffer);
    if(Serial.available()) {
        device.fromCbor(Serial, SENML_HEX);
    }
}                      
```

```c--mbed
Serial pc(USBTX, USBRX);

void setTemp(int value){
    pc.printf("set the temp of the boiler to  %i \r\n", value);
}

void onActuator(const char* device, const char* record, const void* value, int valueLength, SenMLDataType dataType)
{
    pc.printf("for unknown records");
    printData(device, record, value, valueLength, dataType);
}

//use an interrupt callback so we only start to parse when data is available.
//the Serial.readable() function behaves funky and returns 1 even if there is no char available.
void serialDataCallback() {
    doc.fromCbor(pc, SENML_HEX);               //will block until a full message has arrived.
}

SenMLPack device("device_name", onActuator);
SenMLIntActuator temperature(THINGSML_TEMPERATURE, setTemp);

int main() {
    senMLSetLogger(pc);
    device.add(temperature);   
    pc.attach(&serialDataCallback);                 // attach pc ISR for reading from stream
    while(1) {
        const char * buffer = "[{\"i_\":-24,\"v\":23}]";
        device.fromJson(buffer);
        pc.printf("done \r\n \r\n");
    }
}                     
```

Extracting the appropriate information out of senml objects and converting it into the proper format so that the information can be used to drive actuators, can be a bit tedious on embedded devices. The senml library can help you with this so that it becomes easy to send senml messages to your device as actuator commands (send instructions to your devices).  It provides a parsing engine that can handle both json and cbor senml data.  
To process a senml message and retrieve the values, you can use the 'fromJson' or 'fromCbor' functions.  The values found in the message get passed to your application by means of callback functions that you attach to the SenMlPacket object and/or the SenMlRecords.  
If you have a static list of records in your document, you can declare all the objects once, at the beginning, just like for rendering. The only difference here is that you have to attach a callback to each record for which you want to receive events.  During parsing, each callback will be executed when the record is found in the data.   
If your device will receive a dynamic list of records or you want to have a 'catch-all' for unknown records found in the message, than you should attach a callback to the root SenmlPack document. This function gets called for every record found in the data that can't be passed on to a known SenMLRecord. Besides the actual value, which is passed on as a generic void pointer, you also receive the name of the device, the name of the record and the data type so that your application can figure out what it should do for the specified data.    
  

<aside class="warning">It is up to you to determine if all expected records were present in the data or not.  The library does not check for this, but only passes on the values it can find to your applications.</aside>

<aside class="info">You can also send messages to gateways. If you attached SenMLPack objects to the gateway object, then the callbacks of the appropriate device will be called. If no record can be found for the device, but the device is known, then the callback of the device is raised. If record nor device can be found, then the callback of the gateway is used.</aside>

<aside class="info">SenMLPack object that just parsed some incoming data can be used again to render output. This can be useful to send 'ack' messages indicating that the data was correctly processed and the actuators have been changed to the new values.</aside>

## implementing your own record types
It is possible to create your own, custom SenMlRecord classes. This can be used to provide support for more complex data types than already available in the library.  For instance, if you would like to work with a single SenMLRecord to represent location info (lat/lng/alt instead of 3 individual ones, than you could create a new class that inherits from the SenMlRecord that re-implement the rendering and parsing functions.  
For the C++ version of the library, it can also be useful to add support for more basic data types such as longlong. This can easily be done through the SenMLRecordTemplate<T> class which already implements a lot of the functionality needed for records that wrap a basic data type. You can check out some of the existing implementations, such as SenMLIntRecord for inspiration.