/******************************************************* 
 *  File: NonVolatile.hpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __NONVOLATILE_HPP__
#define __NONVOLATILE_HPP__

#include "Arduino.h"
#include "Version.hpp"
#include "RotaryEncoder.hpp"

/**
 * For now, let's keep everything within the TEENSY_LC limit for EEPROM (128B)
 */

/**
 * Store deltas between Keys
 */
#define CT1_DELTA_ADDR    (uint8_t) 0
#define CT2_DELTA_ADDR    (uint8_t) 1
#define CT3_DELTA_ADDR    (uint8_t) 2
#define ROOT_NOTE_ADDR    (uint8_t) 3
#define SCALE_MOD_ADDR    (uint8_t) 4
#define MODE_ENABLED_ADDR (uint8_t) 5
#define CTRL_CHAN_ADDR    (uint8_t) 6
#define OCTAVE_ADDR       (uint8_t) 7
#define PB_CHAN_ADDR      (uint8_t) 8

#define EEPROM_RED_BASE_ADDR (uint8_t) 0x70
#define EEPROM_GREEN_BASE_ADDR (uint8_t) 0x60
#define EEPROM_YELLOW_BASE_ADDR (uint8_t) 0x50
#define EEPROM_BLUE_BASE_ADDR (uint8_t) 0x40
#define EEPROM_PURPLE_BASE_ADDR (uint8_t) 0x30
#define EEPROM_CYAN_BASE_ADDR (uint8_t) 0x20
#define EEPROM_WHITE_BASE_ADDR (uint8_t) 0x10

#define EEPROM_LIMIT EEPROM_RED_BASE_ADDR

/**
 * Check this bit to know whether to go to config menu or regular operation
 */
#define EEPROM_CONFIG_MODE_ADDRESS    (uint8_t) 0
/**
 * Store the Paddle FW version in EEPROM
 */
#define EEPROM_VERSION_MAJOR_ADDRESS  (uint8_t) 1
#define EEPROM_VERSION_MINOR_ADDRESS  (uint8_t) 2
#define EEPROM_VERSION_BUGFIX_ADDRESS (uint8_t) 3

/**
 * 
 */
enum modifier_t
{
  MOD_MAJOR,  
  MOD_MINOR,
  MOD_MIXOLYDIAN,
  MOD_DORIAN,
  MOD_CHROMATIC,
  MOD_LIMIT
};

/**
 * 
 */
typedef struct 
{
  bool is_enabled;
  uint8_t root_note;
  modifier_t modifier;
  uint8_t button1_offset;
  uint8_t button2_offset;
  uint8_t button3_offset;
  uint8_t control_channel;
  uint8_t octave;
  uint8_t pitchbend_channel;
} config_t;

/**
 * List of base addresses matching the enum in RotaryEncoder.h
 */
extern int config_addr_array[ROT_ENC_ENUM_SIZE];
extern const char *color_str_array[ROT_ENC_ENUM_SIZE];
extern const char *modifier_str_array[MOD_LIMIT];
/**
 * Check whether version number in EEPROM matches the version in FW,
 * update the EEPROM version if not
 */
void CheckUpdateVersionNumber(void);

/**
 * Get the version number from EEPROM, return it as a struct
 * 
 * @return struct containing uint8_t ints for MAJOR_, MINOR_, and BUGFIX_VERSION
 */
version_t GetVersionFromEEPROM(void);

/**
 * Return whether config mode is set in EEPROM
 * 
 * @return True == config mode is enabled, else False
 */
bool IsConfigModeEnabled(void);

/**
 * Set config mode in EEPROM based on boolean parameter
 * 
 * @param is_config_mode_enabled [in] True == enable config Mode, else False
 */
void WriteConfigMode(bool is_config_mode_enabled);

/**
 * Fill out a config_t using the values read from the EEPROM
 * 
 * @param state [in] Which config are we reading in
 * @return config_t Struct filled out with the values from EEPROM
 */
config_t loadConfigFromEEPROM(rot_enc_state state);

/**
 * print all EEPROM values
 */
void memdumpEEPROM(void);

/**
 * Compare fields of `in_config` to values in EEPROM, overwrite EEPROM where they don't match
 * 
 * @param in_config [in] Struct holding the full paddle config for this config slot
 * @param state [in] Which config are we writing 
 * @return bool True == one or more writes happened, else false
 */
bool saveConfigToEEPROM(config_t in_config, rot_enc_state state);

#endif /* __NONVOLATILE_HPP__ */
