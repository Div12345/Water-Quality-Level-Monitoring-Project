// Adafruit IO Publish & Subscribe Example
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
const int p1 = 5; //D1
const int p2 = 4; //D2
const int p3 = 0; //D3
int over = 0;
// this int will hold the current count for our sketch
int count = 0,lowlimit=100,highlimit=1500;

// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *pump_1 = io.feed("pump-1");
AdafruitIO_Feed *pump_2 = io.feed("pump-2");
AdafruitIO_Feed *pump_3 = io.feed("pump-3");
AdafruitIO_Feed *tank_1 = io.feed("tank-1");
AdafruitIO_Feed *tank_2 = io.feed("tank-2");
AdafruitIO_Feed *tank_3 = io.feed("tank-3");
AdafruitIO_Feed *master = io.feed("override");

void setup() {

  digitalWrite(p1,OUTPUT);
  digitalWrite(p2,OUTPUT);
  digitalWrite(p3,OUTPUT);
  
  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
/*  pump_1->onMessage(handlep1);
  pump_2->onMessage(handlep2);
  pump_3->onMessage(handlep3);
*/
  tank_1->onMessage(handlet1);
  tank_2->onMessage(handlet2);
  tank_3->onMessage(handlet3);
  master->onMessage(handleover);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }

  tank_1->get();
  tank_2->get();
  tank_3->get();
  master->get();
/*pump_1->get();
  pump_1->get();
  pump_1->get();
*/
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

/*  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // after publishing, store the current time
    lastUpdate = millis();
  }
*/
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handlet1(AdafruitIO_Data *data) {
  if((over/100)==1)
  {digitalWrite(p1,LOW);
  pump_1->save(false);
  }
  else
  {int t1=data->toInt();
  
  if((t1<=lowlimit)&&(digitalRead(p1)==0))
  { digitalWrite(p1,HIGH);
  pump_1->save(true);
  }
  
  if((t1>=highlimit)&&(digitalRead(p1)==1))
  { digitalWrite(p1,LOW);
  pump_1->save(false);
  }
}}
void handlet2(AdafruitIO_Data *data) {
  if(((over/10)%10)==1)
  {digitalWrite(p2,LOW);
  pump_2->save(false);
  }
  else
  {int t2=data->toInt();
  
  if((t2<=lowlimit)&&(digitalRead(p2)==0))
  { digitalWrite(p2,HIGH);
  pump_2->save(true);
  }
  
  if((t2>=highlimit)&&(digitalRead(p2)==1))
  { digitalWrite(p2,LOW);
  pump_2->save(false);
  }

}}
void handlet3(AdafruitIO_Data *data) {
  if((over%10)==1)
  {digitalWrite(p1,LOW);
  pump_1->save(false);
  }
  else
  {int t3=data->toInt();
  
  if((t3<=lowlimit)&&(digitalRead(p3)==0))
  { digitalWrite(p3,HIGH);
  pump_3->save(true);
  }
  
  if((t3>=highlimit)&&(digitalRead(p3)==1))
  { digitalWrite(p3,LOW);
  pump_3->save(false);
  }
}}
void handleover(AdafruitIO_Data *data)
{ over = data->toInt();}
