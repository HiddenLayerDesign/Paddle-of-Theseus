/******************************************************* 
 *  File: CapTouch.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#include "Arduino.h"
#include "CapTouch.h"

CapTouch::CapTouch(int pin)
{
  _pin = pin;
  cap_touch_array[CAP_TOUCH_ARRAY_LEN] = {0};
  array_idx = 0;
  current_reading = LOW;
}

void CapTouch::Update(void)
{
  current_reading = digitalRead(_pin);
  cap_touch_array[array_idx] = current_reading;
  array_idx = (array_idx + 1) % CAP_TOUCH_ARRAY_LEN;
}

bool CapTouch::GetReading(void)
{
  return current_reading;
}

bool CapTouch::NewNote(void)
{
  int sum = 0;
  for (int i=0; i < CAP_TOUCH_ARRAY_LEN; i++)
  {
    sum += cap_touch_array[i]; 
  }
  return (!sum);
}
