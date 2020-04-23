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
#include "ADXL345.h"
#include "Rotary_Encoder.h"
#include <Encoder.h>

/* Teensy layout constants */
const int TEENSY_ULTRASONIC_PIN     = 0;
const int TEENSY_CAP_TOUCH_PIN      = 1;
const int TEENSY_ROT_ENC_PIN_1      = 3;
const int TEENSY_ROT_ENC_PIN_2      = 4;
const int TEENSY_LED_PIN            = 13;
const int TEENSY_ROT_ENC_BUTTON_PIN = 16;

const int CAP_TOUCH_ARRAY_LEN       = 3;
const int CAP_TOUCH_HYPER_DELAY     = 75;
const int CAP_TOUCH_DEBOUNCE_DELAY  = 10;
const int PITCH_BEND_RESOLUTION     = 1;

/* Prototypes */
void print_banner(void);
uint8_t note_from_lin_pot(void);

/* Ultrasonic Pitch Bend variables */
int range_in_cm        = 100;
bool update_pitch_bend = false;

/* MIDI variables */
int curr_note     = 0;
int prev_note     = 0;
int curr_bend_val = 1;
int prev_bend_val = 0;

/* Rotary Encoder Variables */
char rot_enc_array[ROT_ENC_ENUM_SIZE];
int curr_rot_button = LOW;
int prev_rot_button = LOW;
enum rot_enc_state encoder_state = ROT_ENC_EFFECT_1;
bool update_rot_enc = false;
int hyper_delay = CAP_TOUCH_DEBOUNCE_DELAY;

/* Capacitive Touch variables */
int cap_touch_array[CAP_TOUCH_ARRAY_LEN] = {0};
int cap_array_idx = 0;
int cap_touch_reading = 0;
unsigned int update_midi_msec  = 0;
bool midi_needs_update  = true;

/* Sensor class variables */
Ultrasonic ultrasonic(TEENSY_ULTRASONIC_PIN);
ADXL345 accel;
Encoder rot_enc(TEENSY_ROT_ENC_PIN_1,TEENSY_ROT_ENC_PIN_2);

/*
 * Setup PinModes and Serial port, Init digital sensors 
 */
void setup() 
{
  pinMode(TEENSY_LED_PIN, OUTPUT);
  pinMode(TEENSY_CAP_TOUCH_PIN, INPUT);
  pinMode(TEENSY_ROT_ENC_BUTTON_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
  print_banner();

  Serial.println("INFO: Initializing Accelerometer");
  if ( accel.init() < 0)
  {
    Serial.println("WARN: Failed to init ADXL345");
  }
}

/*
 * Read all sensors, create and send MIDI messages as needed.
 */
void loop() 
{
  loop_start = micros(); 
  update_rot_enc = false;
  
  /* Sample Rotary Encoder Pushbutton */
  curr_rot_button = digitalRead(TEENSY_ROT_ENC_BUTTON_PIN);
  if (curr_rot_button == HIGH && prev_rot_button == LOW)  
  {
    encoder_state = ((encoder_state + 1) % ROT_ENC_ENUM_SIZE);
    rot_enc.write(rot_enc_array[encoder_state]);
    if (encoder_state == ROT_ENC_HYPER)
    {
      rot_enc.write(CAP_TOUCH_HYPER_DELAY);
    }
  }
  prev_rot_button = curr_rot_button;
  
  /* Sample Rotary Encoder Twist Knob */
  long enc_reading;
  enc_reading = rot_enc.read();
  if (enc_reading < ROT_ENC_MIN)
  {
    rot_enc.write(ROT_ENC_MIN);  
  }
  else if (enc_reading > ROT_ENC_MAX)
  {
    rot_enc.write(ROT_ENC_MAX);  
  }
  if (enc_reading != rot_enc_array[encoder_state])
  { 
    rot_enc_array[encoder_state] = enc_reading;  
    update_rot_enc = true;
  }

  /* Read MIDI note from potentiometer */
  curr_note = note_from_lin_pot();

  /* Send note on debounced rising edge of TEENSY_CAP_TOUCH_PIN */
  cap_touch_reading = digitalRead(TEENSY_CAP_TOUCH_PIN);
  cap_touch_array[cap_array_idx] = cap_touch_reading;
  cap_array_idx = (cap_array_idx + 1) % CAP_TOUCH_ARRAY_LEN;

  if (encoder_state != ROT_ENC_HYPER)
  {
    if ( cap_touch_reading == HIGH && midi_needs_update == true)
    {
      if (millis() > update_midi_msec) 
      {
        curr_note = note_from_lin_pot();
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note);
        usbMIDI.sendNoteOff(prev_note, 0, MIDI_CHANNEL_1);   
        usbMIDI.sendNoteOn(curr_note, 50, MIDI_CHANNEL_1);
        update_midi_msec  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update = false;
        prev_note = curr_note;
      }
    }
  }
  else
  {
    if (cap_touch_reading == HIGH && (millis() > update_midi_msec))
    {
        curr_note = note_from_lin_pot();
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note);
        usbMIDI.sendNoteOff(prev_note, 0, MIDI_CHANNEL_1);   
        usbMIDI.sendNoteOn(curr_note, 50, MIDI_CHANNEL_1);
        update_midi_msec  = millis() + hyper_delay;
        midi_needs_update = false;
        prev_note = curr_note;
    }
  }

  int sum = 0;
  for (int i=0; i < CAP_TOUCH_ARRAY_LEN; i++) { sum += cap_touch_array[i]; }
  midi_needs_update = (sum == 0) ? true : false;
 
  if (encoder_state != ROT_ENC_HYPER)
  {
    digitalWrite(TEENSY_LED_PIN, HIGH);
    
    if (update_rot_enc == true)
    {
      usbMIDI.sendControlChange(rot_enc_ctrl_change[encoder_state], rot_enc_array[encoder_state], MIDI_CHANNEL_1);
      usbMIDI.send_now();
    }
  }
  else
  {
    digitalWrite(TEENSY_LED_PIN, LOW);  
    if (update_rot_enc == true)
    {
      hyper_delay = rot_enc_array[encoder_state];
    }
  }

  ultrasonic.distance_measure_blocking();
  range_in_cm = ultrasonic.microseconds_to_centimeters();
  if (range_in_cm < 50)
  {
    curr_bend_val = range_in_cm * 200;
    if (curr_bend_val != prev_bend_val)
    {
      update_pitch_bend = true;
    }
    prev_bend_val = curr_bend_val;
  }

  /* Update Pitch Bend and flush usbMIDI message */
  if (update_pitch_bend == true)
  {
    usbMIDI.sendPitchBend(curr_bend_val, MIDI_CHANNEL_1);
    update_pitch_bend = false;
  }
  usbMIDI.send_now();

  /* 
   *  MIDI Controllers should discard incoming MIDI messages.
   *  http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
   */ 
  while (usbMIDI.read()) 
  {
    /* ignore incoming messages */
  }
}

/**
 * Just print a quick serial banner- this is to de-clutter setup()
 */
void print_banner(void)
{
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("****************************************");
  Serial.println("*** Paddle of Theseus Serial Output  ***");
  Serial.println("****************************************");
  Serial.println();  
}

/**
 * Read the linear potentiometer and use it to get a note in the scale
 * 
 * @return MIDI note
 */
uint8_t note_from_lin_pot(void)
{
  const int SENSOR_MAX = 1024;
  const int SENSOR_MIN = 0;

  int lin_pot_voltage = analogRead(0);
  lin_pot_voltage = max(lin_pot_voltage, SENSOR_MIN);
  lin_pot_voltage = min(lin_pot_voltage, SENSOR_MAX);

  return IONIAN_SHARP_5_SCALE[(lin_pot_voltage/ SCALE_LEN)];
}
