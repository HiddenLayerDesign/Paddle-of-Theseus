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

#define ROT_ENC_MAX 255
#define ROT_ENC_MIN 0

/* Enumeration of Settings that can be controlled by the rotary encoder */
enum rot_enc_state {
  ROT_ENC_VOLUME,
  ROT_ENC_HYPER,
  ROT_ENC_EFFECT_1,
  ROT_ENC_EFFECT_2,
  ROT_ENC_EFFECT_3,
  ROT_ENC_EFFECT_4,
  ROT_ENC_ENUM_SIZE
};

/* LED RGB colors (assumes common anode, as for Sparkfun COM-10982) */
#define LED_OFF    0x7 //B111
#define LED_RED    0x6 //B110
#define LED_GREEN  0x5 //B101
#define LED_YELLOW 0x4 //B100
#define LED_BLUE   0x3 //B011
#define LED_PURPLE 0x2 //B010
#define LED_CYAN   0x1 //B001
#define LED_WHITE  0x0 //B000


const int rot_enc_led_color_array[ROT_ENC_ENUM_SIZE] = {LED_OFF, LED_WHITE, LED_GREEN, LED_YELLOW, LED_RED, LED_PURPLE};
const char rot_enc_ctrl_change[ROT_ENC_ENUM_SIZE] = {0x14, 0x15, 0x16, 0x17};


void set_rot_enc_led(uint8_t color);

#endif // __ROT_ENC_H__
