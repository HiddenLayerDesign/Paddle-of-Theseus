/******************************************************* 
 *  File: Rotary_Encoder.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  This design is built off of Teensy MIDI example code 
 *     and Seeed studio code for the Ultrasonic Rangefinder
 *     
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
 
#ifndef __ROT_ENC_H__
#define __ROT_ENC_H__

#define ROT_ENC_MAX 255
#define ROT_ENC_MIN 0

/* Enumeration of Settings that can be controlled by the rotary encoder */
enum rot_enc_state {
  ROT_ENC_HYPER,
  ROT_ENC_EFFECT_1,
  ROT_ENC_EFFECT_2,
  ROT_ENC_EFFECT_3,
  ROT_ENC_EFFECT_4,
  ROT_ENC_ENUM_SIZE
};

char rot_enc_ctrl_change[ROT_ENC_ENUM_SIZE] = {0x14, 0x15, 0x16, 0x17};

#endif // __ROT_ENC_H__
