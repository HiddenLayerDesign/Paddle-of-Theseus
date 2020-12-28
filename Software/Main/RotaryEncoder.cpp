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

/**
 * Set the rotary encoder's built-in LED to one of the values defined at the top of the file 
 * 
 * @param color A byte whose bits correspond to R/G/B. See the definitions in RotaryEncoder.h 
 */
void set_rot_enc_led(uint8_t color)
{
  digitalWrite(TEENSY_ROT_LEDR, color & 0x1);
  digitalWrite(TEENSY_ROT_LEDG, color & 0x2);
  digitalWrite(TEENSY_ROT_LEDB, color & 0x4);
}
