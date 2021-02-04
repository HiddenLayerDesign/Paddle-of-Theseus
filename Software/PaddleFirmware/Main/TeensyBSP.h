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
#include "Preferences.h"

/**
 * Silence all printouts if not in DEBUG
 * 
 * NOTE: Set DEBUG definition in Preferences.h
 */
#ifdef DEBUG
#define DEBUG_PRINTLN(x)  Serial.println(x)
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN_HEX(x) Serial.println (x, HEX)
#define DEBUG_PRINT_HEX(x) Serial.print (x, HEX)
#define DEBUG_PRINT_DEC4(x) Serial.print((float) x, 4)
#define DEBUG_PRINT_DEC3(x) Serial.print((float )x, 3)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN_HEX(x) 
#define DEBUG_PRINT_HEX(x)
#define DEBUG_PRINT_DEC4(x) 
#define DEBUG_PRINT_DEC3(x)
#endif /* DEBUG */

#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location for Teensy

//#define ULTRASONIC_PING // uncomment if using Parallax Ping ultrasonic Sensor
#define INVERT_ROT_POT // uncomment if the rotary encoder turns the wrong way

/* Teensy layout constants */
#define TEENSY_ROT_POT_PIN        1 // Analog A1
#define TEENSY_LIN_POT_PIN        0 // Analog A0

/** 
 * Teensy SDA and SCL pins are just assumed + used by the Wire library, so these #defines are unused
 * However, as a reference, the SDA will always be pin D18 and SCL will always be pin D19 for TeensyLC and Teensy 3/4
 */
#define TEENSY_SDA_PIN            18
#define TEENSY_SCL_PIN            19

#define TEENSY_LED_PIN            13

/* the Parallax Ping sensor only has 1 I/O pin, so TRIG_PIN == SENS_PIN */
#ifdef ULTRASONIC_PING
#define TEENSY_ULTRA_TRIG_PIN     12
#define TEENSY_ULTRA_SENS_PIN     12
#else
#define TEENSY_ULTRA_TRIG_PIN     11
#define TEENSY_ULTRA_SENS_PIN     12
#endif /* ULTRASONIC_PING */

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

#endif /* __TEENSY_BSP_H__ */
