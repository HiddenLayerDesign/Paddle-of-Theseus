/******************************************************* 
 *  File: StrumButton.hpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __STRUM_BUTTON_HPP__
#define __STRUM_BUTTON_HPP__

#include "Preferences.hpp"
#include "NonVolatile.hpp"

#define STRUM_BUTTON_ARRAY_LEN 2

enum strum_button_id {
  STRUM_BUTTON_0 = 0,
  STRUM_BUTTON_1 = 1,
  STRUM_BUTTON_2 = 2,
  STRUM_BUTTON_3 = 3,
  STRUM_BUTTON_LIMIT = 3
};

class StrumButton
{
  public:
    StrumButton(strum_button_id id);
    void Update(bool current_reading);
    bool GetReading(void);
    void SendNote(int int_note, int analog_volume, bool is_lefty_flipped, config_t in_config);
    void CheckMIDINeedsUpdate(void);
    bool ShouldSendNote(void);
    bool IsLongHold(void);
    uint8_t GetCurrentNote(void);
    uint8_t GetPreviousNote(void);

  private:
    strum_button_id _id;
    int _array_idx;
    int _current_reading;
    unsigned long _press_time;
    unsigned long _update_midi_msec;
    bool _midi_needs_update;
    int _previous_note;
    int _current_note;
    int _strum_button_array[STRUM_BUTTON_ARRAY_LEN];
};

#endif /* __STRUM_BUTTON_HPP__ */