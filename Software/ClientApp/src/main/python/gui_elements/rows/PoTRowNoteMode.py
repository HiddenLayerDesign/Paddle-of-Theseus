from gui_elements.common.PoTCommonTypes import PoTRow, PoTComboBox
from src.main.resources.base.config.PoTConstants import octaveDict, rootNoteDict, modeDict


class PoTModeComboBox(PoTComboBox):
    def __init__(self, parent=None, text=None, config_name=None, color=None, keys=[], values=[]):
        super().__init__(parent=parent, text=text, config_name=config_name, color=color, keys=keys, values=values)

    def onChanged(self, text):
        text = str(text)
        self.combo_box.adjustSize()
        self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
        my_argument = self.values[self.keys.index(text)]
        self.parent.si.sendSerialCommand(cmd=self.config_name, argument=my_argument.upper())
        self.parameter = my_argument
        self.combo_box.setCurrentIndex(self.values.index(self.parameter))

    def reload(self):
        self.parameter = self.parent.protocol[self.color][self.config_name].title()
        self.combo_box.setCurrentIndex(self.values.index(self.parameter))


class PoTRowNoteMode(PoTRow):
    """Provides a set of combo boxes for selecting the root octave, note, and mode."""

    def __init__(self, parent=None, text=None, color=None):
        self.octave_combo = PoTComboBox(
            parent=parent,
            text="Octave: ",
            keys=octaveDict.keys(),
            values=octaveDict.values(),
            color=color,
            config_name="octave"
        )

        self.root_combo = PoTComboBox(
            parent=parent,
            text="Root: ",
            keys=rootNoteDict.keys(),
            values=rootNoteDict.keys(),
            color=color,
            config_name="root_note"
        )

        self.mode_combo = PoTModeComboBox(
            parent=parent,
            text="Mode: ",
            keys=modeDict.keys(),
            values=modeDict.keys(),
            color=color,
            config_name="mode"
        )

        super().__init__(
            parent=parent,
            text=text,
            widgets=[self.octave_combo, self.root_combo, self.mode_combo]
        )
