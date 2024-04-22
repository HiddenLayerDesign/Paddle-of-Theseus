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

/* @todo a sort of "theory of operations" is owed here as to why so much internal state */

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
  this->_id = id;
  this->_array_idx = 0;
  this->_current_reading = LOW;
  this->_update_midi_msec = 0;
  this->_press_time = 0;
  this->_midi_needs_update= true;

  for (int i=0; i<STRUM_BUTTON_ARRAY_LEN; i++)
  {
    _strum_button_array[i] = 0;
  }
}

/**
 * Capture the readout of the sensor, update the state variables
 */
void StrumButton::Update(bool value)
{
  this->_current_reading = !value;
  if (this->_current_reading && this->_midi_needs_update)
  {
    this->_press_time = millis();
  }
  
  // set the reading into the array, increment the array modulo ARRAY_LEN
  this->_strum_button_array[this->_array_idx] = this->_current_reading;
  this->_array_idx = (this->_array_idx + 1) % STRUM_BUTTON_ARRAY_LEN;
}

/**
 * @brief Return the current reading
 */
bool StrumButton::GetReading(void)
{
  return this->_current_reading;
}

/**
 * @brief Return whether this reading is new 
 */
void StrumButton::CheckMIDINeedsUpdate(void)
{
  int sum = 0;
  for (int i=0; i < STRUM_BUTTON_ARRAY_LEN; i++)
  {
    sum += this->_strum_button_array[i]; 
  }
  this->_midi_needs_update = (!sum);
}

/**
 * @brief Return current note
 */
uint8_t StrumButton::GetCurrentNote(void)
{
  return this->_current_note;
}

/**
 * @brief Return previous note
 */
uint8_t StrumButton::GetPreviousNote(void)
{
  return this->_previous_note;
}

/**
 * @brief Generate usbMIDI messages based on current note and volume
 */
void StrumButton::SendNote(int fret, int analog_volume, bool is_lefty_flipped, config_t in_config)
{
  this->_current_note = getScaledNote(fret, _id, is_lefty_flipped, in_config);
  usbMIDI.sendNoteOff(this->_previous_note, 0, MIDI_CHANNEL_2);   
  usbMIDI.sendNoteOn(this->_current_note, analog_volume, MIDI_CHANNEL_2);
  this->_update_midi_msec  = millis() + STRUM_BUTTON_DEBOUNCE_MSEC;
  this->_midi_needs_update = false;
  this->_previous_note = this->_current_note;  
}

bool StrumButton::ShouldSendNote(void)
{
  return (GetReading() && 
          this->_midi_needs_update && 
          millis() > this->_update_midi_msec);
}

bool StrumButton::IsLongHold(void)
{
  /* if StrumButton is not currently being held, then false */
  if (this->_midi_needs_update)
  {
    return false;
  }  

  /* return whether StrumButton has been held for more than CONFIG_HOLD_DURATION_MSEC msec */
  return ((millis() - this->_press_time) > CONFIG_HOLD_DURATION_MSEC);
}