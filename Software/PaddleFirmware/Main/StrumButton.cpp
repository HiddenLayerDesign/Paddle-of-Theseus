/******************************************************* Pr
 *  File: StrumButton.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#include "Arduino.h"
#include "StrumButton.hpp"
#include "MIDIConstants.hpp"
#include "BoardLayout.hpp"

static int getScaledNote(int fret, strum_button_id id, bool is_lefty_flipped, config_t in_config)
{
  int delta;
  
  if (is_lefty_flipped)
  {
      id = (strum_button_id) ((int) STRUM_BUTTON_LIMIT - (int) id);
  }

  switch(id)
  {
    case STRUM_BUTTON_0:
      delta = fret;
      break;
      
    case STRUM_BUTTON_1:
      delta = fret + in_config.button1_offset;
      break;
    
    case STRUM_BUTTON_2:
      delta = fret + in_config.button2_offset;
      break;

    case STRUM_BUTTON_3:
      delta = fret + in_config.button3_offset;
      break;
      
    default:
      digitalWrite(PIN_TEENSY_LED, HIGH);
      return in_config.root_note; 
  }
  
  if (in_config.modifier == MOD_MAJOR)
  {
    return in_config.octave + in_config.root_note + MAJOR_DELTAS[delta];
  }
  else if (in_config.modifier == MOD_MINOR)
  {
    return in_config.octave + in_config.root_note + MINOR_DELTAS[delta];
  }
  else if (in_config.modifier == MOD_MIXOLYDIAN)
  {
    return in_config.octave + in_config.root_note + MIXOLYDIAN_DELTAS[delta];
  }
  else if (in_config.modifier == MOD_DORIAN)
  {
    return in_config.octave + in_config.root_note + DORIAN_DELTAS[delta];
  }
  else if (in_config.modifier == MOD_CHROMATIC)
  {
    return in_config.octave + in_config.root_note + CHROMATIC_DELTAS[delta];
  }
  else 
  {  
    return in_config.octave + in_config.root_note;
  }
}

StrumButton::StrumButton(strum_button_id id)
{
  _id = id;
  array_idx = 0;
  current_reading = LOW;
  midi_needs_update= true;
  update_midi_msec = 0;

  for (int i=0; i<STRUM_BUTTON_ARRAY_LEN; i++)
  {
    strum_button_array[i] = 0;
  }

}

/**
 * Capture the readout of the sensor, update the state variables
 */
void StrumButton::Update(bool current_reading)
{
  if (current_reading && midi_needs_update)
  {
    press_time = millis();
  }
  
  strum_button_array[array_idx] = current_reading;
  array_idx = (array_idx + 1) % STRUM_BUTTON_ARRAY_LEN;
}

/**
 * Return the current reading
 */
bool StrumButton::GetReading(void)
{
  return current_reading;
}

/**
 * Return whether this reading is new 
 */
void StrumButton::CheckMIDINeedsUpdate(void)
{
  int sum = 0;
  for (int i=0; i < STRUM_BUTTON_ARRAY_LEN; i++)
  {
    sum += strum_button_array[i]; 
  }
  midi_needs_update= (!sum);
}

void StrumButton::SendNote(int fret, int analog_volume, bool is_lefty_flipped, config_t in_config)
{
  current_note = getScaledNote(fret, _id, is_lefty_flipped, in_config);
  usbMIDI.sendNoteOff(previous_note, 0, MIDI_CHANNEL_2);   
  usbMIDI.sendNoteOn(current_note, analog_volume, MIDI_CHANNEL_2);
  update_midi_msec  = millis() + STRUM_BUTTON_DEBOUNCE_DELAY;
  midi_needs_update = false;
  previous_note = current_note;  
}

bool StrumButton::ShouldSendNote(void)
{
  return (GetReading() && 
          midi_needs_update && 
          millis() > update_midi_msec);
}

bool StrumButton::IsLongHold(void)
{
  /* if StrumButton is not currently being held, then false */
  if (midi_needs_update)
  {
    return false;
  }  

  /* return whether StrumButton has been held for more than CONFIG_HOLD_DURATION_MILLIS msec */
  return ((millis() - press_time) > CONFIG_HOLD_DURATION_MILLIS);
}