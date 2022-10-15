from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QFrame, QPushButton, QGridLayout, QGroupBox, QLabel, QComboBox, QDoubleSpinBox


class PoTRow(QGroupBox):
    """A parameter cluster which may contain multiple rows"""
    def __init__(self, parent=None, text="", widgets=[]):
        super().__init__()
        self.parent = parent
        self.text = text
        self.layout = QGridLayout()
        self.layout.setSpacing(10)
        self.layout.setContentsMargins(5, 1, 5, 1)
        self.setStyleSheet('''
            QGroupBox {
                    background-color: qlineargradient(x1: 0, y1: 0, x2:0, y2: 1, stop: 0 #FFFFFF stop: 1 #E0E0E0);
                    border: 1px solid grey;
                    border-radius: 7px;
            }''')

        self.setContextMenuPolicy(Qt.PreventContextMenu)
        # TODO self.setStyleSheet(TODO)

        self.label_group = QLabel()
        self.label_group.setText(self.text)
        self.label_group.setStyleSheet('QLabel {font: bold 13pt "Helvetica";}')
        # TODO self.label_group.setStyleSheet(TODO)

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


class PoTToggleButton(QFrame):
    """A small region containing a button which toggles between two states"""
    def __init__(self, parent=None, offText="Disabled", onText="Enabled", text=None):

        super().__init__()
        self.parent = parent
        self.text = text
        self.offText = offText
        self.onText = onText

        self.button = QPushButton(self.text)
        self.button.clicked.connect(self.onClick)
        self.button.clicked.connect(lambda: self.updateValue(self.state))

        self.layout = QGridLayout()
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(0, 5, 80, 5)
        self.layout.addWidget(self.button, 0, 0)
        self.setLayout(self.layout)

        self.state = "TRUE"
        self.buttonText = onText
        self.button.setText(self.buttonText)

        self.setStyleSheet('''QPushButton {
            font: 11pt Helvetica; 
            border: 1px solid grey; 
            border-radius: 4px;
            background-color: rgba(94, 230, 73, 100%);
            font: bold 11pt "Helvetica"; 
        }''')
        self.button.setFixedWidth(130)

    def updateValue(self, value):
        """Update the button state when the serial parameter variable changes"""
        self.state = value
        if self.state == "TRUE":
            self.setStyleSheet('''QPushButton {
                font: bold 11pt "Helvetica"; 
                border: 1px solid grey; 
                border-radius: 4px;
                background-color: rgba(94, 230, 73, 100%);
            }''')
            self.buttonText = self.onText
        else:
            self.setStyleSheet('''QPushButton {
                font: bold 11pt "Helvetica"; 
                border: 1px solid grey; 
                border-radius: 4px;
                background-color: rgba(230, 76, 73, 100%); 
                color: white;
            }''')
            self.buttonText = self.offText
        self.button.setText(self.buttonText)

    def onClick(self):
        pass

    def reload(self):
        self.updateValue(self.state)


class PoTSerialEntry(QFrame):
    def __init__(self, parent=None, text=None, color=None, config_name=None):
        super().__init__()
        self.setStyleSheet("QDoubleSpinBox {font: 11pt Helvetica;}")
        self.parent = parent
        self.text = text
        self.parameter = -1
        self.color = color
        self.config_name = config_name

        self.setFixedWidth(220)
        self.setFixedHeight(50)

        self.isValueUpdated = False

        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(0, 5, 80, 5)
        self.layout.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.spinbox_set = QDoubleSpinBox()
        self.spinbox_set.setButtonSymbols(2)
        self.spinbox_set.setDecimals(0)
        self.spinbox_set.setMinimum(0)
        self.spinbox_set.setMaximum(255)
        self.spinbox_set.setContextMenuPolicy(Qt.PreventContextMenu)
        self.spinbox_set.editingFinished.connect(self.editFinishedCallback)
        self.spinbox_set.valueChanged.connect(self.valueChangedCallback)
        # TODO self.spinbox_set.setStyleSheet(TODO)

        self.layout.addWidget(self.spinbox_set, 0, 1, 1, 4)
        self.spinbox_set.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.spinbox_set.setFixedWidth(130)

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


class PoTPushbutton(QFrame):
    """A small region containing a push button"""
    def __init__(self, parent=None, text=None):
        super().__init__()
        self.parent = parent
        self.text = text

        self.button = QPushButton(self.text)
        self.layout = QGridLayout()
        self.setLayout(self.layout)
        # self.layout.setSpacing(5)
        # self.layout.setContentsMargins(0, 5, 80, 5)
        self.layout.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.layout.addWidget(self.button, 0, 0)
        self.button.clicked.connect(self.onClick)

        self.label = QLabel()
        self.label.setText(self.text)

        self.setStyleSheet("QPushButton {font: 11pt Helvetica; border-radius: 4px}")
        self.button.setFixedWidth(130)

    def onClick(self):
        pass


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

        self.setStyleSheet('QComboBox {font: 11pt Helvetica;} QLabel {font: 9pt Helvetica;}')
        self.combo_box.setFixedWidth(130)

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
