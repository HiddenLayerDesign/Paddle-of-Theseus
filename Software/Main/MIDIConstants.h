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
const int MIDI_CTRL_CHG_EXPRESSION = 0x0B;
const int MIDI_CTRL_CHG_EFFECT_1   = 0x0C;
const int MIDI_CTRL_CHG_EFFECT_2   = 0x0D;
const int MIDI_CTRL_CHG_LEGATO_FSW = 0x44;

/* Pitch Bend limits*/
const int MIDI_PBEND_MAX  = 72;
const int MIDI_PBEND_MIN  = 48;

const int SCALE_LEN = 32;
/* Various MIDI Scales- thanks to http://lawriecape.co.uk/theblog/index.php/archives/881 for writing these out */
const int BI_YU_SCALE[SCALE_LEN]               = {24,27,31,34,36,39,43,46,48,51,55,58,60,63,67,70,72,75,79,82,84,87,91,94,96,99,103,106,108,111,115,118};
const int BLUES_SCALE[SCALE_LEN]               = {41,42,43,46,48,51,53,54,55,58,60,63,65,66,67,70,72,75,77,78,79,82,84,87,89,90,91,94,96,99,101,102};
const int BLUES_DIM_SCALE[SCALE_LEN]           = {48,49,51,52,54,55,56,58,60,61,63,64,66,67,68,70,72,73,75,76,78,79,80,82,84,85,87,88,90,91,92,94};
const int DORIAN_SCALE[SCALE_LEN]              = {25,27,30,32,34,37,39,42,44,46,49,51,54,56,58,61,63,66,68,70,73,75,78,80,82,85,87,90,92,94,97,99};
const int FULL_MINOR_SCALE[SCALE_LEN]          = {51,53,55,56,57,58,59,60,62,63,65,67,68,69,70,71,72,74,75,77,79,80,81,82,83,84,86,87,89,91,92,93};
const int HARMONIC_MAJ_SCALE[SCALE_LEN]        = {44,47,48,50,52,53,55,56,59,60,62,64,65,67,68,71,72,74,76,77,79,80,83,84,86,88,89,91,92,95,96,98};
const int HAWAIIAN_SCALE[SCALE_LEN]            = {39,43,45,47,48,50,51,55,57,59,60,62,63,67,69,71,72,74,75,79,81,83,84,86,87,91,93,95,96,98,99,103};
const int IONIAN_SHARP_5_SCALE[SCALE_LEN]      = {45,47,48,50,52,53,56,57,59,60,62,64,65,68,69,71,72,74,76,77,80,81,83,84,86,88,89,92,93,95,96,98};
const int JAZZ_MINOR_SCALE[SCALE_LEN]          = {45,47,48,50,51,53,55,57,59,60,62,63,65,67,69,71,72,74,75,77,79,81,83,84,86,87,89,91,93,95,96,98};
const int LYDIAN_SCALE[SCALE_LEN]              = {45,47,48,50,52,54,55,57,59,60,62,64,66,67,69,71,72,74,76,78,79,81,83,84,86,88,90,91,93,95,96,98};
const int MAJOR_SCALE[SCALE_LEN]               = {43,45,48,50,51,52,54,55,57,60,62,63,64,66,67,69,72,74,75,76,78,79,81,84,86,87,88,90,91,93,96,98};
const int MIXOLYDIAN_SCALE[SCALE_LEN]          = {45,46,48,50,52,53,55,57,58,60,62,64,65,67,69,70,72,74,76,77,79,81,82,84,86,88,89,91,93,94,96,98};
const int ORIENTAL_SCALE[SCALE_LEN]            = {45,46,48,49,52,53,54,57,58,60,61,64,65,66,69,70,72,73,76,77,78,81,82,84,85,88,89,90,93,94,96,97};
const int SUPER_LOCRIAN_SCALE[SCALE_LEN]       = {44,46,48,49,51,52,54,56,58,60,61,63,64,66,68,70,72,73,75,76,78,80,82,84,85,87,88,90,92,94,96,97};
const int VERDI_ENIGMATIC_ASC_SCALE[SCALE_LEN] = {46,47,48,49,52,54,56,58,59,60,61,64,66,68,70,71,72,73,76,78,80,82,83,84,85,88,90,92,94,95,96,97};
const int ZIRAFKEND_SCALE[SCALE_LEN]           = {48,50,51,53,55,56,57,59,60,62,63,65,67,68,69,71,72,74,75,77,79,80,81,83,84,86,87,89,91,92,93,95};

#endif // __MIDI_CONST_H__
