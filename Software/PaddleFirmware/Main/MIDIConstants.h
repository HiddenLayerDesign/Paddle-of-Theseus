/******************************************************* 
 *  File: MIDIConstants.h
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
 
#ifndef __MIDI_CONST_H__
#define __MIDI_CONST_H__

/* MIDI Channel Constants */
const int MIDI_CHANNEL_1  = 0;
const int MIDI_CHANNEL_2  = 1;
const int MIDI_CHANNEL_3  = 2;
const int MIDI_CHANNEL_4  = 3;
const int MIDI_CHANNEL_5  = 4;
const int MIDI_CHANNEL_6  = 5;
const int MIDI_CHANNEL_7  = 6;
const int MIDI_CHANNEL_8  = 7;
const int MIDI_CHANNEL_9  = 8;
const int MIDI_CHANNEL_10 = 9;
const int MIDI_CHANNEL_11 = 10;
const int MIDI_CHANNEL_12 = 11;
const int MIDI_CHANNEL_13 = 12;
const int MIDI_CHANNEL_14 = 13;
const int MIDI_CHANNEL_15 = 14;
const int MIDI_CHANNEL_16 = 15;

/* Generally accepted MIDI Control Change messages */
const int MIDI_CTRL_CHG_MOD_WHEEL  = 0x01;
const int MIDI_CTRL_CHG_MOD_BREATH = 0x02;
const int MIDI_CTRL_CHG_PORTAMENTO = 0x05;
const int MIDI_CTRL_CHG_CHAN_VOL   = 0x07;
const int MIDI_CTRL_CHG_EXPRESSION = 0x0B;
const int MIDI_CTRL_CHG_EFFECT_1   = 0x0C;
const int MIDI_CTRL_CHG_EFFECT_2   = 0x0D;
const int MIDI_CTRL_CHG_LEGATO_FSW = 0x44;

const int FRET_LEN = 29;
const int MIDI_OCTAVE_LEN  = 7;
const int MIDI_NUM_OCTAVES = 4;
const int MIDI_SCALE_LEN = 128;

const int ROOT_NOTE_C = 24;
const int ROOT_NOTE_CSHARP = 25;
const int ROOT_NOTE_D = 26;
const int ROOT_NOTE_DSHARP = 27;
const int ROOT_NOTE_E = 28;
const int ROOT_NOTE_F = 29;
const int ROOT_NOTE_FSHARP = 30;
const int ROOT_NOTE_G = 31;
const int ROOT_NOTE_GSHARP = 32;
const int ROOT_NOTE_A = 33;
const int ROOT_NOTE_ASHARP = 34;
const int ROOT_NOTE_B = 35;

/* Various MIDI Scales- thanks to http://lawriecape.co.uk/theblog/index.php/archives/881 for writing these out */

/* major = W W H W W W H */
const int MAJOR_DELTAS[MIDI_SCALE_LEN] = {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 
                                          50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93, 95, 96,
                                          98, 100, 101, 103, 105, 107, 108, 110, 112, 113, 115, 117, 119, 120, 122, 124, 125, 127, 127, 127, 127, 127, 127, 
                                          127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
                                          127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
                                          
/* minor = W H W W H W W */
const int MINOR_DELTAS[MIDI_SCALE_LEN] = {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48,
                                          50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67, 68, 70, 72, 74, 75, 77, 79, 80, 82, 84, 86, 87, 89, 91, 92, 94, 96, 
                                          98, 99, 101, 103, 104, 106, 108, 110, 111, 113, 115, 116, 118, 120, 122, 123, 125, 127, 127, 127, 127, 127, 127,
                                          127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
                                          127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};

/* mixolydian = W W H W W H W */
const int MIXOLYDIAN_DELTAS[MIDI_SCALE_LEN] = {0, 2, 4, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 22, 24, 26, 28, 29, 31, 33, 34, 36, 38, 40, 41, 43, 45, 46, 48,
                                               50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 70, 72, 74, 76, 77, 79, 81, 82, 84, 86, 88, 89, 91, 93, 94, 96, 
                                               98, 100, 101, 103, 105, 106, 108, 110, 112, 113, 115, 117, 118, 120, 122, 124, 125, 127, 127, 127, 127, 127, 127,
                                               127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
                                               127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};

/* major = W H W W W H W */
const int DORIAN_DELTAS[MIDI_SCALE_LEN] = {0, 2, 3, 5, 7, 9, 10, 12, 14, 15, 17, 19, 21, 22, 24, 26, 27, 29, 31, 33, 34, 36, 38, 39, 41, 43, 45, 46, 48,
                                           50, 51, 53, 55, 57, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 75, 77, 79, 81, 82, 84, 86, 87, 89, 91, 93, 94, 96,
                                           98, 99, 101, 103, 105, 106, 108, 110, 111, 113, 115, 117, 118, 120, 122, 123, 125, 127, 127, 127, 127, 127, 127, 
                                           127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
                                           127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
                                           
/* chromatic = H H H H H H H H H H H H */
const int CHROMATIC_DELTAS[MIDI_SCALE_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
                                          16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 
                                          31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 
                                          46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 
                                          61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 
                                          76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 
                                          91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 
                                          105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 
                                          117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};
#endif /* __MIDI_CONST_H__ */
