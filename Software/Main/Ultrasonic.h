/******************************************************* 
 *  File: Ultrasonic.h
 *    
 *  Author: Seeed Studio Example Code
 *  http://wiki.seeedstudio.com/Ultra_Sonic_range_measurement_module/#software
 *  
 *  Modified by: Chase E Stewart 
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
 

#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include "Arduino.h"

class Ultrasonic
{
  public:
    Ultrasonic(int trig_pin, int sens_pin);
    void distance_measure_blocking(void);
    long microseconds_to_centimeters(void);
  private:
    int _sens_pin; // pin number of Arduino that is connected with SENSE pin of Ultrasonic Ranger.
    int _trig_pin; // pin number of Arduino that is connected with TRIG pin of Ultrasonic Ranger.
    long duration;// the Pulse time received;
};

#endif // __ULTRASONIC_H__
