#include "SevSeg.h"

SevSeg sevseg;
int val;
int tempUnits; 
int refreshTime = 2000;
unsigned long timer;
float celsius = 0, k = 0;

void setup()
{
  sevseg.Begin(0,2,3,4,5,6,7,8,9,10,11,12,13);
  Serial.begin(9600);
}

void loop()
{
  sevseg.PrintOutput();
  val = analogRead(0);
  k = ( val/1024.0)*5000;
  celsius = k/10;
  tempUnits = celsius;
  unsigned long mils=millis();
  if (mils-timer>=refreshTime) {
    timer=mils;
    //Print teperature to the displays
  sevseg.NewNum(tempUnits,(byte) 2);
  }
  Serial.print("Temperature : ");
  Serial.println(tempUnits);
}
