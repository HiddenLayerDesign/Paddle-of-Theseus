/******************************************************* 
 *  File: ConfigConsole.hpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __CONFIG_CONSOLE_HPP__
#define __CONFIG_CONSOLE_HPP__

#include <Commander.h>

#include "Arduino.h"
#include "RotaryEncoder.hpp"

extern Commander cmd;
extern int myInt;
extern float myFloat;

extern void softRestart(void);

/**
 * Initialize the commander
 */
void initializeCommander(void);

/**
 * Initialize the config for the first time the console boots
 */
void initializeConfig(void);

/**
 * Print config for provided state
 * 
 * @param Cmdr [in] Commander object to use for printing
 * @param state [in] Config state to print
 * @param is_last_config [in] True == this is the last config, don't put a comma after this object
 */
void printConfig(Commander &Cmdr, rot_enc_state state, bool is_last_config);

/**
 * Clobber all config settings and go back to a default version 
 */
void restoreToDefaults(void);

/**
 * Print project info 
 */
bool infoHandler(Commander &Cmdr);

/**
 * Print all of the config
 */
bool printConfigHandler(Commander &Cmdr);

/**
 * Set the MIDI control channel for the current config
 */
bool ctrlChanHandler(Commander &Cmdr);

/**
 * Set the button1 delta for the current config
 */ 
bool button1Handler(Commander &Cmdr);

/**
 * Set the button2 delta for the current config
 */
bool button2Handler(Commander &Cmdr);

/**
 * Set the button3 delta for the current config
 */
bool button3Handler(Commander &Cmdr);

/**
 * Select the current config to be worked on
 */
bool selectColorHandler (Commander &Cmdr);

/**
 * Enable or disable the current config
 */
bool colorEnableHandler(Commander &Cmdr);

/**
 * Reset all configs (enabled or not) to a default
 */
bool defaultsHandler(Commander &Cmdr);

/**
 * Set the root note for the current config
 */
bool rootNoteHandler(Commander &Cmdr);

/**
 * Set the octave for the current config
 */
bool octaveHandler(Commander &Cmdr);

/**
 * Set the modifer for the current config
 */
bool modifierHandler(Commander &Cmdr);

/**
 * Set the channel controlled by pitchbend for the current config
 */
bool pbChanHandler(Commander &Cmdr);

/**
 * Send the heartbeat/ ID message
 */
bool paddlePingHandler(Commander &Cmdr);

/**
 * memDump the EEPROM
 */
bool memDumpHandler(Commander &Cmdr);

/**
 * Exit the menu and reboot
 */
bool exitHandler(Commander &Cmdr);

const commandList_t masterCommands[] = {
  {"about",       infoHandler,       "get information about the paddle, format is `about`"},
  {"all_config", printConfigHandler, "print all stored configuration, format is `all_config`"},
  {"enable",     colorEnableHandler,  "enable or disable this color, format is `enable=TRUE`"},
  {"color",      selectColorHandler, "select current config color, format is `color=CYAN`"},
  {"root_note",  rootNoteHandler,    "set root note of scale, format is `root=A`"},
  {"octave",     octaveHandler,      "set root octave of scale, format is `octave=1`"},
  {"mode",       modifierHandler,    "set mode, format is `mode=MAJOR`"},
  {"offset1",    button1Handler,     "set button1 offset for this color, format is `offset1=3`"},
  {"offset2",    button2Handler,     "set button2 offset for this color, format is `offset2=5`"},
  {"offset3",    button3Handler,     "set button3 offset for this color, format is `offset3=7`"},
  {"control",    ctrlChanHandler,    "set MIDI control change idx for this color, format is `control=20`"},
  {"pitchbend",  pbChanHandler,      "set Pitchbend control change idx for this color, format is `pitchbend=20`"},
  {"defaults",   defaultsHandler,    "reset all MIDI configs to default, format is `defaults`"},
  {"memDump",    memDumpHandler,     "dump all memory in EEPROM"},
  {"paddlePing", paddlePingHandler,  "return `paddlePong`"},
  {"exit",       exitHandler,        "exit this menu and reboot, saving all settings. Format is `exit`"}
};

#endif /* __CONFIG_CONSOLE_HPP__ */
