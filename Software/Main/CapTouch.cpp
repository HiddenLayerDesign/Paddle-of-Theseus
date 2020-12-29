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
#include "TeensyBSP.h"

static int getScaledNote(int fret, cap_touch_id id, bool is_lefty_flipped)
{
  
  if (is_lefty_flipped)
  {
      id = (cap_touch_id) ((int) CAP_TOUCH_LIMIT - (int) id);
  }

  int note = fret + MIDI_SCALE_MIN;

  switch(id)
  {
    case CAP_TOUCH_0:
      return note;

    case CAP_TOUCH_1:
      return note + 5;
    
    case CAP_TOUCH_2:
      return note + 7;

    case CAP_TOUCH_3:
      return note + 12;
      
    default:
      digitalWrite(TEENSY_LED_PIN, HIGH);
      return note; 
  }  
}

/**
 * Initialize CapTouch member variables
 */
CapTouch::CapTouch(int pin, cap_touch_id id)
{
  _id = id;
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

void CapTouch::SendNote(int fret, int analog_volume, bool is_lefty_flipped)
{
  current_note = getScaledNote(fret, _id, is_lefty_flipped);
  usbMIDI.sendNoteOff(previous_note, 0, MIDI_CHANNEL_2);   
  usbMIDI.sendNoteOn(current_note, analog_volume, MIDI_CHANNEL_2);
  update_midi_msec  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
  midi_needs_update = false;
  previous_note = current_note;  
}
