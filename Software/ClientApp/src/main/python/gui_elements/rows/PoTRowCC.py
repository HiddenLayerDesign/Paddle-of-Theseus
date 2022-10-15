from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QFrame, QGridLayout, QLabel, QDoubleSpinBox

from gui_elements.common.CommonTypes import PoTRow


class PoTInfoFrame(QFrame):
    """A small region to display informational text"""

    def __init__(self, parent=None, param_name=None, text="", color=None):
        super().__init__()

        self.color = color
        self.parent = parent
        self.text = text

        self.parameter = self.parent.protocol[color][param_name]
        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(0, 5, 80, 5)

        self.label = QLabel()
        self.label.setText(self.text)
        self.label.setAttribute(Qt.WA_TranslucentBackground)
        self.label.setStyleSheet("QLabel {font: bold 12pt Helvetica;}")

        self.label_info = QLabel()
        self.label_info.setAttribute(Qt.WA_TranslucentBackground)
        self.label_info.setStyleSheet("QLabel {font: 12pt Helvetica;}")

        self.layout.addWidget(self.label, 0, 0)
        self.label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.layout.addWidget(self.label_info, 0, 1, 1, 3)
        self.label_info.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

    def updateValue(self, text):
        self.label_info.setText(str(text))

    def reload(self):
        pass


class PoTControlCodeEntry(QFrame):
    """Provide an entry form for entering MIDI control code value for Rotary Encoder to control."""

    def __init__(self, parent=None, text=None, config_name=None, color=None, helpText=None, configDict=None):
        super().__init__()
        self.parent = parent
        self.text = text
        self.color = color
        self.config_name = config_name
        self.helpText = helpText
        self.configDict = configDict
        self.parameter = self.parent.protocol[self.color][self.config_name]

        self.isValueUpdated = False

        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setContentsMargins(0, 5, 80, 5)

        self.spinbox_set = QDoubleSpinBox()
        self.spinbox_set.setButtonSymbols(2)
        self.spinbox_set.setMinimum(0)
        self.spinbox_set.setMaximum(255)
        self.spinbox_set.setContextMenuPolicy(Qt.PreventContextMenu)
        self.spinbox_set.editingFinished.connect(self.editFinishedCallback)
        self.spinbox_set.valueChanged.connect(self.valueChangedCallback)
        self.spinbox_set.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.spinbox_set.setDecimals(0)

        self.layout.addWidget(self.spinbox_set, 0, 1, 1, 4)

        self.spinbox_set.setValue(self.parameter)
        self.helpText.updateValue(self.configDict[str(self.parameter)])

        self.setStyleSheet("QDoubleSpinBox {font: 11pt Helvetica;}")
        self.spinbox_set.setFixedWidth(130)


    def valueChangedCallback(self):
        self.isValueUpdated = True

    def editFinishedCallback(self):
        if self.isValueUpdated:
            self.isValueUpdated = False
            self.onEditingFinished()

    def onEditingFinished(self):
        self.parameter = int(self.spinbox_set.value())
        self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
        self.parent.si.sendSerialCommand(cmd="control", argument=self.parameter)
        self.helpText.updateValue(self.configDict[str(self.parameter)])

    def reload(self):
        self.parameter = self.parent.protocol[self.color][self.config_name]
        self.spinbox_set.setValue(self.parameter)
        self.helpText.updateValue(self.configDict[str(self.parameter)])


class PoTRowCC(PoTRow):
    """Creates the input form and info section to describe the MIDI CC that will be assigned to rotary encoder."""

    def __init__(self, parent=None, text=None, color=None, serial_dict=None):
        self.info_frame = PoTInfoFrame(
            color=color,
            parent=parent,
            param_name="control",
            text="INFO: "
        )
        self.cc_entry = PoTControlCodeEntry(
            parent=parent,
            text="CC_Idx:",
            color=color,
            configDict=serial_dict,
            helpText=self.info_frame,
            config_name="control"
        )

        super().__init__(
            parent=parent,
            text=text,
            widgets=[self.cc_entry, self.info_frame]
        )