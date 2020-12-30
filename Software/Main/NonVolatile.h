/******************************************************* 
 *  File: NonVolatile.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __NONVOLATILE_H__
#define __NONVOLATILE_H__

#include "Arduino.h"
#include "Version.h"

/**
 * For now, let's keep everything within the TEENSY_LC limit for EEPROM (128B)
 */

 
/**
 * Store deltas between Keys
 */
#define CT1_DELTA_ADDR (uint8_t) 0
#define CT2_DELTA_ADDR (uint8_t) 1
#define CT3_DELTA_ADDR (uint8_t) 2
#define SCALE_KEY_ADDR (uint8_t) 3
#define SCALE_MOD_ADDR (uint8_t) 4
#define MODE_ENABLED   (uint8_t) 5

#define EEPROM_RED_BASE_ADDR (uint8_t) 0x70
#define EEPROM_GREEN_BASE_ADDR (uint8_t) 0x60
#define EEPROM_YELLOW_BASE_ADDR (uint8_t) 0x50
#define EEPROM_BLUE_BASE_ADDR (uint8_t) 0x40
#define EEPROM_PURPLE_BASE_ADDR (uint8_t) 0x30
#define EEPROM_CYAN_BASE_ADDR (uint8_t) 0x20
#define EEPROM_WHITE_BASE_ADDR (uint8_t) 0x10

/**
 * Check this bit to know whether to go to config menu or regular operation
 */
#define EEPROM_CONFIG_MODE_ADDRESS    (uint8_t) 100

/**
 * Store the Paddle FW version in EEPROM
 */
#define EEPROM_VERSION_MAJOR_ADDRESS  (uint8_t) 125
#define EEPROM_VERSION_MINOR_ADDRESS  (uint8_t) 126
#define EEPROM_VERSION_BUGFIX_ADDRESS (uint8_t) 127

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

#endif /* __NONVOLATILE_H__ */
