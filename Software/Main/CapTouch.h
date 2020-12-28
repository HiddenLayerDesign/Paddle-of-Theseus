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

#define CAP_TOUCH_ARRAY_LEN      2
#define CAP_TOUCH_HYPER_DELAY    75
#define CAP_TOUCH_DEBOUNCE_DELAY 10


class CapTouch
{
  public:
    CapTouch(int pin);
    void Update(void);
    bool GetReading(void);
    void SendNote(int int_note, int analog_volume);
    void NewNote(void);
    int previous_note;
    int current_note;
    unsigned long update_midi_msec;
    bool midi_needs_update;
    
  private:
    int _pin; // pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
    int cap_touch_array[CAP_TOUCH_ARRAY_LEN];
    int array_idx;
    int current_reading;
};

#endif /* __CAP_TOUCH_H__ */
