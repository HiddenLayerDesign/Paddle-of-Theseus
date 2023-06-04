from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QFrame, QPushButton, QGridLayout, QGroupBox, QLabel, QComboBox, QWidget, \
    QSizePolicy, QSpinBox

from gui_elements.common.PoTStyleSheets import PoTStyleQSpinBox, PoTStyleQComboBox, PoTStyleQPushbuttonRed, \
    PoTStyleQPushbuttonGreen, PoTStyleQLabelLarge, PoTStyleRowQGroupBox


class PoTRow(QGroupBox):
    """A parameter cluster which may contain multiple rows"""
    def __init__(self, parent=None, text="", widgets=[]):
        super().__init__()
        self.parent = parent
        self.text = text
        self.layout = QGridLayout()
        self.layout.setSpacing(10)
        self.layout.setContentsMargins(5, 1, 5, 1)
        self.setStyleSheet(PoTStyleRowQGroupBox)

        self.setContextMenuPolicy(Qt.PreventContextMenu)

        self.label_group = QLabel()
        self.label_group.setText(self.text)
        self.label_group.setStyleSheet(PoTStyleQLabelLarge)
        self.label_group.setFixedWidth(200)

        self.layout.addWidget(self.label_group, 0, 0, 1, 1)
        self.label_group.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)
        self.setLayout(self.layout)
        self.setFixedHeight(60)

        if not len(widgets) > 0:
            widgets = [None]

        self.widgets = widgets

        col = 2
        if len(widgets) > 0:
            for widget in widgets:
                self.layout.addWidget(widget, 0, col)
                col += 1


class PoTPushbutton(QFrame):
    """A small region containing a push button"""
    def __init__(self, parent=None, text=None):
        super().__init__()
        self.parent = parent
        self.text = text

        self.button = QPushButton(self.text)
        self.button.setFlat(False)
        self.layout = QGridLayout()
        self.setLayout(self.layout)

        self.layout.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.layout.addWidget(self.button, 0, 0)
        self.button.clicked.connect(self.onClick)

        self.label = QLabel()
        self.label.setText(self.text)

        self.layout.setContentsMargins(0, 5, 80, 5)
        self.button.setFixedWidth(150)
        self.button.setFixedHeight(35)

    def onClick(self):
        pass


class PoTToggleButton(QFrame):
    """A small region containing a button which toggles between two states"""
    def __init__(self, parent=None, offText="Disabled", onText="Enabled", text=None):

        super().__init__()
        self.parent = parent
        self.text = text
        self.offText = offText
        self.onText = onText

        self.button = QPushButton(self.text)
        self.button.setFlat(False)
        self.button.clicked.connect(self.onClick)
        self.button.clicked.connect(lambda: self.updateValue(self.state))

        self.layout = QGridLayout()
        self.layout.addWidget(self.button, 0, 0)
        self.setLayout(self.layout)

        self.state = "TRUE"
        self.buttonText = onText
        self.button.setText(self.buttonText)

        self.setStyleSheet(PoTStyleQPushbuttonGreen)
        self.button.setFixedWidth(150)
        self.button.setFixedHeight(35)

    def updateValue(self, value):
        """Update the button state when the serial parameter variable changes"""
        self.state = value
        if self.state == "TRUE":
            self.setStyleSheet(PoTStyleQPushbuttonGreen)
            self.buttonText = self.onText
        else:
            self.setStyleSheet(PoTStyleQPushbuttonRed)
            self.buttonText = self.offText
        self.button.setText(self.buttonText)

    def onClick(self):
        pass

    def reload(self):
        self.updateValue(self.state)


class PoTSerialEntry(QFrame):
    def __init__(self, parent=None, text=None, color=None, config_name=None):
        super().__init__()
        self.setStyleSheet(PoTStyleQSpinBox)
        self.parent = parent
        self.text = text
        self.parameter = -1
        self.color = color
        self.config_name = config_name

        self.isValueUpdated = False

        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(0, 5, 80, 5)
        self.layout.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.spinbox_set = QSpinBox()
        self.spinbox_set.setButtonSymbols(2)
        self.spinbox_set.setMinimum(0)
        self.spinbox_set.setMaximum(255)
        self.spinbox_set.setContextMenuPolicy(Qt.PreventContextMenu)
        self.spinbox_set.editingFinished.connect(self.editFinishedCallback)
        self.spinbox_set.valueChanged.connect(self.valueChangedCallback)

        self.layout.addWidget(self.spinbox_set, 0, 1, 1, 4)
        self.spinbox_set.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.spinbox_set.setFixedWidth(150)
        self.spinbox_set.setFixedHeight(35)

    def valueChangedCallback(self):
        self.isValueUpdated = True

    def editFinishedCallback(self):
        if self.isValueUpdated:
            self.isValueUpdated = False
            self.onEditingFinished()

    def updateValue(self):
        self.spinbox_set.setValue(self.parameter)

    def onEditingFinished(self):
        self.parameter = int(self.spinbox_set.value())
        self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
        self.parent.si.sendSerialCommand(cmd=self.config_name, argument=self.parameter)

    def reload(self):
        self.parameter = self.parent.protocol[self.color][self.config_name]
        self.updateValue()


class PoTComboBox(QFrame):
    def __init__(self, parent=None, text=None, color=None, keys=[], values=[], config_name=None):
        super().__init__()
        self.parent = parent
        self.text = text
        self.color = color
        self.config_name = config_name

        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)

        self.label = QLabel()
        self.label.setText(self.text)
        self.label.setFixedWidth(50)
        self.label.setFixedHeight(35)

        self.layout.addWidget(self.label, 0, 0)
        self.layout.setContentsMargins(0, 5, 80, 5)
        self.layout.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.parameter = self.parent.protocol[self.color][self.config_name]
        self.values = list(values)
        self.keys = list(keys)

        self.combo_box = QComboBox()

        # TODO below the [str] is necessary for the combo box to return the title of the combo-box option rather
        #      than just the index. For the future it may be better to work off the index to simplify the mapping
        #      Counterpoint, one upside of this approach is that the dictionaries in PoTConstants document themselves
        self.combo_box.activated[str].connect(self.onChanged)
        for key in keys:
            self.combo_box.addItem(key)

        self.layout.addWidget(self.combo_box, 0, 1, 1, 4)

        self.setStyleSheet(PoTStyleQComboBox)
        self.combo_box.setFixedWidth(90)
        self.combo_box.setFixedHeight(35)

    def onChanged(self, text):
        text = str(text)
        self.combo_box.adjustSize()
        self.parent.si.sendSerialCommand(cmd="color", argument=self.color)
        my_argument = self.values[self.keys.index(text)]
        self.parent.si.sendSerialCommand(cmd=self.config_name, argument=my_argument)
        self.parameter = my_argument
        self.combo_box.setCurrentIndex(self.values.index(self.parameter))

    def updateValue(self):
        self.combo_box.setCurrentIndex(self.values.index(self.parameter))

    def reload(self):
        self.parameter = self.parent.protocol[self.color][self.config_name]
        self.combo_box.setCurrentIndex(self.values.index(self.parameter))


class PoTFiller(QFrame):
    def __init__(self):
        super().__init__()
        self.filler = QWidget()
        self.filler.setSizePolicy(QSizePolicy.Expanding | QSizePolicy.Preferred,
                                  QSizePolicy.Expanding | QSizePolicy.Preferred)
        self.filler.setAttribute(Qt.WA_TranslucentBackground)
        self.filler.setContextMenuPolicy(Qt.PreventContextMenu)

        self.layout = QGridLayout()
        self.setLayout(self.layout)

    def reload(self):
        pass
