from pyqtsa.GUIParameter import GUIParameter
from pyqtsa.Protocol import Protocol


class PoTProtocol(Protocol):
    """
    Holds the parameters that are stored persistently in EEPROM in the Paddle,
    and which are edited by the PoTConfig tool.

    NOTE: The source of truth for these settings will always be the Paddle's EEPROM,
    so this needs to stay in sync, at least at the start of every connection and after changing config.

    ALSO: Parameters "pitchbend_enable" and "pitchbend_is_CC" simplify some of the GUI logic, but are just derived from
    the "pitchbend" argument, and only "pitchbend" is stored on the paddle.
    """

    def __init__(self, master=None, color=""):
        self.master = master
        self.color = color

        """ instantiate all top-level parameters to be configured """
        self.parameters = {
            "enable": GUIParameter(),
            "root_note": GUIParameter(),
            "octave": GUIParameter(),
            "mode": GUIParameter(),
            "offset1": GUIParameter(),
            "offset2": GUIParameter(),
            "offset3": GUIParameter(),
            "control": GUIParameter(),
            "pitchbend_enable": GUIParameter(),
            "pitchbend_is_CC": GUIParameter(),
            "pitchbend": GUIParameter()
        }

        # Setup ColorConfig Enable variable
        self.parameters["enable"].name = "enable"
        self.parameters["enable"].permission = "RW"
        self.parameters["enable"].param_type = "BOOL"
        self.parameters["enable"].description = "Is this color config enabled or disabled"

        # Setup MIDI Root Note parameter
        self.parameters["root_note"].name = "root_note"
        self.parameters["root_note"].permission = "RW"
        self.parameters["root_note"].param_type = "U8"
        self.parameters["root_note"].description = "Which note is the root note"

        # Setup MIDI Octave parameter
        self.parameters["octave"].name = "octave"
        self.parameters["octave"].permission = "RW"
        self.parameters["octave"].param_type = "U8"
        self.parameters["octave"].description = "Which octave is the root for the instrument"

        # Setup pitchbend_enable parameter  NOTE: this is not sent to the paddle
        self.parameters["pitchbend_enable"].name = "pitchbend Enabled"
        self.parameters["pitchbend_enable"].permission = "RW"
        self.parameters["pitchbend_enable"].param_type = "BOOL"
        self.parameters["pitchbend_enable"].description = "Is pitch-bend enabled?"

        # Setup pitchbend_is_CC parameter  NOTE: this is not sent to the paddle
        self.parameters["pitchbend_is_CC"].name = "pitchbend is Control Code"
        self.parameters["pitchbend_is_CC"].permission = "RW"
        self.parameters["pitchbend_is_CC"].param_type = "BOOL"
        self.parameters["pitchbend_is_CC"].description = "Does pitchbend control pitch or ControlCode"

        # Setup MIDI pitch-bend type parameter
        self.parameters["pitchbend"].name = "pitchbend"
        self.parameters["pitchbend"].permission = "RW"
        self.parameters["pitchbend"].param_type = "U8"
        self.parameters["pitchbend"].description = "What control code does the pitch-bend affect"

        # Setup MIDI Scale mode parameter
        self.parameters["mode"].name = "mode"
        self.parameters["mode"].permission = "RW"
        self.parameters["mode"].param_type = "U8"
        self.parameters["mode"].description = "Which mode will be used"

        # Setup MIDI CC control index parameter
        self.parameters["control"].name = "control"
        self.parameters["control"].permission = "RW"
        self.parameters["control"].param_type = "U8"
        self.parameters["control"].description = "MIDI CC index to be changed by Rotary Encoder"
        self.parameters["control"].scale = 1
        self.parameters["control"].precision = 0
        self.parameters["control"].value_min = 0
        self.parameters["control"].value_max = 127

        # Setup scale-offset between capTouch buttons 0 & 1 parameter
        self.parameters["offset1"].name = "offset1"
        self.parameters["offset1"].permission = "RW"
        self.parameters["offset1"].param_type = "U8"
        self.parameters["offset1"].description = "Scale offset in notes between capTouch buttons 0 & 1"
        self.parameters["offset1"].scale = 1
        self.parameters["offset1"].precision = 0
        self.parameters["offset1"].value_min = 0
        self.parameters["offset1"].value_max = 60

        # Setup scale-offset between capTouch buttons 0 & 2 parameter
        self.parameters["offset2"].name = "offset2"
        self.parameters["offset2"].permission = "RW"
        self.parameters["offset2"].param_type = "U8"
        self.parameters["offset2"].description = "Scale offset in notes between capTouch buttons 0 & 2"
        self.parameters["offset2"].scale = 1
        self.parameters["offset2"].precision = 0
        self.parameters["offset2"].value_min = 0
        self.parameters["offset2"].value_max = 60

        # Setup scale-offset between capTouch buttons 0 & 3 parameter
        self.parameters["offset3"].name = "offset3"
        self.parameters["offset3"].permission = "RW"
        self.parameters["offset3"].param_type = "U8"
        self.parameters["offset3"].description = "Scale offset in notes between capTouch buttons 0 & 3"
        self.parameters["offset3"].scale = 1
        self.parameters["offset3"].precision = 0
        self.parameters["offset3"].value_min = 0
        self.parameters["offset3"].value_max = 60

    def set_parameters(self, enable, root_note, mode, offset1, offset2, offset3, control, octave,
                       pb_enable, pb_is_cc, pb_value):
        """ Set class parameters from provided arguments """
        self.parameters["offset1"].variable.value = offset1
        self.parameters["offset2"].variable.value = offset2
        self.parameters["mode"].variable.value = mode
        self.parameters["enable"].variable.value = enable
        self.parameters["octave"].variable.value = octave
        self.parameters["root_note"].variable.value = root_note
        self.parameters["offset3"].variable.value = offset3
        self.parameters["control"].variable.value = control
        self.parameters["pitchbend_enable"].variable.value = pb_enable
        self.parameters["pitchbend_is_CC"].variable.value = pb_is_cc
        self.parameters["pitchbend"].variable.value = pb_value
