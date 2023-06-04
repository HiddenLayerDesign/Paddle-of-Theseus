/******************************************************* 
 *  File: RotaryEncoder.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
#ifndef __ROT_ENC_H__
#define __ROT_ENC_H__

#include "Arduino.h"

#define ROT_ENC_MAX 127
#define ROT_ENC_MIN 0


/* Enumeration of Settings that can be controlled by the rotary encoder */
enum rot_enc_state 
{
  ROT_ENC_BLUE,
  ROT_ENC_CYAN,
  ROT_ENC_GREEN,
  ROT_ENC_PURPLE,
  ROT_ENC_RED,
  ROT_ENC_YELLOW,
  ROT_ENC_WHITE,  
  ROT_ENC_ENUM_SIZE
};

/** 
 * LED RGB colors (assumes common anode, as for Sparkfun COM-10982) 
 * Set a given bit to zero to light the LED- so OFF is 0x7 and WHITE is 0x0
 */
#define LED_OFF    0x7 //B111
#define LED_RED    0x6 //B110
#define LED_GREEN  0x5 //B101
#define LED_BLUE   0x3 //B011
#define LED_YELLOW 0x4 //B100
#define LED_PURPLE 0x2 //B010
#define LED_CYAN   0x1 //B001
#define LED_WHITE  0x0 //B000

const int rot_enc_led_color_array[ROT_ENC_ENUM_SIZE] = {LED_BLUE, LED_CYAN, LED_GREEN, LED_PURPLE, LED_RED, LED_YELLOW, LED_WHITE};

/**
 * Set the rotary encoder's built-in LED to one of the values defined at the top of the file 
 * 
 * @param color A byte whose bits correspond to R/G/B. See the definitions in RotaryEncoder.h 
 */
void RotEncSetLED(uint8_t color);

/**
 * Flash the pattern for standard paddle behavior (BLOCKING)
 */
void RotEncStandardPattern(void);

/**
 * Flash the pattern for paddle config mode (BLOCKING)
 */
void RotEncConfigPattern(void);

#endif // __ROT_ENC_H__
