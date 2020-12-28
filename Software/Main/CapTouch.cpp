/******************************************************* 
 *  File: CapTouch.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#include "Arduino.h"
#include "CapTouch.h"
#include "MIDIConstants.h"

/**
 * Initialize CapTouch member variables
 */
CapTouch::CapTouch(int pin)
{
  _pin = pin;
  cap_touch_array[CAP_TOUCH_ARRAY_LEN] = {0};
  array_idx = 0;
  current_reading = LOW;
  midi_needs_update= true;
  update_midi_msec = 0;
}

/**
 * Capture the readout of the sensor, update the state variables
 */
void CapTouch::Update(void)
{
  current_reading = digitalRead(_pin);
  cap_touch_array[array_idx] = current_reading;
  array_idx = (array_idx + 1) % CAP_TOUCH_ARRAY_LEN;
}

/**
 * Return the current reading
 */
bool CapTouch::GetReading(void)
{
  return current_reading;
}

/**
 * Return whether this reading is new 
 */
void CapTouch::NewNote(void)
{
  int sum = 0;
  for (int i=0; i < CAP_TOUCH_ARRAY_LEN; i++)
  {
    sum += cap_touch_array[i]; 
  }
  midi_needs_update= (!sum);
}

void CapTouch::SendNote(int in_note, int analog_volume)
{
  current_note = in_note;
  usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[previous_note], 0, MIDI_CHANNEL_2);   
  usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[current_note], analog_volume, MIDI_CHANNEL_2);
  update_midi_msec  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
  midi_needs_update = false;
  previous_note = current_note;  
}
