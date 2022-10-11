from gui_elements.common.CommonTypes import PoTRow, PoTToggleButton


class PoTEnableButton(PoTToggleButton):
    """Provides a toggle button to enable or disable the entire config."""

    def __init__(self, parent=None, color=None, config_name=None, text=None):
        super().__init__(parent=parent, text=text)
        self.parent = parent
        self.config_name = config_name
        self.color = color

    def reload(self):
        self.state = self.parent.protocol[self.color][self.config_name]
        if "TRUE" == self.state:
            # TODO self.button.setStyleSheet(TODO)
            self.button.setText(self.onText)
            self.parent.current_tab.enableAllWidgets()
        else:
            # TODO self.button.setStyleSheet(TODO)
            self.button.setText(self.offText)
            self.parent.current_tab.disableAllWidgets()

    def onClick(self):
        self.state = "FALSE" if self.state == "TRUE" else "TRUE"
        self.updateValue(self.state)
        if "TRUE" == self.state:
            self.parent.current_tab.enableAllWidgets()
        else:
            self.parent.current_tab.disableAllWidgets()

        if self.parent is not None:
            self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
            self.parent.si.sendSerialCommand(cmd=self.config_name,
                                             argument=self.state)


class PoTRowEnable(PoTRow):
    """This row simply contains the button to enable/disable a given color config"""

    def __init__(self, parent=None, text=None, color=None):
        self.enableButton = PoTEnableButton(
            parent=parent,
            color=color,
            config_name="enable"
        )

        super().__init__(
            parent=parent,
            text=text,
            widgets=[self.enableButton]
        )
