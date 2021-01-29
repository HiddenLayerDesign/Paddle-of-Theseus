import math
from enum import Enum

from pyqtsa.GUIParameter import GUIParameter
from pyqtsa.Protocol import Protocol

""" Enumeration of musical modes """
modeDict = {
    "MAJOR": 0,
    "MINOR": 1,
    "MIXOLYDIAN": 2,
    "DORIAN": 3
}

rootNoteDict = {
    "A": 33,
    "A#": 34,
    "B": 35,
    "C": 36,
    "C#": 25,
    "D": 26,
    "D#": 27,
    "E": 28,
    "F": 29,
    "F#": 30,
    "G": 31,
    "G#": 32
}


class PoTProtocol(Protocol):
    """
    Holds a list of individual parameters
    """

    def __init__(self, master=None, color=""):
        self.master = master
        self.color = color

        """ instantiate all top-level parameters to be configured """
        self.parameters = {
            "enable": GUIParameter(),
            "root_note": GUIParameter(),
            "mode": GUIParameter(),
            "offset1": GUIParameter(),
            "offset2": GUIParameter(),
            "offset3": GUIParameter(),
            "control": GUIParameter(),
        }

        # Setup enable variable
        self.parameters["enable"].name = "enable"
        self.parameters["enable"].permission = "RW"
        self.parameters["enable"].param_type = "BOOL"
        self.parameters["enable"].description = "Is this color config enabled or disabled?"

        self.parameters["root_note"].name = "root_note"
        self.parameters["root_note"].permission = "RW"
        self.parameters["root_note"].param_type = "U8"
        self.parameters["root_note"].description = "Which note is the root note"

        self.parameters["mode"].name = "mode"
        self.parameters["mode"].permission = "RW"
        self.parameters["mode"].param_type = "U8"
        self.parameters["mode"].description = "Which mode will be used"

        # Setup MIDI CC control index
        self.parameters["control"].name = "control"
        self.parameters["control"].permission = "RW"
        self.parameters["control"].param_type = "U8"
        self.parameters["control"].description = "MIDI CC index to be changed by Rotary Encoder"
        self.parameters["control"].scale = 1
        self.parameters["control"].precision = 0
        self.parameters["control"].value_min = 0
        self.parameters["control"].value_max = 127

        # Setup scale-offset between capTouch buttons 0 & 1
        self.parameters["offset1"].name = "offset1"
        self.parameters["offset1"].permission = "RW"
        self.parameters["offset1"].param_type = "U8"
        self.parameters["offset1"].description = "Scale offset in notes between capTouch buttons 0 & 1"
        self.parameters["offset1"].scale = 1
        self.parameters["offset1"].precision = 0
        self.parameters["offset1"].value_min = 0
        self.parameters["offset1"].value_max = 60

        # Setup scale-offset between capTouch buttons 0 & 2
        self.parameters["offset2"].name = "offset2"
        self.parameters["offset2"].permission = "RW"
        self.parameters["offset2"].param_type = "U8"
        self.parameters["offset2"].description = "Scale offset in notes between capTouch buttons 0 & 2"
        self.parameters["offset2"].scale = 1
        self.parameters["offset2"].precision = 0
        self.parameters["offset2"].value_min = 0
        self.parameters["offset2"].value_max = 60

        # Setup scale-offset between capTouch buttons 0 & 3
        self.parameters["offset3"].name = "offset3"
        self.parameters["offset3"].permission = "RW"
        self.parameters["offset3"].param_type = "U8"
        self.parameters["offset3"].description = "Scale offset in notes between capTouch buttons 0 & 3"
        self.parameters["offset3"].scale = 1
        self.parameters["offset3"].precision = 0
        self.parameters["offset3"].value_min = 0
        self.parameters["offset3"].value_max = 60

    def set_parameters(self, enable, root_note, mode, offset1, offset2, offset3, control):
        """ use function args to set params """
        self.parameters["offset1"].variable.value = offset1
        self.parameters["offset2"].variable.value = offset2
        self.parameters["mode"].variable.value = mode
        self.parameters["enable"].variable.value = enable
        self.parameters["root_note"].variable.value = root_note
        self.parameters["offset3"].variable.value = offset3
        self.parameters["control"].variable.value = control
