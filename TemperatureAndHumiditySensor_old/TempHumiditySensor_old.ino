#include <x10rf.h>
#include <rgb_lcd.h>
#include <TH02_dev.h>


/*
  TempSensor.ino
  
  Author: keigezellig
  2016-03-04
  version: 1.0

  Grove temperature sensor that outputs its temperature in degrees Celsius to a RGB lcd display and through 433 mhz radio.
  As a bonus the background color changes when the temperature changes with a nice gradient
  
  * -----------------------------------------------------------------------------------------------
  * "THE APPRECIATION LICENSE" (Revision 0x100):
  * Copyright (c) 2015 M. Joosten
  * You can do anything with this program and its code, even use it to run a nuclear reactor (why should you)
  * But I won't be responsible for possible damage and mishap, because i never tested it on a nuclear reactor (why should I..)
  * If you think this program/code is absolutely great and supercalifragilisticexpialidocious (or just plain useful), just
  * let me know by sending me a nice email or postcard from your country of origin and leave this header in the code.
  * Or better join my efforts to make this program even better :-)
  * See my blog (http://joosten-industries.nl/blog), for contact info
  * ---------------------------------------------------------------------------------------------------
*/


#include "math.h"
#include "RfxFruitProtocol.h"

//Define struct for holding color values
typedef struct rgb rgb;

struct rgb {
   byte    r;
   byte    g;
   byte    b;
};


//Pin definitions
const int RF_TX_PIN = 3;
const int RF_TXLED_PIN = 7;
const int BUTTON_PIN = 6; //optional button for switching the backlight on and off (for example if you use battery power, you can conserve power this way)

//Minimum and maximum temperature (C) used for calculating the background color gradient.
//Color changes from blue at MIN through cyan, green, yellow to red at MAX.
const int MIN = 16;
const int MAX = 31;

//X10RF specific configuration
const int RF_REPEATS = 4;
const int SENSOR_ID_TEMP = 0xFF;
const int SENSOR_ID_HUMIDITY = 0xFE;

//Our rf transmitter and lcd
rgb_lcd lcd;
x10rf rftrx = x10rf(RF_TX_PIN,RF_TXLED_PIN,RF_REPEATS);
RfxFruitProtocol protocol;

unsigned long previousMillis = 0;   


//Interval in ms when to send
const long interval = 5000;           

//Lookup table for color gradient
rgb gradient[100];

void setupGradient()
{
  //Calculate LUT
  
  //Blue -> Cyan
  byte green = 0x00;
  for (byte i=0; i< 25; i++)
  {
    gradient[i].r = 0x00;
    gradient[i].g = green;
    gradient[i].b = 0xFF;
    green +=0x0A;
  }

  //Cyan to greencalc
  byte blue = 0xFF;
  for (byte i=25; i< 50; i++)
  {
    gradient[i].r = 0x00;
    gradient[i].g = 0xFF;
    gradient[i].b = blue;
    blue -= 0x0A;
  }

  //Green to yellow
  byte red = 0x00;
  for (byte i=50; i< 75; i++)
  {
    gradient[i].r = red;
    gradient[i].g = 0xFF;
    gradient[i].b = 0x00;
    red += 0x0A;
  }

  //Yellow to red
  green = 0xFF;
  for (byte i=75; i< 100; i++)
  {
    gradient[i].r = 0xFF;
    gradient[i].g = green;
    gradient[i].b = 0x00;
    green -= 0x0A;
  }
}

void setupRF()
{ 
  rftrx.begin();
}

void setupLcd()
{  
 lcd.begin(16, 2);    
}

void setupTH02()
{
  delay(150);
  TH02.begin();
  delay(100);
}

void setup()
{  
  //pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);        // start serial for output
  setupGradient();
  setupRF();  
  setupLcd();  
  setupTH02();

}

float getTemperature()
{  
  return TH02.ReadTemperature();  
}

float getHumidity()
{
  return TH02.ReadHumidity();  
}

void sendTemperature(float temp, int sensorId)
{    
  //Get rid of the decimal point
  long T = (long)(temp * 100);
  uint32_t encoded = protocol.encode(T,0x00);
  Serial.print("T= ");
  Serial.println(T);
  Serial.println("Encoded: ");
  Serial.println(encoded);
  Serial.println("");
  
  //Send it according to the RFXMeter protocol with value encoded according to the RfxFruit protocol
  rftrx.RFXmeter(sensorId,0,encoded); 
}

void sendHumidity(float h, int sensorId)
{    
  //Get rid of the decimal point
  long hum = (long)(h * 100);
  uint32_t encoded = protocol.encode(hum,0x01);
  Serial.print("H= ");
  Serial.println(hum);
  Serial.println("Encoded: ");
  Serial.println(encoded);
  Serial.println("");

  
  //Send it according to the RFXMeter protocol
  rftrx.RFXmeter(sensorId,0,encoded); 
}


int calculateColorIndexFromTemp(float temp)
{
  //Calculate index of the LUT for a given temperature.
  //Since the LUT is 100 elements, we basically calculate a percentage according to the formula:
  // ((temperature - MIN) / (MAX - MIN)) * 100
  
  //Get rid of decimal point
  int T = (int)(temp * 100);
  int h = MAX * 100;
  int l = MIN * 100;

  int color = ( (float)((T - l)) / (float((h - l)))) * 100;

  //Clip values that are outside the subscript range
  if (color > 99)
  {
    color = 99;
  }

  if (color < 0)
  {
    color = 0;
    
  }

  return color;
}

void printTempAndHumidity(float temp, float humidity, int buttonState)
{  
  lcd.setCursor(0,0);
  lcd.print("Temp=");  
  lcd.print(temp);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Humidity=");  
  lcd.print(humidity);
  lcd.print(" %"); 

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    int color = calculateColorIndexFromTemp(temp);
    lcd.setRGB(gradient[color].r,gradient[color].g, gradient[color].b);
  } 
  else 
  {
    lcd.setRGB(0,0,0);  
  }

  
  delay(500);
}

void loop()
{
  float temp = getTemperature();
  float humidity = getHumidity();

  int buttonState = HIGH; //digitalRead(BUTTON_PIN); 
  printTempAndHumidity(temp, humidity, buttonState);
  unsigned long currentMillis = millis();

  //When interval has elapsed sent temperature
  if (currentMillis - previousMillis >= interval) 
  {
     lcd.setCursor(14,0);    
     lcd.print("*");
     previousMillis = currentMillis;
     sendTemperature(temp,SENSOR_ID_TEMP);
     delay(500);
     sendHumidity(humidity,SENSOR_ID_HUMIDITY);
     lcd.setCursor(14,0);    
     lcd.print(" ");

     
  }  

  
}


