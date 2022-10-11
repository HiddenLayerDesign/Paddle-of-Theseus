from gui_elements.common.CommonTypes import PoTRow, PoTComboBox
from src.main.resources.base.config.PoTConstants import octaveDict, rootNoteDict, modeDict


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

        self.mode_combo = PoTComboBox(
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
