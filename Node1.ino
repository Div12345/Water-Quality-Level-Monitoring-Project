// Adafruit IO Multiple Feed Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
//Analog Input Control
#define p 13 //D7 - high to get Turbidity
#define t 15 //D8 - high to get Temperature

//ULTRASOUND
// defines pins numbers
const int trigPin1 = 2;  //D4
const int echoPin1 = 0;  //D3
const int trigPin2 = 4;  //D2
const int echoPin2 = 5;  //D1
int last1=0,last2=0;

//Temperature
float tempC=0, lasttemp=0;
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port A0 on the NodeMCU
#define analog A0
#define TEMPERATURE_PRECISION 9 // Lower resolution

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(analog);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
int numberOfDevices; // Number of temperature devices found
DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address



// Adafruit IO
// track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds
#define IO_LOOP_DELAY 15000
unsigned long lastUpdate=0;

// set up the feed
AdafruitIO_Feed *tank_1 = io.feed("tank-1");
// set up the feed
AdafruitIO_Feed *tank_2 = io.feed("tank-2");
// set up the feed
AdafruitIO_Feed *temperature = io.feed("temperature");
// set up the feed
AdafruitIO_Feed *turbidity = io.feed("turbidity");

void setup() {
//Analog In Control
pinMode(p, OUTPUT);
pinMode(t, OUTPUT);  
  
  //Temperature
sensors.begin();
// Grab a count of devices on the wire
numberOfDevices = sensors.getDeviceCount();
sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  {
    // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
    sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
  }
  }
  
  //Ultrasound
pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  
// connect to io.adafruit.com
  io.connect();
// wait for a connection
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }
  }

void loop() {
  
  //Adafruit IO
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) 
  {
  // process messages and keep connection alive
  io.run();
   
  //Temperature
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
    // It responds almost immediately. Let's print out the data
    printTemperature(tempDeviceAddress); // Use a simple function to print out the data
  }

  //Turbidity
  read_turbidity();
  

  //Close Analog
  digitalWrite(p, LOW);
  digitalWrite(t,LOW);
  
  //Ultrasounds
  level1();
  delay(1000);
  level2();  
  
  // update timer
  lastUpdate = millis();
  }
}

void level1()
{// Clears the trigPin
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
long duration = pulseIn(echoPin1, HIGH);

// Calculating the distance
int distance= duration*0.034/2;

  // return if the value hasn't changed
  if(distance == last1)
    return;

  // save the current state to the analog feed
  tank_1->save(distance);

  // store last photocell state
  last1 = distance;
  }

void level2()
{// Clears the trigPin
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
long duration = pulseIn(echoPin2, HIGH);

// Calculating the distance
int distance= duration*0.034/2;

  // return if the value hasn't changed
  if(distance == last2)
    return;

  // save the current state to the analog feed
  tank_2->save(distance);

  // store last photocell state
  last2 = distance;
  }  

void printTemperature(DeviceAddress deviceAddress)
{
  digitalWrite(p, LOW);
  digitalWrite(t, HIGH);
  delay(1000);
  tempC = sensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C)
  {
    temperature->save(lasttemp);
    return;
  }
  else
  {
    temperature->save(tempC);
    lasttemp= tempC;  
  }
}

void read_turbidity(){
  digitalWrite(t, LOW);
  digitalWrite(p, HIGH);
  double avg_ntu = 0 ,ntu;
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1024.0);
  ntu = ((-1120.4)*voltage*voltage)+(5742.3*voltage) - 4352.9;
  avg_ntu += (ntu);
  turbidity->save(avg_ntu);
  }
