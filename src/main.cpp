#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PinChangeInt.h"
#include "encoder.h"

#include "Air_Quality_Sensor.h"

AirQualitySensor sensor(A3);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void barGraph(unsigned v);
void testdrawchar(void);
void bargraph (uint8_t numero, uint8_t valeur);
void TRACE(const char * texte, uint16_t valeur);
void TRACE(const char * texte, uint16_t valeur1, uint16_t valeur2);
void debitmetre( void );
static uint8_t dbm_ticks = 0;

const uint8_t pwmA   = 3;
const uint8_t pwmB   = 11;
const uint8_t brakeA = 9;
const uint8_t brakeB = 8;
const uint8_t dirA   = 12;
const uint8_t dirB   = 13;

const uint8_t flowW  = 2;

#define CSGN_VAL_MIN  128

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
 // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.  delay(2000);

  testdrawchar();

  encoder.Timer_init();

  pinMode(pwmA, OUTPUT);      pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);    pinMode(brakeB, OUTPUT);
  pinMode(dirA, OUTPUT);      pinMode(dirB, OUTPUT);

  digitalWrite(pwmA, HIGH);   digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);  digitalWrite(brakeB, LOW);
  digitalWrite(dirA, LOW);    digitalWrite(dirB, LOW);

  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);

  pinMode(flowW, INPUT);
  // attachPinChangeInterrupt(4, debitmetre, CHANGE);
  attachInterrupt(flowW, debitmetre, CHANGE);

  if (sensor.init()) 
  {
    Serial.println("Sensor ready.");
  } 
  else 
  {
    Serial.println("Sensor ERROR!");
  }
}

void loop() 
{
  static uint16_t consigne = 0;
  static int dbm_p_entree = 0;
  static unsigned long dbm_tmps = 0;
  static unsigned long qs_tmps = 0;

  if(millis() > qs_tmps)
  {
    int quality = sensor.slope();
    qs_tmps += 10000;

//    Serial.print("Sensor value: ");
//    Serial.println(sensor.getValue());

    if (quality == AirQualitySensor::FORCE_SIGNAL) {
        Serial.println("High pollution! Force signal active.");
    } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
        Serial.println("High pollution!");
    } else if (quality == AirQualitySensor::LOW_POLLUTION) {
        Serial.println("Low pollution!");
    } else if (quality == AirQualitySensor::FRESH_AIR) {
        Serial.println("Fresh air.");
    }
    bargraph(3, (sensor.getValue()) >> 1);
  }

  int dbm_entree = digitalRead(flowW);
  if(dbm_entree != dbm_p_entree)
  {
    unsigned long dbm_delta = millis() - dbm_tmps;
    dbm_p_entree = dbm_entree;
    dbm_tmps = millis();
  
    TRACE("Temps : %d", dbm_delta);
  }


  if (encoder.rotate_flag == 1)
  {
    if (encoder.direct == 0)
    {
      if(consigne < 255)
        consigne += 5;
    }
     else
    {
      if(consigne != 0)
        consigne -= 5;
    }

    TRACE("\ncsgn : %3d", consigne);

    if(consigne > CSGN_VAL_MIN)
    {
      bargraph(0, (consigne - 128) << 1);
      analogWrite(pwmB, consigne);
    }
    else
      analogWrite(pwmB, 0);

    encoder.rotate_flag = 0;
  }

}

void bargraph (uint8_t numero, uint8_t valeur)
{
  uint8_t posx = (numero * 8) + 2;
  uint16_t valx = valeur + valeur + valeur;
  valx = valx + valx + valx + valx + valx;
  valx >>= 6; 

  display.fillRect(66, posx, 60, 3, SSD1306_BLACK);
  if(valx)
    display.fillRect(66, posx, (uint8_t) valx, 3, SSD1306_WHITE);

  display.display();
}

void testdrawchar(void) 
{
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.setCursor(0, 0);     // Start at top-left corner
  display.write("consigne :");

  display.setCursor(0, 8);     // Start at top-left corner
  display.write("courant :");

  display.setCursor(0, 16);     // Start at top-left corner
  display.write(("d‚bit :"));
//  display.write(("d\x82bit :"));

  display.drawRect(64, 0, 64, 7, SSD1306_WHITE);

  display.drawRect(64, 8, 64, 7, SSD1306_WHITE);

  display.drawRect(64, 16, 64, 7, SSD1306_WHITE);

  display.drawRect(64, 24, 64, 7, SSD1306_WHITE);
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

void TRACE(const char * texte, uint16_t valeur)
{
  char txt[30];
  snprintf(txt, 30, texte, valeur);
  Serial.println(txt);
}

void debitmetre( void )
{
  dbm_ticks++;
  TRACE("dbm_tick : %d", dbm_ticks);
}