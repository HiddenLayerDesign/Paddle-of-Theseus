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

#define PITCH_BEND_MAX_CM 30 // adjust this for sensitivity range of pitch bend
#define PITCH_BEND_MIN_CM 2 // adjust this for sensitivity range of pitch bend

/**
 * Pitch bend accepts a 14-bit twos compliment value, 
 * but we only want to detune like a whammy bar, so we use 12 bits and subtract 2^12 
 * such that max val is 0 and whammy goes to half of negative range
 * 
 * NOTE: use pow(2,13) instead for full range!
 */
#define SCALED_PITCH_BEND(x) (int) (pow(2,12) * x  / PITCH_BEND_MAX_CM) - pow(2,12)  // Do not adjust!

#endif // __ULTRASONIC_H__
