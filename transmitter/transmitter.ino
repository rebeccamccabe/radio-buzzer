// transmitter.ino
//
// Read potentiometer to determine volume and use 
// VirtualWire to transmit the volume when button pressed
// See VirtualWire.h for detailed API docs

// Original author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// Modifications: Rebecca McCabe, 2019

#include <VirtualWire.h>
int button = 8;
int pot = 14;
int led = 13;
int unused [] = {0,1,2,3,4,5,6,7, 9,10,11, 15,16,17,18,19,20,21,22,23};

float minVolume = 110.0;
float maxVolume = 255.0;
float minPotReading = 80.0;
float maxPotReading = 1023.0;
float slope = (maxVolume - minVolume) / (maxPotReading - minPotReading);

void setup()
{
    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec

    // save power by setting unused pins as outputs
    for(i=0;i < sizeof unused/sizeof unused[0]; i++) {
       int pin = unused[i];
       pinMode(pin, OUTPUT);
    }

    pinMode(led, OUTPUT);
    pinMode(button, INPUT);
    pinMode(pot, INPUT);
}

void loop()
{
    int reading = analogRead(pot);
    int vol = minVolume + slope * (reading - minPotReading);
    char buffer[7];
    dtostrf(vol, 7, 0, buffer);
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
