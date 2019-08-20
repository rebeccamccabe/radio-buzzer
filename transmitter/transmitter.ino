// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
int button = 8;
int pot = 14;
int led = 13;

float minVolume = 110.0;
float maxVolume = 255.0;
float minPotReading = 80.0;
float maxPotReading = 1023.0;
float slope = (maxVolume - minVolume) / (maxPotReading - minPotReading);

void setup()
{
    Serial.begin(9600);	  // Debugging only
    Serial.println("setup");

    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec

    pinMode(led, OUTPUT);
    pinMode(button, INPUT);
    pinMode(pot, INPUT);
}

void loop()
{
    int reading = analogRead(pot);
    Serial.println(reading);
    int vol = minVolume + slope * (reading - minPotReading);
    Serial.println(vol);
    char buffer[7];
    dtostrf(vol, 7, 0, buffer);
    //const char *msg = "hello";
    char *msg = buffer;
    
    bool press = digitalRead(button);
    if(press) {
      digitalWrite(led, true); // Flash a light to show transmitting
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      digitalWrite(led, false);
      delay(50);
    }
}
