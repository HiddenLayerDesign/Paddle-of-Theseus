/******************************************************* 
 *  File: NonVolatile.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#include<EEPROM.h>

#include "Arduino.h"
#include "Version.hpp"
#include "NonVolatile.hpp"
#include "BoardLayout.hpp"

int config_addr_array[ROT_ENC_ENUM_SIZE] = {EEPROM_BLUE_BASE_ADDR, 
                                            EEPROM_CYAN_BASE_ADDR, 
                                            EEPROM_GREEN_BASE_ADDR, 
                                            EEPROM_PURPLE_BASE_ADDR, 
                                            EEPROM_RED_BASE_ADDR, 
                                            EEPROM_YELLOW_BASE_ADDR, 
                                            EEPROM_WHITE_BASE_ADDR};

const char *color_str_array[ROT_ENC_ENUM_SIZE] = {"BLUE", 
                                                  "CYAN", 
                                                  "GREEN", 
                                                  "PURPLE", 
                                                  "RED", 
                                                  "YELLOW", 
                                                  "WHITE"};

const char *modifier_str_array[MOD_LIMIT] = {"MAJOR",
                                             "MINOR",
                                             "MIXOLYDIAN",
                                             "DORIAN",
                                             "CHROMATIC"};
void CheckUpdateVersionNumber(void)
{
  uint8_t eeprom_version_major  = EEPROM.read(EEPROM_VERSION_MAJOR_ADDRESS);
  uint8_t eeprom_version_minor  = EEPROM.read(EEPROM_VERSION_MINOR_ADDRESS);
  uint8_t eeprom_version_bugfix = EEPROM.read(EEPROM_VERSION_BUGFIX_ADDRESS);

  /* Update VERSION bytes in EEPROM if needed */
  EEPROM.update(EEPROM_VERSION_MAJOR_ADDRESS, VERSION_MAJOR);
  EEPROM.update(EEPROM_VERSION_MINOR_ADDRESS, VERSION_MINOR);
  EEPROM.update(EEPROM_VERSION_BUGFIX_ADDRESS, VERSION_BUGFIX);
}

version_t GetVersionFromEEPROM(void)
{
  version_t return_version = {0};
  return_version.version_major =   EEPROM.read(EEPROM_VERSION_MAJOR_ADDRESS);
  return_version.version_minor =   EEPROM.read(EEPROM_VERSION_MINOR_ADDRESS);
  return_version.version_bugfix =   EEPROM.read(EEPROM_VERSION_BUGFIX_ADDRESS);
  return return_version;
}

bool IsConfigModeEnabled(void)
{
  return (0 != EEPROM.read(EEPROM_CONFIG_MODE_ADDRESS));  
}

void WriteConfigMode(bool is_config_mode_enabled)
{
  uint8_t check_eeprom_value = EEPROM.read(EEPROM_CONFIG_MODE_ADDRESS);
  if (check_eeprom_value != is_config_mode_enabled)
  {
    DEBUG_PRINT("EEPROM: Writing config mode = ");
    DEBUG_PRINTLN((is_config_mode_enabled) ? "True " : "False");
    EEPROM.update(EEPROM_CONFIG_MODE_ADDRESS, (int) is_config_mode_enabled);  
  }
}

config_t loadConfigFromEEPROM(rot_enc_state state)
{
  config_t return_config;
  int base_addr = config_addr_array[(int) state];

  /* fill out config_t */
  return_config.is_enabled        = (bool) (0 == EEPROM.read(base_addr + MODE_ENABLED_ADDR)) ? false : true;
  return_config.button1_offset    = (int) EEPROM.read(base_addr + CT1_DELTA_ADDR);
  return_config.button2_offset    = (int) EEPROM.read(base_addr + CT2_DELTA_ADDR);
  return_config.button3_offset    = (int) EEPROM.read(base_addr + CT3_DELTA_ADDR);
  return_config.root_note         = (int) EEPROM.read(base_addr + ROOT_NOTE_ADDR);
  return_config.octave            = (int) EEPROM.read(base_addr + OCTAVE_ADDR);
  return_config.control_channel   = (int) EEPROM.read(base_addr + CTRL_CHAN_ADDR);
  return_config.pitchbend_channel = (int) EEPROM.read(base_addr + PB_CHAN_ADDR);
  return_config.modifier          = (modifier_t) EEPROM.read(base_addr + SCALE_MOD_ADDR);
  if (return_config.modifier > MOD_LIMIT)
  {
    return_config.modifier = MOD_MAJOR;
  }  
  
  return return_config;
}

bool saveConfigToEEPROM(config_t in_config, rot_enc_state state)
{    
  int base_addr = config_addr_array[(int) state];
  const char *color_str = color_str_array[(int) state];
  (void) color_str;
  bool retval = false;
  
  if (in_config.is_enabled != EEPROM.read(base_addr + MODE_ENABLED_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing mode enabled = ");
    DEBUG_PRINTLN((in_config.is_enabled) ? "True " : "False");
    EEPROM.update(base_addr + MODE_ENABLED_ADDR, (int) in_config.is_enabled);  
    retval = true;
  } 

  if (in_config.root_note != EEPROM.read(base_addr + ROOT_NOTE_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing root note = ");
    DEBUG_PRINTLN(in_config.root_note);
    EEPROM.update(base_addr + ROOT_NOTE_ADDR, in_config.root_note);  
    retval = true;
  } 

  if (in_config.octave != EEPROM.read(base_addr + OCTAVE_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing octave = ");
    DEBUG_PRINTLN(in_config.octave);
    EEPROM.update(base_addr + OCTAVE_ADDR, in_config.octave);  
    retval = true;
  } 

  if (in_config.modifier != EEPROM.read(base_addr + SCALE_MOD_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing modifier = ");
    DEBUG_PRINTLN(in_config.modifier);
    EEPROM.update(base_addr + SCALE_MOD_ADDR, (int) in_config.modifier);  
    retval = true;
  } 

  if (in_config.button1_offset != EEPROM.read(base_addr + CT1_DELTA_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing button1 delta = ");
    DEBUG_PRINTLN(in_config.button1_offset);
    EEPROM.update(base_addr + CT1_DELTA_ADDR, (int) in_config.button1_offset);  
    retval = true;
  }
  
  if (in_config.button2_offset != EEPROM.read(base_addr + CT2_DELTA_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing button2 delta = ");
    DEBUG_PRINTLN(in_config.button2_offset);
    EEPROM.update(base_addr + CT2_DELTA_ADDR, (int) in_config.button2_offset);  
    retval = true;
  }
  
  if (in_config.button3_offset != EEPROM.read(base_addr + CT3_DELTA_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing button3 delta = ");
    DEBUG_PRINTLN(in_config.button3_offset);
    EEPROM.update(base_addr + CT3_DELTA_ADDR, (int) in_config.button3_offset);  
    retval = true;
  }

  if (in_config.control_channel != EEPROM.read(base_addr + CTRL_CHAN_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing MIDI control channel = ");
    DEBUG_PRINTLN(in_config.control_channel);
    EEPROM.update(base_addr + CTRL_CHAN_ADDR, (int) in_config.control_channel);  
    retval = true;
  }

  if (in_config.pitchbend_channel != EEPROM.read(base_addr + PB_CHAN_ADDR))
  {
    DEBUG_PRINT(color_str);
    DEBUG_PRINT(": Writing Pitchbend control channel = ");
    DEBUG_PRINTLN(in_config.pitchbend_channel);
    EEPROM.update(base_addr + PB_CHAN_ADDR, (int) in_config.pitchbend_channel);  
    retval = true;
  }
  
  return retval;  
}

void memdumpEEPROM(void)
{
  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("*** DUMPING EEPROM ***");
  uint8_t value;

  for (int i=0; i <= EEPROM_LIMIT; i += 0x10 )
  {
    DEBUG_PRINT("0x");
    DEBUG_PRINT_HEX(i);
    if (i<0x10)
    {
      DEBUG_PRINT("0");
    }
    DEBUG_PRINT(": ");
    for (int j=0; j < 0x10; j++)
    {
      value = EEPROM.read(i+j);
      DEBUG_PRINT(" ");
      if (value < 0x10)
      {
        DEBUG_PRINT("0");
      }
      DEBUG_PRINT_HEX(value);      
    }
    DEBUG_PRINTLN("");
  }
  DEBUG_PRINTLN("*** DONE ***");
  DEBUG_PRINTLN("");
}
