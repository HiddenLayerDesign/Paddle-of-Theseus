/******************************************************* 
 *  File: ADXL345.h
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
#ifndef __ADXL345_H__
#define __ADXL345_H__
 
#include "Arduino.h"

/* ADXL345 Registers */
#define ADXL345_WHOAMI_REG     0x00
#define ADXL345_TAP_THRESH_REG 0x1D
#define ADXL345_OFST_X_REG     0x1E
#define ADXL345_OFST_Y_REG     0x1F
#define ADXL345_OFST_Z_REG     0x20
#define ADXL345_TAP_DUR_REG    0x21


#endif /* __ADXL345_H__ */
