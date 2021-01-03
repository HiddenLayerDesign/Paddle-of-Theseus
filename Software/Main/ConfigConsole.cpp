/******************************************************* 
 *  File: ConfigConsole.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#include "ConfigConsole.h"
#include "NonVolatile.h"
#include "RotaryEncoder.h"

#define CHAR_CR 13
#define TRUE_STR "TRUE"
#define FALSE_STR "FALSE"

const char *colorString = "TODO FIXME";
rot_enc_state config_state;
config_t current_config; 

void initializeCommander()
{
  cmd.begin(&Serial, masterCommands, sizeof(masterCommands));
  cmd.endOfLineChar(CHAR_CR);
  cmd.commandPrompt(ON); //enable the command prompt
  cmd.echo(true);     //Echo incoming characters to theoutput port
  cmd.autoChain(ON); 
  cmd.stripCR(OFF);
  cmd.transferTo(masterCommands, sizeof(masterCommands), "Paddle");
}

void initializeConfig()
{
  config_state = ROT_ENC_BLUE;
  current_config = loadConfigFromEEPROM(config_state);
}

bool infoHandler(Commander &Cmdr)
{
  version_t s_version = GetVersionFromEEPROM();
  Cmdr.println();
  Cmdr.print("Paddle of Theseus, version ");
  Cmdr.print(s_version.version_major);
  Cmdr.print(".");
  Cmdr.print(s_version.version_minor);
  Cmdr.print(".");
  Cmdr.print(s_version.version_bugfix);
  Cmdr.println();
  Cmdr.println("By Chase E. Stewart (chasestewart.co) for Hidden Layer Design");
  Cmdr.println("See github.com/chasestewart/Paddle-of-Theseus.git for source");
  Cmdr.println("Special thanks to Eric Atencio, Spencer Arrasmith, Luke Padilla");
  Cmdr.println();
  return 0;
}

bool printConfigHandler(Commander &Cmdr)
{
  Cmdr.println();
  Cmdr.print("*** Current Config: ");
  Cmdr.print(color_str_array[(int) config_state]);
  Cmdr.println(" ***");
  Cmdr.println();

  for (int i=0; i<ROT_ENC_ENUM_SIZE; i++)
  {
    printConfig(Cmdr, i);
  }
  
  return 0;
}

bool selectColorHandler(Commander &Cmdr)
{
  char *compare = NULL;

  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `color=BLUE`!");
    return false;
  }

  if (NULL == (compare = Cmdr.getPayloadString().c_str()))
  {
    return false;  
  }
  
  if (!strncmp(compare, color_str_array[ROT_ENC_BLUE], strlen(color_str_array[ROT_ENC_BLUE])))
  {
    config_state = ROT_ENC_BLUE;
  }
  else if (!strncmp(compare, color_str_array[ROT_ENC_CYAN], strlen(color_str_array[ROT_ENC_CYAN])))
  {
    config_state = ROT_ENC_CYAN;
  }
  else if (!strncmp(compare, color_str_array[ROT_ENC_GREEN], strlen(color_str_array[ROT_ENC_GREEN])))
  {
    config_state = ROT_ENC_GREEN;
  }
  else if (!strncmp(compare, color_str_array[ROT_ENC_PURPLE], strlen(color_str_array[ROT_ENC_PURPLE])))
  {
    config_state = ROT_ENC_PURPLE;
  }
  else if (!strncmp(compare, color_str_array[ROT_ENC_RED], strlen(color_str_array[ROT_ENC_RED])))
  {
    config_state = ROT_ENC_RED;
  }
  else if (!strncmp(compare, color_str_array[ROT_ENC_YELLOW], strlen(color_str_array[ROT_ENC_WHITE])))
  {
    config_state = ROT_ENC_YELLOW;
  }
  else if (!strncmp(compare, color_str_array[ROT_ENC_WHITE], strlen(color_str_array[ROT_ENC_WHITE])))
  {
    config_state = ROT_ENC_WHITE;
  }
  else 
  {
    Cmdr.println("ERROR: only accepted values are \"BLUE\", \"CYAN\", \"GREEN\", \"PURPLE\", \"RED\",  \"YELLOW\", \"WHITE\"!");
    return false;  
  }
  
  Cmdr.print("Paddle: Changed current config to ");
  Cmdr.println(color_str_array[(int) config_state]);
  return true;
}

bool colorEnableHandler(Commander &Cmdr)
{
  char *compare = NULL;
  const char *color_str = color_str_array[(int) config_state];

  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `enable=TRUE`!");
    return false;
  }

  if (NULL == (compare = Cmdr.getPayloadString().c_str()))
  {
    return false;  
  }

  if (!strncmp(compare, TRUE_STR, strlen(TRUE_STR)))
  {
    Cmdr.print(color_str);
    Cmdr.println(": Enabled = TRUE");
    current_config.is_enabled = true;
    saveConfigToEEPROM(current_config, config_state);
    return true;
  }
  else if (!strncmp(compare, FALSE_STR, strlen(FALSE_STR)))
  {
    Cmdr.print(color_str);
    Cmdr.println(": Enabled = FALSE");
    current_config.is_enabled = false;
    saveConfigToEEPROM(current_config, config_state);
    return true;
  }
  else
  {
    Cmdr.println("ERROR: only accepted values are \"TRUE\" and \"FALSE\"!");
    return false;
  }  
}

bool rootNoteHandler(Commander &Cmdr)
{
  int myInt;
  const char *color_str = color_str_array[(int) config_state];
  char *compare = NULL;

  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `root=C#`!");
    return false;
  }

  if (NULL == (compare = Cmdr.getPayloadString().c_str()))
  {
    Cmdr.println("ERROR: only accepted values are \"A\", \"B\", \"C\", \"D\", \"E\", \"F\", \"G\"!");
    return false;  
  }

  if (!strncmp(compare, "C#", strlen("C#")))
  {
    myInt = 25;
  }
  else if (!strncmp(compare, "C", strlen("C")))
  {
    myInt = 24;
  }
  else if (!strncmp(compare, "D#", strlen("D#")))
  {
    myInt = 27;
  }
  else if (!strncmp(compare, "D", strlen("D")))
  {
    myInt = 26;
  }
  else if (!strncmp(compare, "E", strlen("E")))
  {
    myInt = 28;
  }
  else if (!strncmp(compare, "F#", strlen("F#")))
  {
    myInt = 30;
  }
  else if (!strncmp(compare, "F", strlen("F")))
  {
    myInt = 29;
  }
  else if (!strncmp(compare, "G#", strlen("G#")))
  {
    myInt = 32;
  }
  else if (!strncmp(compare, "G", strlen("G")))
  {
    myInt = 31;
  }
  else if (!strncmp(compare, "A#", strlen("A#")))
  {
    myInt = 34;
  }
  else if (!strncmp(compare, "A", strlen("A")))
  {
    myInt = 33;
  }
  else if (!strncmp(compare, "B", strlen("B")))
  {
    myInt = 35;
  }
  else
  {
    Cmdr.println("ERROR: only accepted values are \"C\", \"C#\", \"D\", \"D#\", \"E\", \"F\", \"G\", \"G#\", \"A\", \"A#\", \"B\"!");
    return false;
  }
  Cmdr.print(color_str);
  Cmdr.print(": Setting root note to ");
  Cmdr.println(myInt);
  current_config.root_note = myInt;
  saveConfigToEEPROM(current_config, config_state);
  return true;
}

bool modifierHandler(Commander &Cmdr)
{
  char *compare = NULL;
  const char *color_str = color_str_array[(int) config_state];
  
  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `modifier=MINOR`!");
    return false;
  }

  if (NULL == (compare = Cmdr.getPayloadString().c_str()))
  {
    Cmdr.println("ERROR: only accepted values are \"MAJOR\", \"MINOR\", \"PENT_MAJOR\", \"PENT_MINOR\"!");
    return false;  
  }

  if (!strncmp(compare, modifier_str_array[MOD_MAJOR], strlen(modifier_str_array[MOD_MAJOR])))
  {
    current_config.modifier = MOD_MAJOR;
  }
  else if (!strncmp(compare, modifier_str_array[MOD_MINOR], strlen(modifier_str_array[MOD_MINOR])))
  {
    current_config.modifier = MOD_MINOR;
  }
  else if (!strncmp(compare, modifier_str_array[MOD_MIXOLYTIAN], strlen(modifier_str_array[MOD_MIXOLYTIAN])))
  {
    current_config.modifier = MOD_MIXOLYTIAN;
  }
  else if (!strncmp(compare, modifier_str_array[MOD_DORIAN], strlen(modifier_str_array[MOD_DORIAN])))
  {
    current_config.modifier = MOD_DORIAN;
  }
  else
  {
    Cmdr.println("ERROR: only accepted values are \"MAJOR\", \"MINOR\", \"MIXOLYTIAN\", \"DORIAN\"!");
    return false;
  }
  Cmdr.print(color_str);
  Cmdr.print(": Setting modifier to ");
  Cmdr.println(modifier_str_array[current_config.modifier]);
  saveConfigToEEPROM(current_config, config_state);
  return true;
}

bool button1Handler(Commander &Cmdr)
{
  if (Cmdr.getInt(myInt))
  {
    const char *color_str = color_str_array[(int) config_state];
    Cmdr.print(color_str);
    Cmdr.print(": Setting button 1 offset to ");
    Cmdr.println(myInt);
    current_config.button1_offset = myInt;
    saveConfigToEEPROM(current_config, config_state);
  }
  return true;
}

bool button2Handler(Commander &Cmdr)
{
  if (Cmdr.getInt(myInt))
  {
    const char *color_str = color_str_array[(int) config_state];
    Cmdr.print(color_str);
    Cmdr.print(": Setting button 2 offset to ");
    Cmdr.println(myInt);
    current_config.button2_offset = myInt;
    saveConfigToEEPROM(current_config, config_state);
  }
  return true;
}

bool button3Handler(Commander &Cmdr)
{
  if (Cmdr.getInt(myInt))
  {
    const char *color_str = color_str_array[(int) config_state];
    Cmdr.print(color_str);
    Cmdr.print(": Setting button 3 offset to ");
    Cmdr.println(myInt);
    current_config.button3_offset = myInt;
    saveConfigToEEPROM(current_config, config_state);
  }
  return true;
}

bool ctrlChanHandler(Commander &Cmdr)
{
  if (Cmdr.getInt(myInt))
  {
    const char *color_str = color_str_array[(int) config_state];
    Cmdr.print(color_str);
    Cmdr.print(": Setting MIDI control channel to ");
    Cmdr.println(myInt);
    current_config.control_channel = myInt;
    saveConfigToEEPROM(current_config, config_state);
  }
  return true;
}

bool saveColorHandler(Commander &Cmdr)
{
  saveConfigToEEPROM(current_config, config_state);
  const char *color_str = color_str_array[(int) config_state];
  Cmdr.print(color_str);
  Cmdr.println(": Saved color handler");
  return true;
}

bool exitHandler(Commander &Cmdr)
{
  Cmdr.println("Saving settings!");
  Cmdr.println("Going down for reboot now!");
  delay(5000);
  softRestart();
  
  /* Will likely never get here, but harmless anyways */
  return true;
}

void printConfig(Commander &Cmdr, rot_enc_state state)
{
  config_t print_config = loadConfigFromEEPROM(state);
  
  Cmdr.print("Color=");
  Cmdr.println(color_str_array[(int) state]);

  Cmdr.print("\tEnable=");
  Cmdr.println((print_config.is_enabled) ? "True " : "False");

  if (print_config.is_enabled)
  {
    Cmdr.print("\tButton_1=");
    Cmdr.print(print_config.button1_offset);
    Cmdr.print(", Button_2=");
    Cmdr.print(print_config.button2_offset);
    Cmdr.print(", Button_3=");
    Cmdr.println(print_config.button3_offset);
  
    Cmdr.print("\tRoot_Note=");
    Cmdr.println(print_config.root_note);
    Cmdr.print("\tModifier=");
    Cmdr.println(modifier_str_array[print_config.modifier]);
    
    Cmdr.print("\tMIDI_Control_channel=0x");
    Cmdr.println(print_config.control_channel, HEX);  
  }
}

bool defaultsHandler(Commander &Cmdr)
{
  config_t default_config;

  default_config.is_enabled = true;
  default_config.root_note=24;
  default_config.modifier = MOD_MAJOR;
  default_config.button1_offset = 3;
  default_config.button2_offset = 5;
  default_config.button3_offset = 7;
  default_config.control_channel = 16;

  for (int i=0; i < ROT_ENC_ENUM_SIZE; i++)
  {
    saveConfigToEEPROM(default_config, i);
  }
  Cmdr.println("INFO: Reset ALL configs to default values");
  return true;
}
