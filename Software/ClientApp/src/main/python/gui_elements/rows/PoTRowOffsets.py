from gui_elements.common.PoTCommonTypes import PoTRow, PoTSerialEntry


class PoTRowOffsets(PoTRow):
    """Creates the three input fields for offset1, offset2, and offset3."""

    def __init__(self, parent=None, text=None, color=None):
        self.color = color
        self.parent = parent

        self.offset1 = PoTSerialEntry(parent=parent, text="Offset1:", color=color, config_name="offset1")
        self.offset2 = PoTSerialEntry(parent=parent, text="Offset2:", color=color, config_name="offset2")
        self.offset3 = PoTSerialEntry(parent=parent, text="Offset3:", color=color, config_name="offset3")

        super().__init__(
            parent=parent,
            text=text,
            widgets=[self.offset1, self.offset2, self.offset3]
        )
