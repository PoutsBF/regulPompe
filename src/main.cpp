#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "encoder.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void barGraph(unsigned v);

const uint8_t pwmA   = 3;
const uint8_t pwmB   = 11;
const uint8_t brakeA = 9;
const uint8_t brakeB = 8;
const uint8_t dirA   = 12;
const uint8_t dirB   = 13;

void setup() 
{
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  encoder.Timer_init();

  pinMode(pwmA, OUTPUT);      pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);    pinMode(brakeB, OUTPUT);
  pinMode(dirA, OUTPUT);      pinMode(dirB, OUTPUT);

  digitalWrite(pwmA, HIGH);   digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);  digitalWrite(brakeB, LOW);
  digitalWrite(dirA, LOW);    digitalWrite(dirB, LOW);

  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);

}

void loop() 
{
  static uint16_t consigne = 0;

  if (encoder.rotate_flag == 1)
  {
    if (encoder.direct == 0)
    {
      if(consigne < 255)
        consigne++;
    }
     else
    {
      if(consigne != 0)
        consigne--;
    }
    char txt[15];
    snprintf(txt, 15, "csgn : %3d", consigne);
    Serial.println(txt);

    display.setCursor(0, 0);
    display.print("csgn : ");
    display.print(consigne);

    analogWrite(pwmA, consigne);
    analogWrite(pwmB, consigne);

    encoder.rotate_flag = 0;
  }
}

/*
void barGraph(unsigned v)
{
  char buf[20];
  unsigned width, w;

  snprintf(buf, sizeof buf, "ADC %u", v);
  width = disp_width - 2;
  w = v * width / 4095;
  display.print(buf);
  display.fillRect(1, 8, w, disp_height - 2);
  display.setColor(BLACK);
  display.fillRect(w, 38, disp_width - 2, disp_height - 2);
  display.fillRect(1, 1, disp_width - 2, 37);
  display.setColor(WHITE);
  display.drawLine(0, 38, disp_width - 2, 38);
  display.drawRect(0, 0, disp_width - 1, disp_height - 1);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_8);
  display.drawString(0, 0, buf);
  display.display();
}
*/