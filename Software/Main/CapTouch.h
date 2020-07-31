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

#define CAP_TOUCH_ARRAY_LEN      3
#define CAP_TOUCH_HYPER_DELAY    75
#define CAP_TOUCH_DEBOUNCE_DELAY 10


class CapTouch
{
  public:
    CapTouch(int pin);
    void Update(void);
    bool GetReading(void);
    bool NewNote(void);
    
  private:
    int _pin; // pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
    int cap_touch_array[CAP_TOUCH_ARRAY_LEN];
    int array_idx;
    int current_reading;
};



#endif /* __CAP_TOUCH_H__ */
