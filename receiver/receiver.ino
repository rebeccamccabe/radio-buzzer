// receiver.ino
//
// Use VirtualWire to receive messages and trigger a buzzer
// See VirtualWire.h for detailed API docs

// Original author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// Modifications: Rebecca McCabe, 2019

#include <VirtualWire.h>
int buzzer=10;
int led = 13;

int unused [] = {0,1,2,3,4,5,6,7,8,9, 12, 14,15,16,17,18,19,20,21,22,23};

void setup()
{
    pinMode(led,OUTPUT);
    pinMode(buzzer,OUTPUT);
    analogWrite(buzzer,LOW);

    // save power by setting unused pins as outputs
    for(i=0;i < sizeof unused/sizeof unused[0]; i++) {
       int pin = unused[i];
       pinMode(pin, OUTPUT);
    }

    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      	int i;
        char word[100];
        digitalWrite(led, true); // Flash a light to show received good message
        
    	// Message with a good checksum received, dump it.
    	
    	for (i = 0; i < buflen; i++)
    	{
            word[i] = toascii(buf[i]);
    	}
         
         int vol = atoi(word);
         analogWrite(buzzer, vol);
         delay(200);
    	
        digitalWrite(led, false);
        analogWrite(buzzer,0);
    }
}
