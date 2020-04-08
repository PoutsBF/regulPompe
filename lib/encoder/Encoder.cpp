/*
Inspiré de la bibliothèque Seeed Studio (LG)
2020 Stéphane Lepoutère
*/

#include "Encoder.h"
#include <Arduino.h>
#include <TimerOne.h>
#include "PinChangeInt.h"
//TimerOne Timer1;

char ready_msg = 0;

//char direct_f=0;
//char direct_b=0;
void timerIsr();
void isr_encod();

static uint8_t encod_A;
static uint8_t encod_B;

Encoder::Encoder()
{
  encod_A = 6;
  encod_B = 7;

  attachPinChangeInterrupt(encod_A, isr_encod, CHANGE);
  attachPinChangeInterrupt(encod_B, isr_encod, CHANGE);
}

Encoder::Encoder(uint8_t port_encodA, uint8_t port_encodB)
{
  encod_A = port_encodA;
  encod_B = port_encodB;
  attachPinChangeInterrupt(encod_A, isr_encod, CHANGE);
  attachPinChangeInterrupt(encod_B, isr_encod, CHANGE);
}

void Encoder::Timer_init(void)
{
    Timer1.initialize(150000); // set a timer of length 100 microseconds
    Timer1.attachInterrupt(timerIsr); // attach the service routine here
    sei();
}

void Encoder::isr_encod(void)
{
//  static int val_p_encod_A = 0;
  static int val_p_encod_B = 0;
  int val_encod_A = digitalRead(encod_A);
  int val_encod_B = digitalRead(encod_B);

  if(ready_msg == 1)
	  {
	    if(val_encod_A ^ val_p_encod_B)
	    {
	      encoder.direct = 1;     //adjust direction forward
			  encoder.rotate_flag = 1;
	    }
	    else
	    {
	      encoder.direct = 0;     //adjust direction forward
        encoder.rotate_flag = 1;
	    }
      val_p_encod_B = val_encod_B;
      ready_msg = 0;
	  }
}

void timerIsr()
{
    ready_msg = 1;
}

Encoder encoder;
