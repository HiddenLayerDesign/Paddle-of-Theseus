/******************************************************* 
 *  File: Main
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  This design is built off of Teensy MIDI example code 
 *     and Seeed studio code for the Ultrasonic Rangefinder
 *  
 *  Note: You must select "Serial + MIDI" from the "Tools > USB Type" menu
 *  http://www.pjrc.com/teensy/td_midi.html
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
 
#include "Arduino.h"
#include "Ultrasonic.h"
#include "MIDIConstants.h"
#include "ITG3200.h"

/* Teensy layout constants */
const int TEENSY_LED_PIN = 13;

elapsedMillis msec = 0;
int range_in_cm    = 100;
int outer_count    = 0;

/* MIDI variables */
int curr_note     = 0;
int prev_note     = 0;
int curr_bend_val = 1;
int prev_bend_val = 0;

Ultrasonic ultrasonic(TEENSY_ULTRASONIC_PIN);

void setup() 
{
  pinMode(TEENSY_LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
  if (msec >= 50) 
  {
    msec = 0;
    outer_count++;

    if (outer_count >= 40)
    {
      outer_count = 0;
      curr_note = BLUES_DIM_SCALE[random(SCALE_LEN)];
      usbMIDI.sendNoteOff(prev_note, 0, MIDI_CHANNEL_1);   
      usbMIDI.sendNoteOn(curr_note, 50, MIDI_CHANNEL_1);
      prev_note = curr_note;
    }

    ultrasonic.DistanceMeasure();
    range_in_cm = ultrasonic.microsecondsToCentimeters();

    /* Whammy bar only recognizes 50cm away or closer */
    if (range_in_cm < 50)
    {
      curr_bend_val = range_in_cm * 100;
      if (curr_bend_val != prev_bend_val)
      {
        usbMIDI.sendPitchBend(curr_bend_val, 0, MIDI_CHANNEL_1);
        digitalWrite(TEENSY_LED_PIN, HIGH);
      }
      else
      {
        digitalWrite(TEENSY_LED_PIN, LOW);
      }
      prev_bend_val = curr_bend_val;
    }
    else
    {
      digitalWrite(TEENSY_LED_PIN, LOW);
    }
  }

  /* 
   *  MIDI Controllers should discard incoming MIDI messages.
   *  http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
   */ 
  while (usbMIDI.read()) 
  {
    /* ignore incoming messages */
  }
}
