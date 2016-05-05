#include <x10rf.h>
#include <rgb_lcd.h>

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

//Define struct for holding color values
typedef struct rgb rgb;

struct rgb {
   byte    r;
   byte    g;
   byte    b;
};


//Pin definitions
const int TEMP_SENSOR_PIN = 0;
const int RF_TX_PIN = 2;
const int RF_TXLED_PIN = 3;
const int BUTTON_PIN = 6; //optional button for switching the backlight on and off (for example if you use battery power, you can conserve power this way)

//Minimum and maximum temperature (C) used for calculating the background color gradient.
//Color changes from blue at MIN through cyan, green, yellow to red at MAX.
const int MIN = 16;
const int MAX = 31;

//X10RF specific configuration
const int RF_REPEATS = 4;
const int SENSOR_ID = 0x01;

//Our rf transmitter and lcd
rgb_lcd lcd;
x10rf rftrx = x10rf(RF_TX_PIN,RF_TXLED_PIN,RF_REPEATS);

unsigned long previousMillis = 0;        

//Interval in ms when to send
const long interval = 10000;           

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

  //Cyan to green
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

void setup()
{  
  pinMode(BUTTON_PIN, INPUT);
  setupGradient();
  setupRF();  
  setupLcd();  
}

float getTemperature()
{
  int a; 
  float temperature;
  int B=3975;                  //B value of the thermistor
  float resistance;
  a=analogRead(0);
  resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
  temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet&nbsp;;
  return temperature;
}

void sendTemperature(float temp, int sensorId)
{    
  //Round temperature to nearest whole value
  float rounded = round(temp);
  //Send it according to the RFXSensor protocol
  rftrx.RFXsensor(sensorId,'t','t',rounded); 
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

void printTemp(float temp, int buttonState)
{  
  lcd.setCursor(0,0);
  lcd.print("Temp. ");  
  lcd.print(temp);
  lcd.print(" C");  

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

  int buttonState = digitalRead(BUTTON_PIN); 
  printTemp(temp, buttonState);
  unsigned long currentMillis = millis();

  //When interval has elapsed sent temperature
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    sendTemperature(temp,SENSOR_ID);    

  }  

  
}


