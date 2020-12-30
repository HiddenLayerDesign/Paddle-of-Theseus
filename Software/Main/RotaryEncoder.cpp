/******************************************************* 
 *  File: RotaryEncoder.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
 
#include "Arduino.h"
#include "RotaryEncoder.h"
#include "TeensyBSP.h"

void RotEncSetLED(uint8_t color)
{
  digitalWrite(TEENSY_ROT_LEDR, color & 0x1);
  digitalWrite(TEENSY_ROT_LEDG, color & 0x2);
  digitalWrite(TEENSY_ROT_LEDB, color & 0x4);
}

void RotEncStandardPattern(void)
{
  RotEncSetLED(LED_BLUE);
  delay(180);  
  RotEncSetLED(LED_PURPLE);
  delay(180);  
  RotEncSetLED(LED_GREEN);
  delay(180);  
  RotEncSetLED(LED_CYAN);
  delay(180);  
  RotEncSetLED(LED_RED);
  delay(180);  
  RotEncSetLED(LED_YELLOW);
  delay(180);  
  RotEncSetLED(LED_WHITE);
  delay(2000);  
}

void RotEncConfigPattern(void)
{
  for (int i=0; i<4; i++)
  {
    RotEncSetLED(LED_GREEN);
    delay(250);  
    RotEncSetLED(LED_YELLOW);
    delay(250);  
  }
  RotEncSetLED(LED_OFF);
  delay(2000);  
}

void RotEncErrorPattern(void)
{
  for (int i=0; i<4; i++)
  {
    RotEncSetLED(LED_RED);
    delay(250);  
    RotEncSetLED(LED_WHITE);
    delay(250);  
  }
  RotEncSetLED(LED_OFF);
  delay(2000);  
}
