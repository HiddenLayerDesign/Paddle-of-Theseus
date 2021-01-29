/******************************************************* 
 *  File: LinearPotentiometer.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __LIN_POT_H__
#define __LIN_POT_H__

#include "Arduino.h"
 
#define LIN_POT_MAX  1024
#define LIN_POT_MIN  0

/**
 * Read the linear potentiometer and use it to get a note in the scale
 * 
 * @return uint8_t Index of un-scaled "fret" on potentiometer;
 */
uint8_t fret_from_lin_pot(void);

#endif /* __LIN_POT_H__ */
