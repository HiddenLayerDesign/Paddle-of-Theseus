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
#include "MIDIConstants.h"

#define CHAR_CR 13
#define TRUE_STR "TRUE"
#define FALSE_STR "FALSE"

rot_enc_state config_state;
config_t current_config; 

void initializeCommander()
{
  cmd.begin(&Serial, masterCommands, sizeof(masterCommands));
  cmd.endOfLineChar(CHAR_CR);
  cmd.commandPrompt(ON); //enable the command prompt
  cmd.echo(false);     //Echo incoming characters to theoutput port
  cmd.errorMessages(false);
  cmd.autoChain(OFF); // TODO FIXME test
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
  Cmdr.print("{\"current_config\": \"");
  Cmdr.print(color_str_array[(int) config_state]);
  Cmdr.print("\",");
  Cmdr.print(" \"all_configs\": {");

  for (int i=0; i<ROT_ENC_ENUM_SIZE; i++)
  {
    Cmdr.print("\"");
    Cmdr.print(color_str_array[i]);
    Cmdr.print("\": ");
    printConfig(Cmdr, i, (i == ROT_ENC_ENUM_SIZE-1));
  }
  Cmdr.println("}}");
  return 0;
}

bool selectColorHandler(Commander &Cmdr)
{
  String compare = NULL;

  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `color=BLUE`!");
    return false;
  }

  compare = Cmdr.getPayloadString();
  if ("" == compare.c_str())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `color=BLUE`!");
    return false;  
  }
  
  if (!strncmp(compare.c_str(), color_str_array[ROT_ENC_BLUE], strlen(color_str_array[ROT_ENC_BLUE])))
  {
    config_state = ROT_ENC_BLUE;
  }
  else if (!strncmp(compare.c_str(), color_str_array[ROT_ENC_CYAN], strlen(color_str_array[ROT_ENC_CYAN])))
  {
    config_state = ROT_ENC_CYAN;
  }
  else if (!strncmp(compare.c_str(), color_str_array[ROT_ENC_GREEN], strlen(color_str_array[ROT_ENC_GREEN])))
  {
    config_state = ROT_ENC_GREEN;
  }
  else if (!strncmp(compare.c_str(), color_str_array[ROT_ENC_PURPLE], strlen(color_str_array[ROT_ENC_PURPLE])))
  {
    config_state = ROT_ENC_PURPLE;
  }
  else if (!strncmp(compare.c_str(), color_str_array[ROT_ENC_RED], strlen(color_str_array[ROT_ENC_RED])))
  {
    config_state = ROT_ENC_RED;
  }
  else if (!strncmp(compare.c_str(), color_str_array[ROT_ENC_YELLOW], strlen(color_str_array[ROT_ENC_WHITE])))
  {
    config_state = ROT_ENC_YELLOW;
  }
  else if (!strncmp(compare.c_str(), color_str_array[ROT_ENC_WHITE], strlen(color_str_array[ROT_ENC_WHITE])))
  {
    config_state = ROT_ENC_WHITE;
  }
  else 
  {
    Cmdr.println("ERROR: only accepted values are \"BLUE\", \"CYAN\", \"GREEN\", \"PURPLE\", \"RED\",  \"YELLOW\", \"WHITE\"!");
    return false;  
  }

  current_config = loadConfigFromEEPROM(config_state);
  Cmdr.print("SUCCESS: Changed current config to ");
  Cmdr.println(color_str_array[(int) config_state]);
  return true;
}

bool colorEnableHandler(Commander &Cmdr)
{
  String compare = NULL;
  const char *color_str = color_str_array[(int) config_state];

  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `enable=TRUE`!");
    return false;
  }

  compare = Cmdr.getPayloadString();
  if ("" == compare.c_str())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `enable=TRUE`!");
    return false;  
  }

  if (!strncmp(compare.c_str(), TRUE_STR, strlen(TRUE_STR)))
  {
    Cmdr.print(color_str);
    Cmdr.println(": Enabled = TRUE");
    current_config.is_enabled = true;
    saveConfigToEEPROM(current_config, config_state);
    return true;
  }
  else if (!strncmp(compare.c_str(), FALSE_STR, strlen(FALSE_STR)))
  {
    Cmdr.println("SUCCESS: Enabled = FALSE");
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
  String compare = NULL;

  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `root=C#`!");
    return false;
  }

  compare = Cmdr.getPayloadString();
  if ("" == compare.c_str())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `root=C#`!");
    return false;  
  }

  if (!strncmp(compare.c_str(), "C#", strlen("C#")))
  {
    myInt = ROOT_NOTE_CSHARP;
  }
  else if (!strncmp(compare.c_str(), "C", strlen("C")))
  {
    myInt = ROOT_NOTE_C;
  }
  else if (!strncmp(compare.c_str(), "D#", strlen("D#")))
  {
    myInt = ROOT_NOTE_DSHARP;
  }
  else if (!strncmp(compare.c_str(), "D", strlen("D")))
  {
    myInt = ROOT_NOTE_D;
  }
  else if (!strncmp(compare.c_str(), "E", strlen("E")))
  {
    myInt = ROOT_NOTE_E;
  }
  else if (!strncmp(compare.c_str(), "F#", strlen("F#")))
  {
    myInt = ROOT_NOTE_FSHARP;
  }
  else if (!strncmp(compare.c_str(), "F", strlen("F")))
  {
    myInt = ROOT_NOTE_F;
  }
  else if (!strncmp(compare.c_str(), "G#", strlen("G#")))
  {
    myInt = ROOT_NOTE_GSHARP;
  }
  else if (!strncmp(compare.c_str(), "G", strlen("G")))
  {
    myInt = ROOT_NOTE_G;
  }
  else if (!strncmp(compare.c_str(), "A#", strlen("A#")))
  {
    myInt = ROOT_NOTE_ASHARP;
  }
  else if (!strncmp(compare.c_str(), "A", strlen("A")))
  {
    myInt = ROOT_NOTE_A;
  }
  else if (!strncmp(compare.c_str(), "B", strlen("B")))
  {
    myInt = ROOT_NOTE_B;
  }
  else
  {
    Cmdr.println("ERROR: only accepted values are \"C\", \"C#\", \"D\", \"D#\", \"E\", \"F\", \"G\", \"G#\", \"A\", \"A#\", \"B\"!");
    return false;
  }
  Cmdr.print("SUCCESS: Setting root note to ");
  Cmdr.println(myInt);
  current_config.root_note = myInt;
  saveConfigToEEPROM(current_config, config_state);
  return true;
}

bool octaveHandler(Commander &Cmdr)
{
  if (Cmdr.getInt(myInt))
  {

    if ((myInt > 60) ||
    ((myInt % 12 > 0) && (myInt > 0)))
    {
      Cmdr.println("ERROR: only accepted values are \"0\", \"12\", \"24\", \"36\", \"48\", \"60\"!");
      return false;  
    }
    
    Cmdr.print("SUCCESS: Setting octave to ");
    Cmdr.println(myInt);
    current_config.octave = myInt;
    saveConfigToEEPROM(current_config, config_state);
  }
  return true;
}

bool modifierHandler(Commander &Cmdr)
{
  String compare = NULL;
  
  if (!Cmdr.hasPayload())
  {
    Cmdr.println("ERROR: This command needs a payload. Example: `modifier=MINOR`!");
    return false;
  }

  compare = Cmdr.getPayloadString();
  if ("" == compare.c_str())
  {
    Cmdr.println("ERROR: only accepted values are \"MAJOR\", \"MINOR\", \"PENT_MAJOR\", \"PENT_MINOR\"!");
    return false;  
  }

  if (!strncmp(compare.c_str(), modifier_str_array[MOD_MAJOR], strlen(modifier_str_array[MOD_MAJOR])))
  {
    current_config.modifier = MOD_MAJOR;
  }
  else if (!strncmp(compare.c_str(), modifier_str_array[MOD_MINOR], strlen(modifier_str_array[MOD_MINOR])))
  {
    current_config.modifier = MOD_MINOR;
  }
  else if (!strncmp(compare.c_str(), modifier_str_array[MOD_MIXOLYDIAN], strlen(modifier_str_array[MOD_MIXOLYDIAN])))
  {
    current_config.modifier = MOD_MIXOLYDIAN;
  }
  else if (!strncmp(compare.c_str(), modifier_str_array[MOD_DORIAN], strlen(modifier_str_array[MOD_DORIAN])))
  {
    current_config.modifier = MOD_DORIAN;
  }
  else if (!strncmp(compare.c_str(), modifier_str_array[MOD_CHROMATIC], strlen(modifier_str_array[MOD_CHROMATIC])))
  {
    current_config.modifier = MOD_CHROMATIC;
  }
  else
  {
    Cmdr.println("ERROR: only accepted values are \"MAJOR\", \"MINOR\", \"MIXOLYDIAN\", \"DORIAN\", \"CHROMATIC\"!");
    return false;
  }
  Cmdr.print("SUCCESS: Setting modifier to ");
  Cmdr.println(modifier_str_array[current_config.modifier]);
  saveConfigToEEPROM(current_config, config_state);
  return true;
}

bool paddlePingHandler(Commander &Cmdr)
{
  Cmdr.println("paddlePong");
  return true;
}

bool button1Handler(Commander &Cmdr)
{
  if (Cmdr.getInt(myInt))
  {
    Cmdr.print("SUCCESS: Setting button 1 offset to ");
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
    Cmdr.print("SUCCESS: Setting button 2 offset to ");
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
    Cmdr.print("SUCCESS: Setting button 3 offset to ");
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
    Cmdr.print("SUCCESS: Setting MIDI control channel to ");
    Cmdr.println(myInt);
    current_config.control_channel = myInt;
    saveConfigToEEPROM(current_config, config_state);
  }
  return true;
}

bool pbChanHandler(Commander &Cmdr)
{
  if (Cmdr.getInt(myInt))
  {
    Cmdr.print("SUCCESS: Setting Pitchbend control channel to ");
    Cmdr.println(myInt);
    current_config.pitchbend_channel = myInt;
    saveConfigToEEPROM(current_config, config_state);
  }
  return true;
}

bool exitHandler(Commander &Cmdr)
{
  Cmdr.println("Saving settings!");
  Cmdr.println("Going down for reboot now!");
  delay(1000);
  WriteConfigMode(false);
  softRestart();
  
  /* Will likely never get here, but harmless anyways */
  return true;
}

bool defaultsHandler(Commander &Cmdr)
{
  config_t default_config;

  default_config.is_enabled = true;
  default_config.root_note = 0;
  default_config.octave = 24;
  default_config.pitchbend_channel = MIDI_CTRL_CHG_PITCHBEND;
  default_config.modifier = MOD_MAJOR;
  default_config.button1_offset = 3;
  default_config.button2_offset = 5;
  default_config.button3_offset = 7;
  default_config.control_channel = 16;

  for (int i=0; i < ROT_ENC_ENUM_SIZE; i++)
  {
    saveConfigToEEPROM(default_config, i);
  }
  Cmdr.println("SUCCESS: Reset ALL configs to default values");
  return true;
}

bool memDumpHandler(Commander &Cmdr)
{
  memdumpEEPROM();
  return true;  
}


void printConfig(Commander &Cmdr, rot_enc_state state, bool is_last_config)
{
  config_t print_config = loadConfigFromEEPROM(state);
  
  Cmdr.print("{\"enable\": ");
  Cmdr.print((print_config.is_enabled) ? "\"True\" " : "\"False\"");
  Cmdr.print(", \"offset1\": ");
  Cmdr.print(print_config.button1_offset);
  Cmdr.print(", \"offset2\": ");
  Cmdr.print(print_config.button2_offset);
  Cmdr.print(", \"offset3\": ");
  Cmdr.print(print_config.button3_offset);

  Cmdr.print(",\"octave\": ");
  Cmdr.print(print_config.octave);
  Cmdr.print(", \"root_note\": ");
  Cmdr.print(print_config.root_note);
  Cmdr.print(", \"mode\": \"");
  Cmdr.print(modifier_str_array[print_config.modifier]);
  
  Cmdr.print("\", \"control\": ");
  Cmdr.print(print_config.control_channel);  
  Cmdr.print(", \"pitchbend\": ");
  Cmdr.print(print_config.pitchbend_channel);
  Cmdr.print((is_last_config) ? "}" : "},");
}
