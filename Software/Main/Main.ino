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

/* Teensy layout constants */
const int TEENSY_LED_PIN        = 13;
const int TEENSY_ULTRASONIC_PIN = 0;
const int TEENSY_CAP_TOUCH_PIN  = 1;
const int CAP_TOUCH_DEBOUNCE_DELAY = 50;
const int PITCH_BEND_RESOLUTION = 1;

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

/* Capacitive Touch variables */
int cap_touch_reading = 0;
int update_midi_msec  = 0;
bool midi_needs_update  = true;

/* Sensor class variables */
Ultrasonic ultrasonic(TEENSY_ULTRASONIC_PIN);
ADXL345 accel;

/*
 * Setup PinModes and Serial port, Init digital sensors 
 */
void setup() 
{
  pinMode(TEENSY_LED_PIN, OUTPUT);
  pinMode(TEENSY_CAP_TOUCH_PIN, INPUT);

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
  /* Get Pitch-bend value from Ultrasonic Sensor */
  ultrasonic.DistanceMeasure();
  range_in_cm = ultrasonic.microsecondsToCentimeters();
  if (range_in_cm < 50)
  {
    curr_bend_val = range_in_cm * 200;
    if (curr_bend_val != prev_bend_val)
    {
      update_pitch_bend = true;
      digitalWrite(TEENSY_LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(TEENSY_LED_PIN, LOW);
    }
    prev_bend_val = curr_bend_val;
  }
  else
  {
    digitalWrite(TEENSY_LED_PIN, LOW);
  }

  /* TODO Get X/Y/Z from Accelerometer */
  accel.accel_update();

  /* Read MIDI note from potentiometer */
  curr_note = note_from_lin_pot();

  /* Send note on debounced rising edge of TEENSY_CAP_TOUCH_PIN */
  cap_touch_reading = digitalRead(TEENSY_CAP_TOUCH_PIN);
  if (cap_touch_reading == HIGH)
  {
    if ( (millis() > update_midi_msec) && midi_needs_update)
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
  else
  {
    midi_needs_update = true;
  }

  /* Update Pitch Bend and flush usbMIDI message */
  if (update_pitch_bend == true)
  {
    usbMIDI.sendPitchBend(curr_bend_val, MIDI_CHANNEL_1);
    usbMIDI.send_now();
    update_pitch_bend = false;
  }

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

  return BLUES_SCALE[(lin_pot_voltage/ SCALE_LEN)];
}
