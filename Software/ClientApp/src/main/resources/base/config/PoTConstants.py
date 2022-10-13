"""
Constant values for the ClientApp
=============================================================

Lookup file for constant values including serial commands and default state variables
"""

""" Commands to send to the Paddle of Theseus in configuration mode """
CMD_ABOUT = b"about\r"
CMD_ALL_CONFIG = b"all_config\r"
CMD_RESTORE_DEFAULTS = b"defaults\r"
CMD_EXIT = b"exit\r"

"""JSON blob keys returned from `all_config`"""
STR_ALL_CONFIGS = "all_configs"
STR_CURR_CONFIG = "current_config"

""" MIDI message values """
MIDI_CC_P_BEND = 0xE0

"""Enumerate supported musical modes."""
modeDict = {
    "MAJOR": 0,
    "MINOR": 1,
    "MIXOLYDIAN": 2,
    "DORIAN": 3,
    "CHROMATIC": 4
}

"""Enumerate root notes as MIDI."""
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
    "B": 11
}

""" Same information as rootNoteDict above but in zero-indexed list format."""
rootNoteArray = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]

"""Per-octave offsets to be added to root notes as MIDI."""
octaveDict = {
    "-1": 0,
    "0": 12,
    "1": 24,
    "2": 36,
    "3": 48,
    "4": 60
}

"""Enumerate of internal mapping to pitchBendEnable."""
pitchbendDict = {
    "Pitch Bend": 0,
    "Control Code": 1
}

"""Hold default values for empty colorConfig dictionary."""
base_config_dict = dict(enable="TRUE", offset1=1, offset2=2, offset3=3, octave=0, root_note="C", mode="MAJOR",
                        control=5, pitchbend=6)
