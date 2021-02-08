/******************************************************* 
 *  File: LinearPotentiometer.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#include "LinearPotentiometer.h"
#include "MIDIConstants.h"
#include "TeensyBSP.h"

/**
 * Read the linear potentiometer and use it to get a note in the scale
 * 
 * @return uint8_t Index of note within provided scale;
 */
uint8_t fret_from_lin_pot(void)
{
  int lin_pot_voltage = analogRead(TEENSY_LIN_POT_PIN);
  int constrained_lin_pot = constrain(lin_pot_voltage, LIN_POT_MIN, LIN_POT_MAX);
  return (int) (constrained_lin_pot * FRET_LEN) / LIN_POT_MAX;
}
