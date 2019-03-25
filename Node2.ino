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
AdafruitIO_Feed *ph = io.feed("ph");

//pH
#define analog A0
#define Offset 0.00 //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40 //times of collection

int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHArrayIndex = 0;

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

  //pH
  getPH();
  
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
int distance= (duration*0.034/2)/26*100;

  // return if the value hasn't changed
  if(distance == last1)
    return;

  // save the current state to the analog feed
  tank_3->save(distance);

  // store last photocell state
  last1 = distance;
  }



void getPH(void) 
{
  static float pHValue, voltage;
    delay(samplingInterval);
    pHArray[pHArrayIndex++] = analogRead(analog);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset;
    ph->save(pHValue);
}

double avergearray(int* arr, int number) 
{
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  }
  else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min; //arr<min
        min = arr[i];
      }
      else {
        if (arr[i] > max) {
          amount += max; //arr>max
          max = arr[i];
        }
        else {
          amount += arr[i]; //min<=arr<=max
        }
      }
    }
    avg = (double)amount / (number - 2);
  }
  return avg;
}
