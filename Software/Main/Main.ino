/******************************************************* 
 *  File: Main
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  This design is built off of Teensy MIDI example code 
 *     Sparkfun code for the accelerometer
 *     and Seeed studio code for the Ultrasonic Rangefinder
 *  
 *  Note: You must select "Serial + MIDI" from the "Tools > USB Type" menu
 *  http://www.pjrc.com/teensy/td_midi.html
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
#include <Encoder.h>
#include <NewPing.h>
//#include <SerialUI.h>

#include "Arduino.h"
#include "MMA8452Q.h"
#include "CapTouch.h"
#include "LinearPotentiometer.h"
#include "MIDIConstants.h"
#include "RotaryEncoder.h"
#include "TeensyBSP.h"
#include "Ultrasonic.h"
#include "NonVolatile.h"
#include "Version.h"
#include "Preferences.h"

/* Prototypes */
void printBanner(void);
void pingCheck(void);
void softRestart(void);
void configurePins(void);
void printLoopDebugInfo(void);
int getVolume(bool is_lefty_flipped);


/* Ultrasonic Pitch Bend variables */
unsigned long ping_time;
unsigned long range_in_us;
unsigned long range_in_cm;
int curr_bend_val = 1;
int prev_bend_val = 0;

/* Rotary Potentiometer variables */
int analog_volume = 0;
int prev_analog_volume = 0;

/* MIDI variables */
int current_fret = 0;

/* Rotary Encoder Variables */
int curr_enc_reading = 0;
int prev_enc_reading = 1;
int constrained_enc_reading = 0;
enum rot_enc_state encoder_state = (rot_enc_state) 0;
int curr_rot_button = HIGH;
int prev_rot_button = HIGH;

/* Sensor class variables */
NewPing ultrasonic(TEENSY_ULTRA_TRIG_PIN, TEENSY_ULTRA_SENS_PIN, PITCH_BEND_MAX_CM+1);
Encoder rotEnc(TEENSY_ROT_ENC_PIN_1, TEENSY_ROT_ENC_PIN_2);
CapTouch capTouch0(TEENSY_CAP_TOUCH0_PIN, CAP_TOUCH_0);
CapTouch capTouch1(TEENSY_CAP_TOUCH1_PIN, CAP_TOUCH_1);
CapTouch capTouch2(TEENSY_CAP_TOUCH2_PIN, CAP_TOUCH_2);
CapTouch capTouch3(TEENSY_CAP_TOUCH3_PIN, CAP_TOUCH_3);
MMA8452Q accel;

/* General variables */
bool is_lefty_flipped = false;
bool is_config_mode = false;

/*
 * Setup PinModes and Serial port, Init digital sensors 
 */
void setup() 
{
  configurePins();    
  CheckUpdateVersionNumber();
  
  if (true == (is_config_mode = IsConfigModeEnabled()))
  {
    RotEncConfigPattern();
  }
  else
  {
    RotEncStandardPattern();

#ifdef DEBUG
    unsigned long flip_time = millis() + 500;
    bool is_green_not_yellow = true;

    Serial.begin(9600);
    while (!Serial)
    {
        if (PollForSerial())
        {
          is_green_not_yellow = !is_green_not_yellow;
          flip_time = millis() + 500;
        }
      }
    }
    printBanner();
#endif /* DEBUG */

    RotEncSetLED(rot_enc_led_color_array[encoder_state]);
  }
  
  accel.init();
  WriteConfigMode(false);
}

/*
 * Read all sensors, create and send MIDI messages as needed.
 */
void loop() 
{
  /* Check Lefty Flip status */
  accel.accel_update();
  is_lefty_flipped = accel.is_lefty_flipped();

  /* Sample Rotary Encoder Pushbutton */
  curr_rot_button = digitalRead(TEENSY_ROT_ENC_BUTTON_PIN);
  if (curr_rot_button == HIGH && prev_rot_button == LOW)  
  {
    if (capTouch0.IsLongHold() && capTouch1.IsLongHold() && capTouch2.IsLongHold() && capTouch3.IsLongHold())
    {
        DEBUG_PRINT("Setting Config Mode!");
        WriteConfigMode(true);
        RotEncConfigPattern();
        softRestart();  
    }
    encoder_state =  (rot_enc_state) ((encoder_state + 1) % ROT_ENC_ENUM_SIZE);
    RotEncSetLED(rot_enc_led_color_array[encoder_state]);
  }
  prev_rot_button = curr_rot_button;
  
  /* Sample Rotary Encoder Twist Knob */
  curr_enc_reading = rotEnc.read();
  constrained_enc_reading = constrain(curr_enc_reading, ROT_ENC_MIN, ROT_ENC_MAX);
  
  if (constrained_enc_reading != prev_enc_reading)
  {
    usbMIDI.sendControlChange(ROT_ENC_CTRL_CHANGE, constrained_enc_reading, MIDI_CHANNEL_2);
  }
  prev_enc_reading = constrained_enc_reading;

  /* Read MIDI note from potentiometer */
  current_fret = fret_from_lin_pot();

  /* Send note on debounced rising edge of TEENSY_CAP_TOUCH1_PIN */
  capTouch0.Update();
  capTouch1.Update();
  capTouch2.Update();
  capTouch3.Update();

  /* send notes if needed */
  if (capTouch0.ShouldSendNote()) 
    capTouch0.SendNote(current_fret, analog_volume, is_lefty_flipped);
  if (capTouch1.ShouldSendNote()) 
    capTouch1.SendNote(current_fret, analog_volume, is_lefty_flipped);
  if (capTouch2.ShouldSendNote())
    capTouch2.SendNote(current_fret, analog_volume, is_lefty_flipped);
  if (capTouch3.ShouldSendNote())
    capTouch3.SendNote(current_fret, analog_volume, is_lefty_flipped);

  /* Consider CapTouch sensors as triggered if any of last CAP_TOUCH_ARRAY_LEN samples were high */
  capTouch0.CheckMIDINeedsUpdate();
  capTouch1.CheckMIDINeedsUpdate();
  capTouch2.CheckMIDINeedsUpdate();
  capTouch3.CheckMIDINeedsUpdate();

  /* Get Ultrasonic Distance sensor reading */
  if (micros() >= ping_time)
  {
    /* NOTE: due to using newPing timer, this has to indirectly set range_in_us */
    ultrasonic.ping_timer(pingCheck);
    range_in_cm = range_in_us / US_ROUNDTRIP_CM;
    ping_time += ULTRASONIC_PING_PERIOD;
  }

  /* constrain range_in_cm, but sufficiently low values are treated as high ones */
  if (range_in_cm < PITCH_BEND_MIN_CM || range_in_cm > PITCH_BEND_MAX_CM)
  {
    range_in_cm = PITCH_BEND_MAX_CM;
  }

  /* Decide whether to update ultrasonic sensor */
  curr_bend_val = SCALED_PITCH_BEND(range_in_cm);

  if (curr_bend_val!= prev_bend_val && abs(curr_bend_val- prev_bend_val) < MAX_PITCH_BEND_DELTA)
  {
    usbMIDI.sendPitchBend(curr_bend_val, MIDI_CHANNEL_2);
    prev_bend_val = curr_bend_val;
  }

  /* set volume */
  analog_volume = getVolume(is_lefty_flipped);
  if (analog_volume < TEENSY_MIN_VOLUME)
  {
    analog_volume = 0;
    if ((prev_analog_volume >= TEENSY_MIN_VOLUME))
    {
      usbMIDI.sendNoteOff(capTouch0.current_note, 0, MIDI_CHANNEL_2);
      usbMIDI.sendNoteOff(capTouch1.current_note, 0, MIDI_CHANNEL_2);
      usbMIDI.sendNoteOff(capTouch2.current_note, 0, MIDI_CHANNEL_2);
      usbMIDI.sendNoteOff(capTouch3.current_note, 0, MIDI_CHANNEL_2);
    }
  }
  else
  {
    if (abs(prev_analog_volume - analog_volume) > 1)
    {
      usbMIDI.sendAfterTouch(analog_volume, MIDI_CHANNEL_2);
    }
  }
  prev_analog_volume = analog_volume;
  usbMIDI.send_now();

  /** 
   *  ignore incoming MIDI messages;
   *  
   *  MIDI Controllers should discard incoming MIDI messages.
   *  http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
   */ 
  while (usbMIDI.read()); 
  
  printLoopDebugInfo();
}

/**
 * Callback function to check whether ultrasonic sonar has returned data
 */
void pingCheck(void)
{
  range_in_us = (ultrasonic.check_timer()) ? ultrasonic.ping_result : range_in_us +2;
}

/**
 * Do static configuration of pins as INPUT or OUTPUT
 */
void configurePins(void)
{
  /* Set input sensor pins */
  pinMode(TEENSY_CAP_TOUCH0_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH1_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH2_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH3_PIN, INPUT);
  pinMode(TEENSY_ROT_ENC_BUTTON_PIN, INPUT);
  pinMode(TEENSY_ROT_POT_PIN, INPUT);

  /* The rotary switch is common anode with external pulldown, do not turn on pullup */
  pinMode(TEENSY_LED_PIN, OUTPUT);
  pinMode(TEENSY_ROT_LEDB, OUTPUT);
  pinMode(TEENSY_ROT_LEDG, OUTPUT);
  pinMode(TEENSY_ROT_LEDR, OUTPUT);  
}

/**
 * Return the instrument volume, taking into account whether a left- or right-handed person is using the paddle
 * 
 * @param is_lefty_flipped [in] True == the paddle is currently lefty-flipped, else False
 * @return int Volume between 0-255 where 255 is MAX volume
 */
int getVolume(bool is_lefty_flipped)
{
  return (is_lefty_flipped) ?
            floor(analogRead(TEENSY_ROT_POT_PIN) * 256.0/1024.0) :
            floor((1024 - analogRead(TEENSY_ROT_POT_PIN)) * 256.0/1024.0);
}

/**
 * Just print a quick serial banner- this is to de-clutter setup()
 */
void printBanner(void)
{
  version_t s_version = GetVersionFromEEPROM();
#ifndef DEBUG
  (void) s_version; // remove warning
#endif /* !DEBUG */

  DEBUG_PRINTLN();
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("***********************************************");
  DEBUG_PRINTLN("*               Paddle of Theseus             *");
  DEBUG_PRINTLN("*                                             *");
  DEBUG_PRINTLN("* By Chase E. Stewart for Hidden Layer Design *");
  DEBUG_PRINTLN("***********************************************");
  DEBUG_PRINT("Version ");
  DEBUG_PRINT(s_version.version_major);
  DEBUG_PRINT(".");
  DEBUG_PRINT(s_version.version_minor);
  DEBUG_PRINT(".");
  DEBUG_PRINT(s_version.version_bugfix);
  DEBUG_PRINTLN();
  DEBUG_PRINT("Config Mode enabled: ");
  DEBUG_PRINT((IsConfigModeEnabled()) ? "True " : "False");
  DEBUG_PRINTLN();
  DEBUG_PRINTLN();
}

/**
 * Use Carriage return to print a constantly-updating variable(s) once-per-loop
 */
void printLoopDebugInfo()
{
  DEBUG_PRINT("\rLoop Time: ");
  DEBUG_PRINT(loop_micros);
  DEBUG_PRINT(", volume: ");
  DEBUG_PRINT(analog_volume);
  DEBUG_PRINT("     ");
}

/**
 * Reset the Teensy in software
 */
void softRestart() 
{
  Serial.end();  //clears the serial monitor  if used
  SCB_AIRCR = 0x05FA0004;  //write value for restart
}

/**
 * Non-blocking function to check whether it's time to flip the LED, return whether LED was flipped
 * just a way to unclutter setup()
 * 
 * @param flip_time [in] timestamp in msec after which to flip the LED
 * @param is_green_not_yellow [in] True == set LED to green, else False == yellow
 * @return bool True == LED was flipped on this call, False == nothing happened
 */
bool PollForSerial(unsigned long flip_time, bool is_green_not_yellow)
{
  if (millis() > flip_time)
  {      
    RotEncSetLED((is_green_not_yellow) ? LED_GREEN : LED_YELLOW);
    return true;
  }
  return false;
}
