/******************************************************* 
 *  File: TeensyBSP.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
#ifndef __TEENSY_BSP_H__
#define __TEENSY_BSP_H__

#include "Arduino.h"

#define ULTRASONIC_PING // uncomment if using Parallax Ping ultrasonic Sensor
#define INVERT_ROT_POT // uncomment if the rotary encoder turns the wrong way

/* Teensy layout constants */
#define TEENSY_ROT_POT_PIN        15
#define TEENSY_LIN_POT_PIN        14
#define TEENSY_LED_PIN            13

/* the Parallax Ping sensor only has 3 pins, so TRIG_PIN == SENS_PIN */
#ifdef ULTRASONIC_PING
#define TEENSY_ULTRA_TRIG_PIN     12
#define TEENSY_ULTRA_SENS_PIN     12
#else
#define TEENSY_ULTRA_TRIG_PIN     12
#define TEENSY_ULTRA_SENS_PIN     11
#endif /* ULTRASONIC_PING*/

#define TEENSY_CAP_TOUCH0_PIN     10
#define TEENSY_CAP_TOUCH1_PIN     9
#define TEENSY_CAP_TOUCH2_PIN     8
#define TEENSY_CAP_TOUCH3_PIN     7
#define TEENSY_ROT_ENC_PIN_1      6
#define TEENSY_ROT_ENC_PIN_2      5
#define TEENSY_ROT_LEDB           4
#define TEENSY_ROT_ENC_BUTTON_PIN 3
#define TEENSY_ROT_LEDG           2
#define TEENSY_ROT_LEDR           1

#ifdef INVERT_ROT_POT
#define GET_VOLUME(pin) floor((1024 - analogRead(pin)) * 256.0/1024.0)
#else 
#define GET_VOLUME(pin) floor(analogRead(pin) * 256.0/1024.0)
#endif /* INVERT_ROT_POT */

#endif /* __TEENSY_BSP_H__ */
