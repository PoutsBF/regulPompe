/*
Inspiré de la bibliothèque Seeed Studio (LG)
2020 Stéphane Lepoutère
*/

#include <Arduino.h>

#ifndef Encoder_h
#define Encoder_h

class Encoder
{
  private:

  public:
    Encoder();
    Encoder(uint8_t port_encodA, uint8_t port_encodB);
    void Timer_init(void);
    char direct;
    char rotate_flag;
    static void isr_encod(void);
};

extern Encoder encoder;
#endif
