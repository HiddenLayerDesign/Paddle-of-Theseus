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

#define ACCEL_I2C_ADDR              (0xA6>>1)


/* Teensy layout constants */
const int TEENSY_LED_PIN = 13;

elapsedMillis msec = 0;
int range_in_cm    = 100;
int outer_count    = 0;

/* MIDI variables */
int curr_note     = 0;
int prev_note     = 0;
int curr_bend_val = 1;
int prev_bend_val = 0;

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
int note_from_lin_pot(void)
{
  const int SENSOR_MAX = 1024;
  const int SENSOR_MIN = 0;

  int lin_pot_voltage = analogRead(0);
  lin_pot_voltage = max(lin_pot_voltage, SENSOR_MIN);
  lin_pot_voltage = min(lin_pot_voltage, SENSOR_MAX);

  if (lin_pot_voltage > 5)
  {
    Serial.print("Analog 0 is: ");
    Serial.println(lin_pot_voltage);
  }
  return BLUES_SCALE[(lin_pot_voltage/ SCALE_LEN)];
}

Ultrasonic ultrasonic(TEENSY_ULTRASONIC_PIN);
ADXL345 accel;

void setup() 
{
  byte rx_byte = 255;
  
  pinMode(TEENSY_LED_PIN, OUTPUT);
  Serial.begin(9600);
  print_banner();

  Serial.println("INFO: Initializing Accelerometer");
  if ( accel.init() < 0)
  {
    Serial.println("WARN: Failed to init ADXL345");
  }

  
}

void loop() 
{  
  if (msec >= 200) 
  {
    accel.accel_update();
    msec = 0;

    curr_note = note_from_lin_pot();
    usbMIDI.sendNoteOff(prev_note, 0, MIDI_CHANNEL_1);   
    usbMIDI.sendNoteOn(curr_note, 50, MIDI_CHANNEL_1);
    prev_note = curr_note;

    ultrasonic.DistanceMeasure();
    range_in_cm = ultrasonic.microsecondsToCentimeters();

    /* Whammy bar only recognizes 50cm away or closer */
    if (range_in_cm < 50)
    {
      curr_bend_val = range_in_cm * 200;
      if (curr_bend_val != prev_bend_val)
      {
        usbMIDI.sendPitchBend(curr_bend_val, 0, MIDI_CHANNEL_1);
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
