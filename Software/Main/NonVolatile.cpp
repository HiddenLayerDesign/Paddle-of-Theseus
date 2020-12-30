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
#include "Version.h"
#include "NonVolatile.h"
#include "TeensyBSP.h"


void CheckUpdateVersionNumber(void)
{
  uint8_t eeprom_version_major  = EEPROM.read(EEPROM_VERSION_MAJOR_ADDRESS);
  uint8_t eeprom_version_minor  = EEPROM.read(EEPROM_VERSION_MINOR_ADDRESS);
  uint8_t eeprom_version_bugfix = EEPROM.read(EEPROM_VERSION_BUGFIX_ADDRESS);

  /* Update VERSION_MAJOR in EEPROM if needed */
  if (eeprom_version_major != VERSION_MAJOR)
  {
    EEPROM.write(EEPROM_VERSION_MAJOR_ADDRESS, VERSION_MAJOR);
  } 
  
  /* Update VERSION_MINOR in EEPROM if needed */
  if (eeprom_version_minor != VERSION_MINOR) 
  {
    EEPROM.write(EEPROM_VERSION_MINOR_ADDRESS, VERSION_MINOR);
  }
  
  /* Update VERSION_BUGFIX in EEPROM if needed */
  if (eeprom_version_bugfix != VERSION_BUGFIX)
  {
    EEPROM.write(EEPROM_VERSION_BUGFIX_ADDRESS, VERSION_BUGFIX);
  }
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
    EEPROM.write(EEPROM_CONFIG_MODE_ADDRESS, (int) is_config_mode_enabled);  
  }
}
