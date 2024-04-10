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
#include "RotaryEncoder.hpp"
#include "BoardLayout.hpp"
#include "Preferences.hpp"

void RotEncSetLED(uint8_t color)
{
  digitalWrite(PIN_ROT_LEDR, color & 0x1);
  digitalWrite(PIN_ROT_LEDG, color & 0x2);
  digitalWrite(PIN_ROT_LEDB, color & 0x4);
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
  for (int i=0; i < LED_PATTERN_NUM_REPEATS; i++)
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
  for (int i=0; i < LED_PATTERN_NUM_REPEATS; i++)
  {
    RotEncSetLED(LED_RED);
    delay(250);  
    RotEncSetLED(LED_WHITE);
    delay(250);  
  }
  RotEncSetLED(LED_OFF);
  delay(2000);  
}

int32_t ProcessRotEnc(int32_t rotEncReading, bool isLeftyFlipped)
{
  int32_t rotEnc = (isLeftyFlipped) ? (-1 * rotEncReading) : rotEncReading ;
  return constrain(rotEnc, ROT_ENC_MIN, ROT_ENC_MAX);
}
