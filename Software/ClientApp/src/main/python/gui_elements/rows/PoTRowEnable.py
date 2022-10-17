from PyQt5.QtCore import Qt

from gui_elements.common.PoTCommonTypes import PoTRow, PoTToggleButton
from gui_elements.common.PoTStyleSheets import PoTStyleQPushbuttonGreen, PoTStyleQPushbuttonRed


class PoTEnableButton(PoTToggleButton):
    """Provides a toggle button to enable or disable the entire config."""

    def __init__(self, parent=None, color=None, config_name=None, text=None):
        super().__init__(parent=parent, text=text)
        self.parent = parent
        self.config_name = config_name
        self.color = color
        self.layout.setContentsMargins(0, 5, 80, 5)
        self.layout.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

    def reload(self):
        self.state = self.parent.protocol[self.color][self.config_name]
        if "TRUE" == self.state:
            self.button.setText(self.onText)
            self.setStyleSheet(PoTStyleQPushbuttonGreen)
            self.parent.current_tab.enableAllWidgets()
        else:
            self.button.setText(self.offText)
            self.setStyleSheet(PoTStyleQPushbuttonRed)
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
