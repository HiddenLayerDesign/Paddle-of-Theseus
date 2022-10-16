from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QComboBox, QFrame, QGridLayout, QLabel

from gui_elements.common.CommonTypes import PoTToggleButton, PoTRow, PoTSerialEntry
from src.main.resources.base.config.PoTConstants import MIDI_CC_P_BEND, pitchbendDict


class PitchbendEnableButton(PoTToggleButton):
    """Enables or disables the entire pitchBend functionality."""

    def __init__(self, parent=None, config_name=None, color=None, text=None):
        super().__init__(parent=parent, text=text)
        self.parent = parent
        self.color = color
        self.config_name = config_name
        self.parameter = self.parent.protocol[self.color][self.config_name]
        self.state = "FALSE" if self.parameter == 255 else "TRUE"
        self.text = text
        self.layout.setContentsMargins(0, 5, 80, 5)

    def reload(self):
        self.state = self.parent.protocol[self.color][self.config_name]
        self.state = "FALSE" if self.parameter == 255 else "TRUE"

        if "TRUE" == self.state:
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.onText
            self.parent.current_tab.enablePitchbendWidget()
        else:
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.offText
            self.parent.current_tab.disablePitchbendWidget()

    def onClick(self):
        self.state = "FALSE" if self.state == "TRUE" else "TRUE"
        self.updateValue(self.state)
        if self.parent is not None:
            if "TRUE" == self.state:
                self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
                self.parent.si.sendSerialCommand(cmd="pitchbend", argument=self.parameter)
                self.parent.current_tab.enablePitchbendWidget()
            else:
                self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
                self.parent.si.sendSerialCommand(cmd="pitchbend", argument=255)
                self.parent.current_tab.disablePitchbendWidget()


class PitchbendComboBox(QFrame):
    """Selects whether pitchbend will be used as a true pitchbend or to control a MIDI CC."""

    def __init__(self, parent=None, text=None, config_name=None, color=None, keys=[], values=[],
                 entry=None):
        super().__init__()
        self.parent = parent
        self.text = text
        self.color = color
        self.config_name = config_name
        self.setStyleSheet("QComboBox {font: 9pt Helvetica;}")

        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)

        self.label = QLabel()
        self.label.setText(self.text)

        self.layout.addWidget(self.label, 0, 0, 1, 1)
        self.layout.setContentsMargins(0, 5, 80, 5)
        self.layout.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.values = list(values)
        self.keys = list(keys)
        self.entry = entry

        self.combo_box = QComboBox()
        self.combo_box.activated.connect(self.onChanged)

        self.parameter = self.parent.protocol[self.color][self.config_name]
        if self.parameter == MIDI_CC_P_BEND:
            self.combo_box.setCurrentIndex(pitchbendDict["Pitch Bend"])
        else:
            self.combo_box.setCurrentIndex(pitchbendDict["Control Code"])

        for key in keys:
            self.combo_box.addItem(key)

        self.layout.addWidget(self.combo_box, 0, 0)
        self.combo_box.setFixedWidth(150)
        self.combo_box.setFixedHeight(35)

    def setEntry(self, entry):
        self.entry = entry

    def reload(self):
        self.parameter = self.parent.protocol[self.color][self.config_name]
        if self.parameter == MIDI_CC_P_BEND:
            self.combo_box.setCurrentIndex(pitchbendDict["Pitch Bend"])
            self.parameter = MIDI_CC_P_BEND
            self.entry.parameter = MIDI_CC_P_BEND
            self.entry.updateValue()
            self.parent.current_tab.disablePitchbendCC()
        else:
            self.combo_box.setCurrentIndex(pitchbendDict["Control Code"])
            if self.parameter < 128:
                self.parent.current_tab.enablePitchbendCC()

    def onChanged(self, text):
        self.combo_box.adjustSize()
        if self.combo_box.currentIndex() == 0:
            self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
            self.parameter = MIDI_CC_P_BEND
            self.entry.parameter = MIDI_CC_P_BEND
            self.entry.updateValue()
            self.parent.si.sendSerialCommand(cmd="pitchbend", argument=self.parameter)
            self.parent.current_tab.disablePitchbendCC()
        else:
            self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
            self.parent.si.sendSerialCommand(cmd="pitchbend", argument=self.parameter)
            self.parent.current_tab.enablePitchbendCC()

    def updateValue(self):
        self.combo_box.setCurrentIndex(self.parameter)


class PoTRowPitchBend(PoTRow):
    """Create all the input fields that control the pitchbend options."""

    def __init__(self, parent=None, text=None, color=None):
        self.pbEnableButton = PitchbendEnableButton(
            parent=parent,
            text="Enable:",
            config_name="pitchbend",
            color=color
        )
        self.pbComboBox = PitchbendComboBox(
            parent=parent,
            text="Type:",
            config_name="pitchbend",
            keys=pitchbendDict.keys(),
            values=pitchbendDict.values(),
            color=color,
        )
        self.pbEntry = PoTSerialEntry(
            parent=parent,
            text="CC_Idx:",
            color=color,
            config_name="pitchbend"
        )

        super().__init__(
            parent=parent,
            text=text,
            widgets=[self.pbEnableButton, self.pbComboBox, self.pbEntry]
        )
        self.pbComboBox.setEntry(self.pbEntry)
