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
#include <Encoder.h>

#include "Arduino.h"
#include "ADXL345.h"
#include "CapTouch.h"
#include "LinearPotentiometer.h"
#include "MIDIConstants.h"
#include "ITG3200.h"
#include "RotaryEncoder.h"
#include "TeensyBSP.h"
#include "Ultrasonic.h"

#define PITCH_BEND_RESOLUTION    1

/* Prototypes */
void print_banner(void);

/* Ultrasonic Pitch Bend variables */
int range_in_cm        = 100;
bool update_pitch_bend = false;

/* MIDI variables */
int curr_note0    = 0;
int curr_note1    = 0;
int curr_note2    = 0;
int curr_note3    = 0;
int prev_note0    = 0;
int prev_note1    = 0;
int prev_note2    = 0;
int prev_note3    = 0;
int curr_bend_val = 1;
int prev_bend_val = 0;

/* Rotary Encoder Variables */
char rot_enc_array[ROT_ENC_ENUM_SIZE];
enum rot_enc_state encoder_state = ROT_ENC_EFFECT_1;
int hyper_delay = CAP_TOUCH_DEBOUNCE_DELAY;
int curr_rot_button = LOW;
int prev_rot_button = LOW;
bool update_rot_enc = false;

/* Capacitive Touch variables */
unsigned int update_midi_msec0  = 0;
unsigned int update_midi_msec1  = 0;
unsigned int update_midi_msec2  = 0;
unsigned int update_midi_msec3  = 0;
bool midi_needs_update0  = true;
bool midi_needs_update1  = true;
bool midi_needs_update2  = true;
bool midi_needs_update3  = true;

/* Sensor class variables */
Ultrasonic ultrasonic(TEENSY_ULTRASONIC_PIN);
Encoder rot_enc(TEENSY_ROT_ENC_PIN_1,TEENSY_ROT_ENC_PIN_2);
CapTouch capTouch0(TEENSY_CAP_TOUCH0_PIN);
CapTouch capTouch1(TEENSY_CAP_TOUCH1_PIN);
CapTouch capTouch2(TEENSY_CAP_TOUCH2_PIN);
CapTouch capTouch3(TEENSY_CAP_TOUCH3_PIN);

/*
 * Setup PinModes and Serial port, Init digital sensors 
 */
void setup() 
{
  pinMode(TEENSY_CAP_TOUCH0_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH1_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH2_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH3_PIN, INPUT);
  pinMode(TEENSY_ROT_ENC_BUTTON_PIN, INPUT_PULLUP);

  /* The rotary switch is common anode with external pulldown, do not turn on pullup */
  pinMode(TEENSY_ROT_LEDB, OUTPUT);
  pinMode(TEENSY_ROT_LEDG, OUTPUT);
  pinMode(TEENSY_ROT_LEDR, OUTPUT);
  set_rot_enc_led(rot_enc_led_color_array[encoder_state]);
  digitalWrite(TEENSY_LED_PIN, HIGH);

  for (int i=0; i< ROT_ENC_ENUM_SIZE; i++)
  {
    rot_enc_array[i] = 100;
  }
  
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
  curr_note0 = note_from_lin_pot();

  /* Send note on debounced rising edge of TEENSY_CAP_TOUCH1_PIN */
  capTouch0.Update();
  capTouch1.Update();
  capTouch2.Update();
  capTouch3.Update();

  if (encoder_state != ROT_ENC_HYPER)
  {
    if (capTouch0.GetReading() && midi_needs_update0)
    {
      if (millis() > update_midi_msec0) 
      {
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note0);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note0], 0, MIDI_CHANNEL_2);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note0], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec0  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update0 = false;
        prev_note0 = curr_note0;
      }
    }
    if (capTouch1.GetReading() && midi_needs_update1)
    {
      if (millis() > update_midi_msec1) 
      {
        curr_note1 = min(curr_note0 + 3, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note1);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note1], 0, MIDI_CHANNEL_2);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note1], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec1  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update1 = false;
        prev_note1 = curr_note1;
      }
    }
    if (capTouch2.GetReading() && midi_needs_update2)
    {
      if (millis() > update_midi_msec2) 
      {
        curr_note2 = min(curr_note0 + 5, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note2);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note2], 0, MIDI_CHANNEL_2);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note2], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec2  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update2 = false;
        prev_note2 = curr_note2;
      }
    }
    if (capTouch3.GetReading() && midi_needs_update3)
    {
      if (millis() > update_midi_msec3) 
      {
        curr_note3 = min(curr_note0 + 7, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note3);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note3], 0, MIDI_CHANNEL_2);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note3], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec3  = millis() + CAP_TOUCH_DEBOUNCE_DELAY;
        midi_needs_update3 = false;
        prev_note3 = curr_note3;
      }
    }
  }
  else
  {
    if (capTouch0.GetReading() && (millis() > update_midi_msec0))
    {
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note0);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note0], 0, MIDI_CHANNEL_2);
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note0], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec0  = millis() + hyper_delay;
        midi_needs_update0 = false;
        prev_note0 = curr_note0;
    }
    if (capTouch1.GetReading() && (millis() > update_midi_msec1))
    {
        curr_note1 = min(curr_note0 + 3, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note1);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note1], 0, MIDI_CHANNEL_2);
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note1], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec1  = millis() + hyper_delay;
        midi_needs_update1 = false;
        prev_note1 = curr_note1;
    }
    if (capTouch2.GetReading() && (millis() > update_midi_msec2))
    {
        curr_note2 = min(curr_note0 + 5, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note2);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note2], 0, MIDI_CHANNEL_2);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note2], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec2  = millis() + hyper_delay;
        midi_needs_update2 = false;
        prev_note2 = curr_note2;
    }
    if (capTouch3.GetReading() && (millis() > update_midi_msec3))
    {
        curr_note3 = min(curr_note0 + 7, SCALE_LEN-1);
        Serial.print("INFO: Sent MIDI note ");
        Serial.println(curr_note3);
        usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[prev_note3], 0, MIDI_CHANNEL_2);   
        usbMIDI.sendNoteOn(IONIAN_SHARP_5_SCALE[curr_note3], rot_enc_array[ROT_ENC_VOLUME], MIDI_CHANNEL_2);
        update_midi_msec3  = millis() + hyper_delay;
        midi_needs_update3 = false;
        prev_note3 = curr_note3;
    }
  }

  /* Consider CapTouch sensors as triggered if any of last CAP_TOUCH_ARRAY_LEN samples were high */
  midi_needs_update0 = capTouch0.NewNote();
  midi_needs_update1 = capTouch1.NewNote();
  midi_needs_update2 = capTouch2.NewNote();
  midi_needs_update3 = capTouch3.NewNote();

  /* Update MIDI settings based on RotEnc twist knob */
  if (update_rot_enc)
  {
    if (encoder_state == ROT_ENC_HYPER)
    {
      hyper_delay = rot_enc_array[encoder_state];
    }
    else
    {
      usbMIDI.sendControlChange(rot_enc_ctrl_change[encoder_state], rot_enc_array[encoder_state], MIDI_CHANNEL_2);
      usbMIDI.send_now();
    }
  }

  /* Get Ultrasonic Distance sensor reading */
  ultrasonic.distance_measure_blocking();
  range_in_cm = ultrasonic.microseconds_to_centimeters();

  /* Decide whether to update ultrasonic sensor */
  curr_bend_val = (range_in_cm < 30) ? (range_in_cm * 50) : 0 ;
  update_pitch_bend = (curr_bend_val != prev_bend_val);
  prev_bend_val = curr_bend_val;
  
  /* Update Pitch Bend and flush usbMIDI message */
  if (update_pitch_bend)
  {
    usbMIDI.sendPitchBend(curr_bend_val, MIDI_CHANNEL_2);
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
