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
//ULTRASOUND
// defines pins numbers
const int trigPin1 = 2;  //D4
const int echoPin1 = 0;  //D3
int last1=0;

// Adafruit IO
// track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds
#define IO_LOOP_DELAY 15000
unsigned long lastUpdate=0;

// set up the feed
AdafruitIO_Feed *tank_3 = io.feed("tank-3");
// set up the feed
AdafruitIO_Feed *turbidity = io.feed("turbidity");


void setup() 
{  
  //Ultrasound
pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  
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

  //Ultrasound
  level1();

  //Turbidity
  read_turbidity();
  
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
  tank_3->save(distance);

  // store last photocell state
  last1 = distance;
  }

void read_turbidity(){
  double avg_ntu = 0 ,ntu;
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1024.0);
  ntu = ((-1120.4)*voltage*voltage)+(5742.3*voltage) - 4352.9;
  avg_ntu += (ntu);
  turbidity->save(avg_ntu);
  }
