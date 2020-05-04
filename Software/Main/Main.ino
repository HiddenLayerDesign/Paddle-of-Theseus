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
#define TEENSY_ULTRASONIC_PIN     0
#define TEENSY_CAP_TOUCH1_PIN     2
#define TEENSY_CAP_TOUCH2_PIN     21
#define TEENSY_CAP_TOUCH3_PIN     22
#define TEENSY_ROT_ENC_PIN_1      3
#define TEENSY_ROT_ENC_PIN_2      4
#define TEENSY_LED_PIN            13
#define TEENSY_ROT_ENC_BUTTON_PIN 16
#define TEENSY_ROT_LEDG           10
#define TEENSY_ROT_LEDB           11
#define TEENSY_ROT_LEDR           9

#define CAP_TOUCH_ARRAY_LEN      3
#define CAP_TOUCH_HYPER_DELAY    75
#define CAP_TOUCH_DEBOUNCE_DELAY 10
#define PITCH_BEND_RESOLUTION    1


/* Prototypes */
void print_banner(void);
uint8_t note_from_lin_pot(void);
void set_rot_enc_led(uint8_t color);

/* Ultrasonic Pitch Bend variables */
int range_in_cm        = 100;
bool update_pitch_bend = false;

/* MIDI variables */
int curr_note1    = 0;
int curr_note2    = 0;
int curr_note3    = 0;
int prev_note1    = 0;
int prev_note2    = 0;
int prev_note3    = 0;
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
int cap_touch_array1[CAP_TOUCH_ARRAY_LEN] = {0};
int cap_touch_array2[CAP_TOUCH_ARRAY_LEN] = {0};
int cap_touch_array3[CAP_TOUCH_ARRAY_LEN] = {0};
int cap_array_idx1 = 0;
int cap_array_idx2 = 0;
int cap_array_idx3 = 0;
int cap_touch_reading1 = 0;
int cap_touch_reading2 = 0;
int cap_touch_reading3 = 0;
unsigned int update_midi_msec1  = 0;
unsigned int update_midi_msec2  = 0;
unsigned int update_midi_msec3  = 0;
bool midi_needs_update1  = true;
bool midi_needs_update2  = true;
bool midi_needs_update3  = true;

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
  pinMode(TEENSY_CAP_TOUCH1_PIN, INPUT);
  pinMode(TEENSY_ROT_ENC_BUTTON_PIN, INPUT_PULLUP);

  // The rotary switch is common anode with external pulldown, do not turn on pullup
  pinMode(TEENSY_ROT_LEDB, OUTPUT);
  pinMode(TEENSY_ROT_LEDG, OUTPUT);
  pinMode(TEENSY_ROT_LEDR, OUTPUT);
  set_rot_enc_led(rot_enc_led_color_array[encoder_state]);
  
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
  update_rot_enc = false;
  
  /* Sample Rotary Encoder Pushbutton */
  curr_rot_button = digitalRead(TEENSY_ROT_ENC_BUTTON_PIN);
  if (curr_rot_button == LOW && prev_rot_button == HIGH)  
  {
    encoder_state = ((encoder_state + 1) % ROT_ENC_ENUM_SIZE);
    set_rot_enc_led(rot_enc_led_color_array[encoder_state]);
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
  curr_note1 = note_from_lin_pot();

  /* Send note on debounced rising edge of TEENSY_CAP_TOUCH1_PIN */
  cap_touch_reading1 = digitalRead(TEENSY_CAP_TOUCH1_PIN);
  cap_touch_array1[cap_array_idx1] = cap_touch_reading1;
  cap_array_idx1 = (cap_array_idx1 + 1) % CAP_TOUCH_ARRAY_LEN;

  cap_touch_reading2 = digitalRead(TEENSY_CAP_TOUCH2_PIN);
  cap_touch_array2[cap_array_idx2] = cap_touch_reading2;
  cap_array_idx2 = (cap_array_idx2 + 1) % CAP_TOUCH_ARRAY_LEN;

  cap_touch_reading3 = digitalRead(TEENSY_CAP_TOUCH3_PIN);
  cap_touch_array3[cap_array_idx3] = cap_touch_reading3;
  cap_array_idx3 = (cap_array_idx3 + 1) % CAP_TOUCH_ARRAY_LEN;

  if (encoder_state != ROT_ENC_HYPER)
  {
    if ( cap_touch_reading1 == HIGH && midi_needs_update1 == true)
    {
      if (millis() > update_midi_msec1) 
      {
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note1);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note1], 0, MIDI_CHANNEL_1);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note1], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_1);
        update_midi_msec1  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update1 = false;
        prev_note1 = curr_note1;
      }
    }
    if ( cap_touch_reading2 == HIGH && midi_needs_update2 == true)
    {
      if (millis() > update_midi_msec2) 
      {
        curr_note2 = min(curr_note1 + 3, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note2);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note2], 0, MIDI_CHANNEL_1);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note2], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_1);
        update_midi_msec2  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update2 = false;
        prev_note2 = curr_note2;
      }
    }
    if ( cap_touch_reading3 == HIGH && midi_needs_update3 == true)
    {
      if (millis() > update_midi_msec3) 
      {
        curr_note3 = min(curr_note1 + 5, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note3);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note3], 0, MIDI_CHANNEL_1);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note3], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_1);
        update_midi_msec3  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update3 = false;
        prev_note3 = curr_note3;
      }
    }
  }
  else
  {
    if (cap_touch_reading1 == HIGH && (millis() > update_midi_msec1))
    {
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note1);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note1], 0, MIDI_CHANNEL_1);
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note1], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_1);
        update_midi_msec1  = millis() + hyper_delay;
        midi_needs_update1 = false;
        prev_note1 = curr_note1;
    }
    if (cap_touch_reading2 == HIGH && (millis() > update_midi_msec2))
    {
        curr_note2 = min(curr_note1 + 3, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note2);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note2], 0, MIDI_CHANNEL_1);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note2], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_1);
        update_midi_msec2  = millis() + hyper_delay;
        midi_needs_update2 = false;
        prev_note2 = curr_note2;
    }
    if (cap_touch_reading3 == HIGH && (millis() > update_midi_msec3))
    {
        curr_note3 = min(curr_note1 + 5, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note3);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note3], 0, MIDI_CHANNEL_1);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note3], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_1);
        update_midi_msec3  = millis() + hyper_delay;
        midi_needs_update3 = false;
        prev_note3 = curr_note3;
    }
  }

  int sum1 = 0;
  int sum2 = 0;
  int sum3 = 0;
  for (int i=0; i < CAP_TOUCH_ARRAY_LEN; i++)
  {
    sum1 += cap_touch_array1[i]; 
    sum2 += cap_touch_array2[i]; 
    sum3 += cap_touch_array3[i];
  }
  midi_needs_update1 = (sum1 == 0) ? true : false;
  midi_needs_update2 = (sum2 == 0) ? true : false;
  midi_needs_update3 = (sum3 == 0) ? true : false;
 
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

  return (lin_pot_voltage/ SCALE_LEN);
}

/**
 * Set the rotary encoder's built-in LED to one of the values defined at the top of the file 
 * 
 * @param color An enum of one of the possible RGB colors. 
 */
void set_rot_enc_led(uint8_t color)
{
  digitalWrite(TEENSY_ROT_LEDR, color & 0x1);
  digitalWrite(TEENSY_ROT_LEDG, color & 0x2);
  digitalWrite(TEENSY_ROT_LEDB, color & 0x4);
}
