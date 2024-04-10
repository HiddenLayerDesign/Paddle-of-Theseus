/******************************************************* 
 *  File: Main.ino
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
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
#include "BoardLayout.hpp"
#include "ConfigConsole.hpp"
#include "MIDIConstants.hpp"
#include "MMA8452Q.hpp"
#include "NonVolatile.hpp"
#include "Preferences.hpp"
#include "QTouchBoard.hpp"
#include "RotaryEncoder.hpp"
#include "StrumButton.hpp"
#include "Ultrasonic.hpp"
#include "Version.hpp"

/* Static Prototypes */
static int getVolume(bool is_lefty_flipped);
static bool PollForSerial(unsigned long flip_time, bool is_green_not_yellow);
static void configurePins(void);
static void pingCheck(void);

/* external Prototypes */
void softRestart(void);

#ifdef DEBUG
static void printLoopDebugInfo(void);
static void printBanner(void);
#endif /* DEBUG */

/* Sensor class variables */
NewPing ultrasonic(PIN_ULTRA_TRIG, PIN_ULTRA_SENS, PITCH_BEND_MAX_CM+1);
Encoder RotaryEncoder = Encoder(PIN_ROT_ENC_A, PIN_ROT_ENC_C);
QTouchBoard fretBoard = QTouchBoard(PIN_FRET_1070_INT, PIN_FRET_2120_INT);
QTouchBoard strumBoard = QTouchBoard(PIN_STRUM_1070_INT, PIN_STRUM_2120_INT);
StrumButton button0 = StrumButton(STRUM_BUTTON_0);
StrumButton button1 = StrumButton(STRUM_BUTTON_1);
StrumButton button2 = StrumButton(STRUM_BUTTON_2);
StrumButton button3 = StrumButton(STRUM_BUTTON_3);

MMA8452Q accel;
Commander cmd;

int myInt = 0;
float myFloat = 0.0;

/* Ultrasonic Pitch Bend variables */
unsigned long ping_time;
unsigned long range_in_us;
unsigned long range_in_cm;
int curr_bend_val = 1;
int prev_bend_val = 0;

/* Rotary Potentiometer variables */
int analog_volume = 0;
int prev_analog_volume = 0;

// QTouchBoard variables
uint8_t strumStatus0, strumStatus1, strumStatus2;
uint8_t keyStatus0, keyStatus1, keyStatus2;

/* MIDI variables */
int current_fret = 0;
uint8_t strum_keys = 0;

/* Rotary Encoder Variables */
int curr_enc_reading = 0;
int prev_enc_reading = 1;
int constrained_enc_reading = 0;
enum rot_enc_state encoder_state = (rot_enc_state) ((int)ROT_ENC_ENUM_SIZE - 1);
int curr_rot_button = HIGH;
int prev_rot_button = HIGH;

/* General variables */
unsigned long flip_time;
bool is_lefty_flipped = false;
bool is_config_mode = false;
bool is_green_not_yellow = false;
config_t running_config;

/*
 * Setup PinModes and Serial port, Init digital sensors 
 */
void setup() 
{
  configurePins();    
  CheckUpdateVersionNumber();

  Serial.begin(9600);

  if (true == (is_config_mode = IsConfigModeEnabled()))
  {
    RotEncConfigPattern();
    initializeCommander();
    
    while (!Serial)
    {
      if (PollForSerial(flip_time, is_green_not_yellow))
      {
        is_green_not_yellow = !is_green_not_yellow;
        flip_time = millis() + 500;
      }
    }        
    RotEncSetLED(LED_YELLOW);
    initializeConfig();
    version_t s_version = GetVersionFromEEPROM();

    Serial.print("*** Paddle of Theseus Configuration Mode, Version ");
    Serial.print(s_version.version_major);
    Serial.print(".");
    Serial.print(s_version.version_minor);
    Serial.print(".");
    Serial.print(s_version.version_bugfix);
    Serial.println(" ***");
    Serial.println("Usage: Type \"help\" and then `Enter` key to get help");
    Serial.println("For all commands that take values, type \"command=VALUE\" and then `Enter` key");
    cmd.printCommandPrompt(); 
  }
  else
  {
    RotEncStandardPattern();

#ifdef DEBUG
    flip_time = millis() + 500;
    is_green_not_yellow = true;

    while (!Serial)
    {
      if (PollForSerial(flip_time, is_green_not_yellow))
      {
        is_green_not_yellow = !is_green_not_yellow;
        flip_time = millis() + 500;
      }
    }

    if (CrashReport) 
    {
      Serial.print(CrashReport);
      delay(5000);
    }
    printBanner();
#endif /* DEBUG */

    int num_disabled_configs = 0;
    /* Select the next enabled configuration */
    encoder_state = (rot_enc_state) encoder_state;
    
    while (!running_config.is_enabled)
    {
      encoder_state = (rot_enc_state) ((encoder_state + 1) % ROT_ENC_ENUM_SIZE);
      num_disabled_configs += 1;

      if (num_disabled_configs == ROT_ENC_ENUM_SIZE)
      {
        encoder_state = ROT_ENC_BLUE;
        running_config = loadConfigFromEEPROM(encoder_state);
        running_config.is_enabled = true;
        saveConfigToEEPROM(running_config, encoder_state);
        break;
      }
    }

    Wire.begin();
    fretBoard.begin(Wire);
    Wire1.begin();
    strumBoard.begin(Wire1);
    
    running_config = loadConfigFromEEPROM(encoder_state);
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
  /**
   * If in config mode, only Commander runs
   */
  if (is_config_mode)
  {
    cmd.update();
  }
  else
  {
    /* Check Lefty Flip status */
    accel.accel_update();
    is_lefty_flipped = accel.is_lefty_flipped();
  
    /* Sample Rotary Encoder Pushbutton */
    curr_rot_button = digitalRead(PIN_ROT_ENC_SW);
    if (curr_rot_button == HIGH && prev_rot_button == LOW)  
    {
      /**
       * Reboot into config mode if config gesture entered
       */
      if ((strumBoard.ReturnStrumKey(strumStatus0, strumStatus1, strumStatus2) & 0x0F) == 0x0F)
      {
          DEBUG_PRINT("Setting Config Mode!");
          WriteConfigMode(true);
          RotEncConfigPattern();
          softRestart();  
      }

      /* Select the next enabled configuration */
      do
      {
        encoder_state = (rot_enc_state) ((encoder_state + 1) % ROT_ENC_ENUM_SIZE);
        running_config = loadConfigFromEEPROM(encoder_state);
      } while (!running_config.is_enabled);

      /* Set the LED color to the next enabled config */
      RotEncSetLED(rot_enc_led_color_array[encoder_state]);
    }
    prev_rot_button = curr_rot_button;
    
    /* Sample Rotary Encoder Twist Knob */
    constrained_enc_reading = ProcessRotEnc(RotaryEncoder.read(), is_lefty_flipped); 
    RotaryEncoder.write( (is_lefty_flipped) ? 
                       (-1 *constrained_enc_reading) : constrained_enc_reading);
       
    if (constrained_enc_reading != prev_enc_reading)
    {
      usbMIDI.sendControlChange(running_config.control_channel, constrained_enc_reading, MIDI_CHANNEL_2);
    }
    prev_enc_reading = constrained_enc_reading;
  
    /* Read Fretboard */ 
    if (true) // TODO fix this to read from GPIO Interrupt pins
    {    
      keyStatus0 = fretBoard.GetLeastSigMask();
      keyStatus1 = fretBoard.GetMiddleMask();
      keyStatus2 = fretBoard.GetMostSigMask();
      current_fret = fretBoard.ReturnFret(keyStatus0, keyStatus1, keyStatus2);
    }

    /* Read Strumboard */
    if (strumBoard.isValueUpdate())
    {
      strumStatus0 = strumBoard.GetLeastSigMask();
      strumStatus1 = strumBoard.GetMiddleMask();
      strumStatus2 = strumBoard.GetMostSigMask();
      strum_keys = strumBoard.ReturnStrumKey(strumStatus0, strumStatus1, strumStatus2);
    }
     
    /* Send note on debounced rising edge of TEENSY_CAP_TOUCH1_PIN */
    button0.Update(strum_keys & 0x01);
    button1.Update(strum_keys & 0x02);
    button2.Update(strum_keys & 0x04);
    button3.Update(strum_keys & 0x08);

    if (button0.ShouldSendNote()) 
      button0.SendNote(current_fret, analog_volume, is_lefty_flipped,running_config);
    if (button1.ShouldSendNote()) 
      button1.SendNote(current_fret, analog_volume, is_lefty_flipped,running_config);
    if (button2.ShouldSendNote())
      button2.SendNote(current_fret, analog_volume, is_lefty_flipped,running_config);
    if (button3.ShouldSendNote())
      button3.SendNote(current_fret, analog_volume, is_lefty_flipped,running_config);

    button0.CheckMIDINeedsUpdate();
    button1.CheckMIDINeedsUpdate();
    button2.CheckMIDINeedsUpdate();
    button3.CheckMIDINeedsUpdate();

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
      if (running_config.pitchbend_channel == MIDI_CTRL_CHG_PITCHBEND)
      {
        usbMIDI.sendPitchBend(curr_bend_val, MIDI_CHANNEL_2);
      }
      else if (running_config.pitchbend_channel < MIDI_CTRL_CHG_MAX)
      {        
        usbMIDI.sendControlChange(running_config.pitchbend_channel, ONEBYTE_SCALED_PITCH_BEND(range_in_cm), MIDI_CHANNEL_2);
      }
      else if (running_config.pitchbend_channel == MIDI_CTRL_CHG_INVAL)
      {
        // don't send pitchbend            
      }
      else // any invalid Control Change input value should be ignored
      {
        // also don't send pitchbend                    
      }
      prev_bend_val = curr_bend_val;
    }
  
    /* set volume */
    analog_volume = getVolume(is_lefty_flipped);
    constrain(analog_volume, 0, 127);
    if (analog_volume < TEENSY_MIN_VOLUME)
    {
      analog_volume = 0;
      if (prev_analog_volume >= TEENSY_MIN_VOLUME)
      {
        usbMIDI.sendNoteOff(button0.GetCurrentNote(), 0, MIDI_CHANNEL_2);
        usbMIDI.sendNoteOff(button1.GetCurrentNote(), 0, MIDI_CHANNEL_2);
        usbMIDI.sendNoteOff(button2.GetCurrentNote(), 0, MIDI_CHANNEL_2);
        usbMIDI.sendNoteOff(button3.GetCurrentNote(), 0, MIDI_CHANNEL_2);
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
    
#ifdef DEBUG
    printLoopDebugInfo();
#endif  /* DEBUG */
  }
}

/**
 * Callback function to check whether ultrasonic sonar has returned data
 */
static void pingCheck(void)
{
  range_in_us = (ultrasonic.check_timer()) ? ultrasonic.ping_result : range_in_us +2;
}

/**
 * Do static configuration of pins as INPUT or OUTPUT
 */
static void configurePins(void)
{
  /* Set input sensor pins */
  pinMode(PIN_FRET_1070_INT, INPUT);
  pinMode(PIN_FRET_2120_INT, INPUT);
  pinMode(PIN_STRUM_1070_INT, INPUT);
  pinMode(PIN_STRUM_2120_INT, INPUT);
  pinMode(PIN_ROT_ENC_SW, INPUT);
  pinMode(PIN_ROT_POT, INPUT);

  /* The rotary switch LEDS are common anode with external pulldown */
  pinMode(PIN_ROT_LEDB, OUTPUT);
  pinMode(PIN_ROT_LEDG, OUTPUT);
  pinMode(PIN_ROT_LEDR, OUTPUT);  

  pinMode(PIN_TEENSY_LED, OUTPUT);
}

/**
 * Return the instrument volume, taking into account whether a left- or right-handed person is using the paddle
 * 
 * @param is_lefty_flipped [in] True == the paddle is currently lefty-flipped, else False
 * @return int Volume between 0-127 where 127 is MAX volume
 */
int getVolume(bool is_lefty_flipped)
{
  return (is_lefty_flipped) ?
            floor(analogRead(PIN_ROT_POT) * 128.0/1024.0) :
            floor((1024 - analogRead(PIN_ROT_POT)) * 128.0/1024.0);
}

/**
 * Just print a serial banner- this is to de-clutter setup()
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
  DEBUG_PRINT("\rVolume: ");
  DEBUG_PRINT(analog_volume);
  DEBUG_PRINT("     ");
}

/**
 * Reset the Teensy in software
 */
void softRestart(void) 
{
  Serial.end();  // clears the serial monitor  if used
  REG_SCB_AIRCR = VAL_SCB_AIRCR_RESET;  // write value for restart
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
