/******************************************************* 
 *  File: Ultrasonic.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

/**.
 * This file is now very barebones because the NewPing library is doing the heavy-lifting for the Ultrasonic firmware 
 */
#define PITCH_BEND_MAX_CM 30 // adjust this for sensitivity range of pitch bend
#define PITCH_BEND_MIN_CM 2 // adjust this for sensitivity range of pitch bend

/**
 * MIDI Pitch bend message accepts a 14-bit twos compliment value, 
 * but we only want to detune like a whammy bar, so we use 12 bits and subtract 2^12 
 * such that max val is 0 and whammy goes to half of the full negative (detune) range
 * 
 * NOTE: use pow(2,13) instead for full range!
 */
#define SCALED_PITCH_BEND(x) (int) (pow(2,12) * x  / PITCH_BEND_MAX_CM) - pow(2,12)  // Do not adjust!

#endif // __ULTRASONIC_H__
