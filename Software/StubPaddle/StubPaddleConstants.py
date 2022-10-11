COLOR_BLUE_IDX = 0
COLOR_CYAN_IDX = 1
COLOR_GREEN_IDX = 2
COLOR_PURPLE_IDX = 3
COLOR_RED_IDX = 4
COLOR_YELLOW_IDX = 5
COLOR_WHITE_IDX = 6

base_config_dict = dict(enable="TRUE", offset1=5, offset2=7, offset3=12, octave=24, root_note="C",
                        mode="MAJOR", control=17, pitchbend=14)

color_str_array = ["BLUE", "CYAN", "GREEN", "PURPLE", "RED", "YELLOW", "WHITE"]
modeList = ["MAJOR", "MINOR", "MIXOLYDIAN", "DORIAN", "CHROMATIC"]

rootNoteDict = {
    "C": 0,
    "C#": 1,
    "D": 2,
    "D#": 3,
    "E": 4,
    "F": 5,
    "F#": 6,
    "G": 7,
    "G#": 8,
    "A": 9,
    "A#": 10,
    "B": 11,
    "Unknown": 255,
}

""" per-octave Offsets to be added to root notes as MIDI """
octaveDict = {
    "-1": 0,
    "0": 12,
    "1": 24,
    "2": 36,
    "3": 48,
    "4": 60,
    "Unknown": 255,
}

""" Memory serialization constants """
CT1_DELTA_ADDR = 0
CT2_DELTA_ADDR = 1
CT3_DELTA_ADDR = 2
ROOT_NOTE_ADDR = 3
SCALE_MOD_ADDR = 4
MODE_ENABLED_ADDR = 5
CTRL_CHAN_ADDR = 6
OCTAVE_ADDR = 7
PB_CHAN_ADDR = 8

EEPROM_RED_BASE_ADDR = 0x70
EEPROM_GREEN_BASE_ADDR = 0x60
EEPROM_YELLOW_BASE_ADDR = 0x50
EEPROM_BLUE_BASE_ADDR = 0x40
EEPROM_PURPLE_BASE_ADDR = 0x30
EEPROM_CYAN_BASE_ADDR = 0x20
EEPROM_WHITE_BASE_ADDR = 0x10

EEPROM_LIMIT = EEPROM_RED_BASE_ADDR

# Check this bit to know whether to go to config menu or regular operation
EEPROM_CONFIG_MODE_ADDRESS = 0

# Store the Paddle FW version in EEPROM
EEPROM_VERSION_MAJOR_ADDRESS = 1
EEPROM_VERSION_MINOR_ADDRESS = 2
EEPROM_VERSION_BUGFIX_ADDRESS = 3

mem_dump_hint = ["General", "White", "Cyan", "Purple", "Blue", "Yellow", "Green", "Red"]