
COLOR_BLUE_IDX = 0
COLOR_CYAN_IDX = 1
COLOR_GREEN_IDX = 2
COLOR_PURPLE_IDX = 3
COLOR_RED_IDX = 4
COLOR_YELLOW_IDX = 5
COLOR_WHITE_IDX = 6

base_config_dict = dict(enable="TRUE", offset1=5, offset2=7, offset3=12, octave=24, root_note="C", mode="MAJOR", control=17,
                        pitchbend=14)

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