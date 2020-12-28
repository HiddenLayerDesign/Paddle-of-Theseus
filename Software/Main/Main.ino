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

#include "Arduino.h"
#include "MMA8452Q.h"
#include "CapTouch.h"
#include "LinearPotentiometer.h"
#include "MIDIConstants.h"
#include "RotaryEncoder.h"
#include "TeensyBSP.h"
#include "Ultrasonic.h"

/**
 * Pitch bend accepts a 14-bit twos compliment value, 
 * but we only want to detune like a whammy bar, so we use 13 bits and subtract 2/13 so max val is 0
 */

// TODO FIXE pow(2,13 for full range!)
#define SCALED_PITCH_BEND(x) (int) (pow(2,12) * x  / PITCH_BEND_MAX_CM) - pow(2,12)  // Do not adjust!

#define GET_VOLUME(is_lefty_flipped) (is_lefty_flipped) ? \
            floor(analogRead(TEENSY_ROT_POT_PIN) * 256.0/1024.0) : \
            floor((1024 - analogRead(TEENSY_ROT_POT_PIN)) * 256.0/1024.0);

#define TEENSY_MIN_VOLUME 30


/* Prototypes */
void print_banner(void);
void pingCheck(void);

/* Ultrasonic Pitch Bend variables */
const unsigned long ping_period = 1;
unsigned long ping_time;
int range_in_cm        = PITCH_BEND_MAX_CM;
bool update_pitch_bend = false;

/* Rotary Potentiometer variables */
int analog_volume = 0;
int prev_analog_volume = 0;

/* MIDI variables */
int current_note  = 0;
int curr_bend_val = 1;
int prev_bend_val = 0;

/* Rotary Encoder Variables */
char rot_enc_array[ROT_ENC_ENUM_SIZE];
enum rot_enc_state encoder_state = (rot_enc_state) 0;
int hyper_delay = CAP_TOUCH_DEBOUNCE_DELAY;
int curr_rot_button = HIGH;
int prev_rot_button = HIGH;
bool update_rot_enc = false;

/* Sensor class variables */
NewPing ultrasonic(TEENSY_ULTRA_TRIG_PIN, TEENSY_ULTRA_SENS_PIN, 100);
Encoder rot_enc(TEENSY_ROT_ENC_PIN_1,TEENSY_ROT_ENC_PIN_2);
CapTouch capTouch0(TEENSY_CAP_TOUCH0_PIN);
CapTouch capTouch1(TEENSY_CAP_TOUCH1_PIN);
CapTouch capTouch2(TEENSY_CAP_TOUCH2_PIN);
CapTouch capTouch3(TEENSY_CAP_TOUCH3_PIN);
MMA8452Q accel;

/* General variables */
bool is_lefty_flipped    = false;
unsigned long start_micros = 0;
unsigned long loop_micros = 0;

/*
 * Setup PinModes and Serial port, Init digital sensors 
 */
void setup() 
{
  /* Set input sensor pins */
  pinMode(TEENSY_CAP_TOUCH0_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH1_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH2_PIN, INPUT);
  pinMode(TEENSY_CAP_TOUCH3_PIN, INPUT);
  pinMode(TEENSY_ROT_ENC_BUTTON_PIN, INPUT);
  pinMode (TEENSY_ROT_POT_PIN, INPUT_PULLUP);

  /* The rotary switch is common anode with external pulldown, do not turn on pullup */
  pinMode(TEENSY_LED_PIN, OUTPUT);
  pinMode(TEENSY_ROT_LEDB, OUTPUT);
  pinMode(TEENSY_ROT_LEDG, OUTPUT);
  pinMode(TEENSY_ROT_LEDR, OUTPUT);
  set_rot_enc_led(rot_enc_led_color_array[encoder_state]);  

  for (int i=0; i< ROT_ENC_ENUM_SIZE; i++)
  {
    rot_enc_array[i] = 100;
  }

#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial);
  print_banner();
#endif /* DEBUG */

  if (!accel.init())
  {
     DEBUG_PRINTLN("WARNING: Failed to init accelerometer!");  
  }
}

/*
 * Read all sensors, create and send MIDI messages as needed.
 */
void loop() 
{
  start_micros = micros();
 
  /* Check Lefty Flip status */
  accel.accel_update();
  is_lefty_flipped = accel.is_lefty_flipped();
  set_rot_enc_led(is_lefty_flipped ? LED_RED : LED_GREEN);

  /* set volume */
  analog_volume = GET_VOLUME(is_lefty_flipped);
  
  /* Sample Rotary Encoder Pushbutton */
  curr_rot_button = digitalRead(TEENSY_ROT_ENC_BUTTON_PIN);
  if (curr_rot_button == HIGH && prev_rot_button == LOW)  
  {
    encoder_state =  (rot_enc_state) ((encoder_state + 1) % ROT_ENC_ENUM_SIZE);
    set_rot_enc_led(rot_enc_led_color_array[encoder_state]);
    rot_enc.write((encoder_state == ROT_ENC_HYPER) ? CAP_TOUCH_HYPER_DELAY : rot_enc_array[encoder_state] );
  }
  prev_rot_button = curr_rot_button;
  
  /* Sample Rotary Encoder Twist Knob */
  long enc_reading;
  int constrained_enc_reading;
  
  enc_reading = rot_enc.read();
  constrained_enc_reading = constrain(enc_reading, ROT_ENC_MIN, ROT_ENC_MAX);

  update_rot_enc = false;
  if (constrained_enc_reading != rot_enc_array[encoder_state])
  {
    rot_enc_array[encoder_state] = constrained_enc_reading;
    update_rot_enc = true;
  }

  /* Read MIDI note from potentiometer */
  current_note = note_from_lin_pot();

  /* Send note on debounced rising edge of TEENSY_CAP_TOUCH1_PIN */
  capTouch0.Update();
  capTouch1.Update();
  capTouch2.Update();
  capTouch3.Update();

  if (capTouch0.GetReading() && capTouch0.midi_needs_update)
  {
    if (millis() > capTouch0.update_midi_msec) 
    {
      capTouch0.SendNote(current_note, analog_volume);
    }
  }
  if (capTouch1.GetReading() && capTouch1.midi_needs_update)
  {
    if (millis() > capTouch1.update_midi_msec) 
    {
      capTouch1.SendNote(min(current_note + 3, SCALE_LEN -1), analog_volume);
    }
  }
  if (capTouch2.GetReading() && capTouch2.midi_needs_update)
  {
    if (millis() > capTouch2.update_midi_msec) 
    {
      capTouch2.SendNote(min(current_note + 5, SCALE_LEN-1), analog_volume);
    }
  }
  if (capTouch3.GetReading() && capTouch3.midi_needs_update)
  {
    if (millis() > capTouch3.update_midi_msec) 
    {
      capTouch3.SendNote(min(current_note + 7, SCALE_LEN-1), analog_volume);
    }
  }

  /* Consider CapTouch sensors as triggered if any of last CAP_TOUCH_ARRAY_LEN samples were high */
  capTouch0.NewNote();
  capTouch1.NewNote();
  capTouch2.NewNote();
  capTouch3.NewNote();

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
  if (micros() >= ping_time)
  {
    /* NOTE: due to using newPing timer, this has to indirectly set range_in_cm */
    ultrasonic.ping_timer(pingCheck);
    ping_time += ping_period;
  }

  if (range_in_cm == 0 || range_in_cm > PITCH_BEND_MAX_CM)
  {
    range_in_cm = PITCH_BEND_MAX_CM;
  }
  
  /* Decide whether to update ultrasonic sensor */
  curr_bend_val = SCALED_PITCH_BEND(range_in_cm);

  update_pitch_bend = false;
  if (curr_bend_val!= prev_bend_val && abs(curr_bend_val- prev_bend_val) < 1000)
  {
    update_pitch_bend= true;
    prev_bend_val = curr_bend_val;
  }
  
  /* Update Pitch Bend and flush usbMIDI message */
  if (update_pitch_bend)
  {
    usbMIDI.sendPitchBend(curr_bend_val, MIDI_CHANNEL_2);
    update_pitch_bend = false;
  }

  if ((analog_volume < TEENSY_MIN_VOLUME) && (prev_analog_volume >= TEENSY_MIN_VOLUME))
  {
    analog_volume = 0;
    usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[capTouch0.current_note], analog_volume, MIDI_CHANNEL_2);
    usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[capTouch1.current_note], analog_volume, MIDI_CHANNEL_2);
    usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[capTouch2.current_note], analog_volume, MIDI_CHANNEL_2);
    usbMIDI.sendNoteOff(IONIAN_SHARP_5_SCALE[capTouch3.current_note], analog_volume, MIDI_CHANNEL_2);
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

  loop_micros = micros() - start_micros;
  
#ifdef DEBUG
  print_loop_time();
#endif /* DEBUG*/
}


void pingCheck(void)
{
  if (ultrasonic.check_timer()) 
  {
    range_in_cm = (ultrasonic.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM
  }
}


/**
 * Just print a quick serial banner- this is to de-clutter setup()
 */
void print_banner(void)
{
  DEBUG_PRINTLN();
  DEBUG_PRINTLN();
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("****************************************");
  DEBUG_PRINTLN("*** Paddle of Theseus Serial Output  ***");
  DEBUG_PRINTLN("****************************************");
  DEBUG_PRINTLN();  
}

void print_loop_time()
{
  DEBUG_PRINT("\rLoop Time: ");
  DEBUG_PRINT(loop_micros);
  DEBUG_PRINT("analog_volume: ");
  DEBUG_PRINT(analogRead(TEENSY_ROT_POT_PIN));
  DEBUG_PRINT("     ");
}
