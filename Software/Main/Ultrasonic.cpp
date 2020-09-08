/******************************************************* 
 *  File: Ultrasonic.cpp
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
 
#include "Arduino.h"
#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int trig_pin, int sens_pin)
{
  _trig_pin = trig_pin;
  _sens_pin = sens_pin;
}

/*Begin the detection and get the pulse back signal*/
void Ultrasonic::distance_measure_blocking(void)
{
  pinMode(_trig_pin, OUTPUT);
  digitalWrite(_trig_pin, LOW);
  delay(2);
  digitalWrite(_trig_pin, HIGH);
  delay(5);
  digitalWrite(_trig_pin,LOW);
  pinMode(_sens_pin,INPUT);
  duration = pulseIn(_sens_pin,HIGH);
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microseconds_to_centimeters(void)
{
  return duration/29/2; 
}
