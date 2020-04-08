/*
 2012 Copyright (c) Seeed Technology Inc.

 Author: LG
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc.,51 Franklin St,Fifth Floor, Boston, MA 02110-1301 USA

*/
#ifndef Encoder_h
#define Encoder_h

//#include<TimerOne.h>

class Encoder
{
  private:
    uint8_t encod_A;
    uint8_t encod_B;

  public:
    Encoder();
    Encoder(uint8_t port_encodA, uint8_t port_encodB);
    void Timer_init(void);
    char direct;
    char rotate_flag;
};

extern Encoder encoder;
#endif