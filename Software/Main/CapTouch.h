/******************************************************* 
 *  File: CapTouch.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __CAP_TOUCH_H__
#define __CAP_TOUCH_H__

#include "Preferences.h"
#include "NonVolatile.h"

#define CAP_TOUCH_ARRAY_LEN 2

enum cap_touch_id {
  CAP_TOUCH_0 = 0,
  CAP_TOUCH_1 = 1,
  CAP_TOUCH_2 = 2,
  CAP_TOUCH_3 = 3,
  CAP_TOUCH_LIMIT = 3
};

class CapTouch
{
  public:
    CapTouch(int pin, cap_touch_id id);
    void Update(void);
    bool GetReading(void);
    void SendNote(int int_note, int analog_volume, bool is_lefty_flipped, config_t in_config);
    void CheckMIDINeedsUpdate(void);
    bool ShouldSendNote(void);
    bool IsLongHold(void);
    int previous_note;
    int current_note;
    unsigned long update_midi_msec;
    bool midi_needs_update;
    unsigned long press_time;
    
  private:
    cap_touch_id _id;
    int _pin; // pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
    int cap_touch_array[CAP_TOUCH_ARRAY_LEN];
    int array_idx;
    int current_reading;
};

#endif /* __CAP_TOUCH_H__ */
