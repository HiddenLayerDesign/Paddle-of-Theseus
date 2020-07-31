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
#include "LinearPotentiometer.h"
#include "MIDIConstants.h"
#include "ITG3200.h"
#include "ADXL345.h"
#include "RotaryEncoder.h"
#include "TeensyBSP.h"
#include <Encoder.h>

#define CAP_TOUCH_ARRAY_LEN      3
#define CAP_TOUCH_HYPER_DELAY    75
#define CAP_TOUCH_DEBOUNCE_DELAY 10
#define PITCH_BEND_RESOLUTION    1

/* Prototypes */
void print_banner(void);

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
Encoder rot_enc(TEENSY_ROT_ENC_PIN_1,TEENSY_ROT_ENC_PIN_2);

/*
 * Setup PinModes and Serial port, Init digital sensors 
 */
void setup() 
{
  pinMode(TEENSY_LED_PIN, OUTPUT);
  pinMode(TEENSY_CAP_TOUCH1_PIN, INPUT);
  pinMode(TEENSY_ROT_ENC_BUTTON_PIN, INPUT_PULLUP);

  /* The rotary switch is common anode with external pulldown, do not turn on pullup */
  pinMode(TEENSY_ROT_LEDB, OUTPUT);
  pinMode(TEENSY_ROT_LEDG, OUTPUT);
  pinMode(TEENSY_ROT_LEDR, OUTPUT);
  set_rot_enc_led(rot_enc_led_color_array[encoder_state]);
  
  Serial.begin(9600);
  print_banner();
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
    encoder_state =  (rot_enc_state) ((encoder_state + 1) % ROT_ENC_ENUM_SIZE);
    set_rot_enc_led(rot_enc_led_color_array[encoder_state]);
    rot_enc.write((encoder_state == ROT_ENC_HYPER) ? CAP_TOUCH_HYPER_DELAY : rot_enc_array[encoder_state] );
    Serial.print("INFO: Encoder state is");
    Serial.println(encoder_state);    
  }
  prev_rot_button = curr_rot_button;
  
  /* Sample Rotary Encoder Twist Knob */
  long enc_reading;
  int constrained_enc_reading;
  enc_reading = rot_enc.read();
  constrained_enc_reading = constrain(enc_reading, ROT_ENC_MIN, ROT_ENC_MAX);
  if (constrained_enc_reading != rot_enc_array[encoder_state])
  { 
    Serial.print("INFO: Encoder value is");
    Serial.println(enc_reading);    
    rot_enc_array[encoder_state] = constrained_enc_reading;
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
    if ( cap_touch_reading1 == HIGH && midi_needs_update1)
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
    if ( cap_touch_reading2 == HIGH && midi_needs_update2)
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
    if ( cap_touch_reading3 == HIGH && midi_needs_update3)
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

  /* Consider CapTouch sensors as triggered if any of last CAP_TOUCH_ARRAY_LEN samples were high */
  int sum1 = 0;
  int sum2 = 0;
  int sum3 = 0;
  for (int i=0; i < CAP_TOUCH_ARRAY_LEN; i++)
  {
    sum1 += cap_touch_array1[i]; 
    sum2 += cap_touch_array2[i]; 
    sum3 += cap_touch_array3[i];
  }
  midi_needs_update1 = (!sum1) ? true : false;
  midi_needs_update2 = (!sum2) ? true : false;
  midi_needs_update3 = (!sum3) ? true : false;

  /* Update MIDI settings based on RotEnc twist knob */
  if (update_rot_enc)
  {
    if (encoder_state == ROT_ENC_HYPER)
    {
      hyper_delay = rot_enc_array[encoder_state];
    }
    else
    {
      usbMIDI.sendControlChange(rot_enc_ctrl_change[encoder_state], rot_enc_array[encoder_state], MIDI_CHANNEL_1);
      usbMIDI.send_now();
    }
  }
  digitalWrite(TEENSY_LED_PIN, (encoder_state == ROT_ENC_HYPER) ? LOW : HIGH);

  /* Get Ultrasonic Distance sensor reading */
  ultrasonic.distance_measure_blocking();
  range_in_cm = ultrasonic.microseconds_to_centimeters();

  /* Decide whether to update ultrasonic sensor */
  curr_bend_val = (range_in_cm < 30) ? (range_in_cm * 200) : 0 ;
  update_pitch_bend = (curr_bend_val != prev_bend_val);
  prev_bend_val = curr_bend_val;
  
  /* Update Pitch Bend and flush usbMIDI message */
  if (update_pitch_bend)
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
