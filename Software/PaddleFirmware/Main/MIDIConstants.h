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

const int MIDI_OCTAVE_LEN  = 12;
const int MIDI_NUM_OCTAVES = 5;
const int MIDI_SCALE_LEN = MIDI_OCTAVE_LEN * MIDI_NUM_OCTAVES;

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
const int MAJOR_DELTAS[MIDI_SCALE_LEN] = {0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24,  
                                          26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 
                                          50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72};
/* minor = W H W W H W W */
const int MINOR_DELTAS[MIDI_SCALE_LEN] = {0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24,
                                          26, 27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48,
                                          50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67, 68, 70, 72};
/* major = W W H W W H W */
const int MIXOLYDIAN_DELTAS[MIDI_SCALE_LEN] = {0, 2, 4, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 22, 24,
                                               26, 28, 29, 31, 33, 34, 36, 38, 40, 41, 43, 45, 46, 48, 
                                               50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 70, 72};
/* major = W H W W W H W */
const int DORIAN_DELTAS[MIDI_SCALE_LEN] = {0, 2, 3, 5, 7, 9, 10, 12, 14, 15, 17, 19, 21, 22, 24,
                                          26, 27, 29, 31, 33, 34, 36, 38, 39, 41, 43, 45, 46, 48,
                                          50, 51, 53, 55, 57, 58, 60, 62, 63, 65, 67, 69, 70, 72};

#endif /* __MIDI_CONST_H__ */
