/******************************************************* 
 *  File: BoardLayout.hpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
#ifndef __TEENSY_BSP_HPP__
#define __TEENSY_BSP_HPP__

#include "Arduino.h"
#include "Preferences.hpp"

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

// Software Reset register and value
#define REG_SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location for Teensy
#define VAL_SCB_AIRCR_RESET (0x05FA0004)

//#define ULTRASONIC_PING // uncomment if using Parallax Ping ultrasonic Sensor
#define INVERT_ROT_POT // uncomment if the rotary encoder turns the wrong way

// Pinout of Teensy
#define PIN_STRUM_1070_INT  0  ///< GPIO interrupt pin for changes on StrumBoard AT42QT1070
#define PIN_STRUM_2120_INT  1  ///< GPIO interrupt pin for changes on StrumBoard AT42QT2120
#define PIN_ROT_LEDR  2  ///< Illuminated Rotary Encoder Red LED
#define PIN_ROT_LEDG  3  ///< Illuminated Rotary Encoder Green LED
#define PIN_ROT_ENC_SW  4  ///< Digital pin 4 for Rotary Encoder Switch
#define PIN_ROT_LEDB  5  ///< Illuminated Rotary Encoder Blue LED
#define PIN_ROT_ENC_C  6  ///< Illuminated Rotary Encoder "C" input for quadrature encoding
#define PIN_ROT_ENC_A  7  ///< Illuminated Rotary Encoder "A" inut for quadrature encoding
#define PIN_ULTRA_TRIG  10  ///< Trigger pin for ultrasonic rangefinder
#define PIN_ULTRA_SENS  11  ///< Sensing pin for ultrasonic rangefinder
#define PIN_TEENSY_LED 13  ///< UNUSED pin that controls Teensy LED
#define PIN_ROT_POT  A1  ///< Analog pin 1 for Rotary Potentiometer
#define PIN_FRET_1070_INT  14  ///< GPIO interrupt pin for changes on FretBoard AT42QT1070
#define PIN_FRET_2120_INT  15  ///< GPIO interrupt pin for changes on FretBoard AT42QT2120

#endif /* __TEENSY_BSP_HPP__ */
