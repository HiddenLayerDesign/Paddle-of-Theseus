/******************************************************* 
 *  File: Preferences.hpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __PREFERENCES_HPP__
#define __PREFERENCES_HPP__

//#define DEBUG // uncomment to enable printing variables for debug


#define TEENSY_MIN_VOLUME 10

#define ULTRASONIC_PING_PERIOD (unsigned long) (3)

#define MAX_PITCH_BEND_DELTA 1700

#define CONFIG_HOLD_DURATION_MSEC 5000

#define STRUM_BUTTON_DEBOUNCE_MSEC 10

#define ROT_ENC_CTRL_CHANGE 0x16

#define LED_PATTERN_NUM_REPEATS 4

#endif /* __PREFERENCES_HPP__ */
