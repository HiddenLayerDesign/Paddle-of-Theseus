import json

from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPixmap, QColor, QBitmap

from gui_elements.protocol.PoTConstants import *
from serialInterpreter import CMD_EXIT, CMD_RESTORE_DEFAULTS


class PoTEntry(QFrame):
    """A small region containing an editable field, units label, and button for constants popup"""
    def __init__(self, parent=None,
                 row=0, column=0, index=0,
                 parameter=None,
                 text="",
                 constants=[],
                 value_min=0,
                 value_max=1024,
                 units=""
                 ):
        super().__init__()

        self.parent = parent
        self.row = row
        self.column = column
        self.setFixedWidth(220)
        self.setFixedHeight(50)
        self.text = text

        self.isValueUpdated = False
        self.index = index
        self.value_min = value_min
        self.value_max = value_max
        self.units = units

        self.parameter = parameter
        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(5, 1, 5, 1)

        self.spinbox_set = QDoubleSpinBox()
        self.spinbox_set.setButtonSymbols(2)
        self.spinbox_set.setMinimum(self.value_min)
        self.spinbox_set.setMaximum(self.value_max)
        self.spinbox_set.setContextMenuPolicy(Qt.PreventContextMenu)
        self.spinbox_set.editingFinished.connect(self.editFinishedCallback)
        self.spinbox_set.valueChanged.connect(self.valueChangedCallback)

        self.label_units = QLabel()
        self.label_units.setText(self.units)
        # TODO self.spinbox_set.setStyleSheet(TODO)

        self.layout.addWidget(self.spinbox_set, 0, 1, 1, 4)
        self.spinbox_set.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.spinbox_set.setFixedWidth(90)
        self.layout.addWidget(self.label_units, 0, 5)
        self.label_units.setFixedWidth(40)
        self.label_units.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)

    def updateValue(self, value=None):
        """Update the displayed value when the serial parameter variable changes"""
        self.spinbox_set.setValue(self.parameter)

    def updateParameter(self, value=None):
        """Update the serial parameter variable when the displayed value changes"""
        self.parameter = self.spinbox_set.value()

    def valueChangedCallback(self):
        self.isValueUpdated = True

    def editFinishedCallback(self):
        if self.isValueUpdated:
            self.isValueUpdated = False
            self.onEditingFinished()

    def onEditingFinished(self):
        return


class PoTInfoFrame(QFrame):
    """A small region to display informational text"""
    def __init__(self, parent=None,
                 row=0, column=0, index=0,
                 parameter=None,
                 text="",
                 interval=0,
                 constants=[]):

        super().__init__()

        self.parent = parent
        self.row = row
        self.column = column
        self.setFixedWidth(220)
        self.setFixedHeight(50)
        self.text = text

        self.parameter = parameter
        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(5, 1, 5, 1)

        self.timer = QTimer()
        self.timer.timeout.connect(self.poll)
        self.interval = interval
        self.defaultinterval = interval

        self.label = QLabel()
        self.label.setText(self.text)
        self.label.setFixedWidth(100)
        self.label.setAttribute(Qt.WA_TranslucentBackground)
        self.label_info = QLabel()
        self.label_info.setFixedWidth(150)
        self.label_info.setAttribute(Qt.WA_TranslucentBackground)

        # TODO self.label.setStyleSheet(TODO)
        # TODO self.label_info.setStyleSheet(TODO)

        self.layout.addWidget(self.label, 0, 0)
        self.label.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)

        self.index = index

        self.layout.addWidget(self.label_info, 0, 1, 1, 3)
        self.label_info.setAlignment(Qt.AlignRight | Qt.AlignVCenter)

        self.updateValue()


    @property
    def interval(self):
        return self._interval

    @interval.setter
    def interval(self, interval):
        if interval >= 100:
            self._interval = abs(interval / 1000)
        else:
            self._interval = abs(interval)
        if self._interval > 0:
            self.timer.setInterval(self._interval * 1000)
            self.timer.start()
        else:
            self.timer.setSingleShot(True)

    @interval.getter
    def interval(self):
        return self._interval

    def poll(self):
        """Read the parameter at a regular interval"""
        self.updateValue()

    def updateValue(self, text=None):
        """Update the displayed value when the serial parameter variable changes"""
        if text:
            self.label_info.setText(str(text))
        else:
            self.label_info.setText(str(self.parameter))


class PoTTab(QFrame):
    """A GUI region which contains all parameters relevant to one subsystem"""
    def __init__(self, serial=None, protocol=None, parent=None, title="", icon="", index=0, widgets=[], widthActive=160, widthInactive=80):
        super().__init__()

        self.serial = serial
        self.parent = parent
        self.protocol = protocol

        self.setContextMenuPolicy(Qt.PreventContextMenu)

        self.title = title
        self.label_title = QLabel(title)
        self.label_title.setAttribute(Qt.WA_TranslucentBackground)

        self.index = index
        self.widthActive = widthActive
        self.widthInactive = widthInactive

        # A simple visual representation of the subsystem
        self.iconfile = self.parent.get_resource(icon)
        self.icon = QLabel()
        self.icon.setAttribute(Qt.WA_TranslucentBackground)
        if len(self.iconfile):
            self.iconpix = QPixmap(self.iconfile)
            self.icon.setPixmap(self.iconpix)

        # Build the look of the tab for when it is active
        self.button_active = QGroupBox()
        self.button_active.setFixedHeight(46)
        self.button_active.setFixedWidth(self.widthActive)
        self.layout_buttonactive = QGridLayout()
        self.layout_buttonactive.addWidget(self.label_title, 0, 0)
        self.button_active.setLayout(self.layout_buttonactive)
        # TODO self.button_active.setStyleSheet(TODO)
        self.button_active.setContextMenuPolicy(Qt.CustomContextMenu)
        self.button_active.customContextMenuRequested.connect(self.contextMenuEvent)

        # Build the look of the tab for when it is inactive
        self.button_inactive = QGroupBox()
        self.button_inactive.setFixedHeight(46)
        self.button_inactive.setFixedWidth(self.widthInactive)
        self.layout_buttoninactive = QGridLayout()
        self.button_inactive.setLayout(self.layout_buttoninactive)
        self.layout_buttoninactive.addWidget(self.icon, 0, 0)
        # TODO self.button_inactive.setStyleSheet(TODO)
        self.button_inactive.setAttribute(Qt.WA_TranslucentBackground)
        self.button_inactive.setToolTip(self.title)

        # Add widgets to the tab window
        self.widgets = widgets

        self.layout = QGridLayout()
        self.layout.setSpacing(2)
        self.layout.setContentsMargins(10,10,10,10)
        # TODO self.setStyleSheet(TODO)

        row = 0
        for widget in self.widgets:
            self.layout.addWidget(widget, row, 0, 1, 5)
            try:
                widget.setAlignment(Qt.AlignRight | Qt.AlignTop)
            except:
                continue
            row += 1

        # Fill the bottom with a variable-size widget to keep all others pushed against the top of the region
        self.filler = QWidget()
        self.filler.setSizePolicy(QSizePolicy.Expanding | QSizePolicy.Preferred, QSizePolicy.Expanding | QSizePolicy.Preferred)
        self.filler.setAttribute(Qt.WA_TranslucentBackground)
        self.filler.setContextMenuPolicy(Qt.PreventContextMenu)
        self.layout.addWidget(self.filler, row, 0, 1, 5)
        self.setLayout(self.layout)

    def onClick(self):
        return


class PoTToggleButton(QFrame):
    """A small region containing a button which toggles between two states"""
    def __init__(self, parent=None, offText="Disabled", onText="Enabled", text=None):

        super().__init__()
        self.parent = parent
        self.text = text
        self.button = QPushButton(self.text)
        self.button.setFixedWidth(150)
        self.button.clicked.connect(self.onClick)
        self.button.clicked.connect(lambda: self.updateValue(self.state))

        self.layout = QGridLayout()
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(5, 1, 5, 1)
        self.layout.addWidget(self.button, 0, 0)

        self.setLayout(self.layout)

        self.offText = offText
        self.onText = onText
        self.buttonText = offText
        self.button.setText(self.buttonText)
        self.state = "TRUE"

    def updateValue(self, value):
        """Update the button state when the serial parameter variable changes"""
        self.state = value
        if self.state == "TRUE":
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.onText
        else:
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.offText
        self.button.setText(self.buttonText)

    def onClick(self):
        pass


class PoTPushbutton(QFrame):
    """A small region containing a push button"""
    def __init__(self, parent=None, text=None):
        super().__init__()
        self.parent = parent
        self.text = text

        self.button = QPushButton(self.text)
        self.button.setFixedWidth(150)
        # TODO self.button.setStyleSheet(TODO)
        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(5, 1, 5, 1)

        self.layout.addWidget(self.button, 0, 0)
        self.button.clicked.connect(self.onClick)

        self.label = QLabel()
        self.label.setText(self.text)

    def onClick(self):
        return


class PoTWidgetCluster(QGroupBox):
    """A parameter cluster which may contain multiple widgets"""
    def __init__(self, parent=None,
                 text="", widgets=[],
                 row=0, column=0):      
        super().__init__()
        self.parent = parent
        self.text = text
        self.row = row
        self.column = column
        self.layout = QGridLayout()
        self.layout.setSpacing(10)
        self.layout.setContentsMargins(5, 1, 5, 1)

        self.setContextMenuPolicy(Qt.PreventContextMenu)
        # TODO self.setStyleSheet(TODO)

        self.label_group = QLabel()
        self.label_group.setText(self.text)
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
                #widget.setAlignment(Qt.AlignRight | Qt.AlignTop)
                col += 1


class PoTTabWidget(QTabWidget):
    """A container object for adding tabs to"""
    def __init__(self, pages=None):
        super().__init__()

        self.pages = pages
        self.index_previous = 0
        for i, page in enumerate(pages):
            self.addTab(page, "")
            if page.icon:
                self.tabBar().setTabButton(i, QTabBar.LeftSide, page.button_inactive)
            page.parent = self


class PoTInputFrame(QFrame):
    """A small region which has information tied to a serial parameter"""
    def __init__(self, parent=None,
                 row=0, column=0, index=0,
                 parameter=None,
                 text="",
                 constants=[]
                 ):
        super().__init__()

        self.parent = parent
        self.row = row
        self.column = column
        self.setFixedWidth(220)
        self.setFixedHeight(50)

        self.parameter = parameter
        self.layout = QGridLayout()
        self.setLayout(self.layout)
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(5, 1, 5, 1)

        self.constants = constants
        self._text = text

        self.label = QLabel()
        self.label.setText(self.text)

        self.layout.addWidget(self.label, 0, 0)
        self.label.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)

        self.index = index

    @property
    def text(self):
        return self._text

    @text.setter
    def text(self, t):
        self._text = t
        self.label.setText(t)


class ColorConfigTab(PoTTab):
    def __init__(self, parent=None, protocol=None, title="", icon="", index=0, color=""):
        with open(parent.get_resource("config/midi_control_codes.json")) as f:
            self.MIDI_descriptions = json.loads(f.read())

        self.widgets = [
            WidgetEnable(parent=parent, protocol=protocol, text="Color is Currently:", color=color),
            WidgetNoteAndMode(parent=parent, protocol=protocol, text="Scale:", color=color),
            WidgetCCValue(parent=parent, protocol=protocol, text="MIDI Control Code:", color=color,
                          serial_dict=self.MIDI_descriptions),
            WidgetPitchBendValue(parent=parent, protocol=protocol, text="Pitch Bend:", color=color),
            WidgetOffsets(parent=parent, protocol=protocol, text="Offsets:", color=color),
            WidgetQuit(parent=parent, text="Danger Section:")
        ]
        super().__init__(parent=parent, protocol=protocol, title=title, icon=icon, index=index, widgets=self.widgets)
        self.button_active.setContextMenuPolicy(Qt.PreventContextMenu)

    def disableAllWidgets(self):
        for widget in self.widgets:
            if not isinstance(widget, WidgetEnable) and not isinstance(widget, WidgetQuit):
                widget.setEnabled(False)

    def enableAllWidgets(self):
        for widget in self.widgets:
            if not isinstance(widget, WidgetEnable) and not isinstance(widget, WidgetQuit):
                widget.setEnabled(True)

    def enablePitchbendWidget(self):
        for widget in self.widgets:
            if isinstance(widget, WidgetPitchBendValue):
                for sub_component in widget.widgets:
                    if not isinstance(sub_component, PitchbendEnableButton):
                        sub_component.setVisible(True)

    def disablePitchbendWidget(self):
        for widget in self.widgets:
            if isinstance(widget, WidgetPitchBendValue):
                for sub_component in widget.widgets:
                    if not isinstance(sub_component, PitchbendEnableButton):
                        sub_component.setVisible(False)

    def enablePitchbendCC(self):
        for widget in self.widgets:
            if isinstance(widget, WidgetPitchBendValue):
                for sub_component in widget.widgets:
                    if isinstance(sub_component, PoTSerialEntry):
                        sub_component.setEnabled(True)

    def disablePitchbendCC(self):
        for widget in self.widgets:
            if isinstance(widget, WidgetPitchBendValue):
                for sub_component in widget.widgets:
                    if isinstance(sub_component, PoTSerialEntry):
                        sub_component.setEnabled(False)

    def fullReload(self):
        for widget in self.widgets:
            for sub_component in widget.widgets:
                try:
                    sub_component.reload()
                except AttributeError:
                    pass


class PoTComboBox(PoTInputFrame):
    def __init__(self, parent=None, text=None, parameter=None, color=None, keys=[], values=[], config_name=None):
        super().__init__(parent=parent, parameter=parameter)
        self.parent = parent
        self.text = text
        self.parameter = parameter
        self.color = color
        self.values = list(values)
        self.keys = list(keys)
        self.config_name = config_name

        self.combo_box = QComboBox()
        self.combo_box.activated[str].connect(self.onChanged)
        for key in keys:
            self.combo_box.addItem(key)
        self.combo_box.setFixedWidth(90)
        # TODO self.combo_box.setStyleSheet(TODO)

        self.layout.addWidget(self.combo_box, 0, 1, 1, 4)

    def reload(self):
        try:
            self.combo_box.setCurrentIndex(self.values.index(self.parameter.variable.value))
        except ValueError:
            pass

    def onChanged(self, text):
        self.combo_box.adjustSize()
        self.parent.si.send_serial_command(cmd="color", argument=self.color)
        my_argument = self.values[self.keys.index(text)]
        self.parent.si.send_serial_command(cmd=self.config_name, argument=my_argument)
        self.parameter = my_argument
        self.combo_box.setCurrentIndex(self.values.index(self.parameter))

    def updateValue(self):
        self.combo_box.setCurrentIndex(self.values.index(self.parameter))


class PoTRestoreDefaultsButton(PoTPushbutton):
    def __init__(self, parent=None, text="Restore Defaults"):
        super().__init__(text=text)
        self.parent = parent
        self.text = text

    def onClick(self):
        reply = QMessageBox.question(
            self, "Message",
            "Are you sure you want to restore defaults?\nThis cannot be undone!",
            QMessageBox.Cancel | QMessageBox.RestoreDefaults,
            QMessageBox.Cancel
        )

        if reply == QMessageBox.RestoreDefaults:
            self.parent.si.send_serial_command(cmd=CMD_RESTORE_DEFAULTS, argument=None)
            self.parent.si.set_gui_config_from_serial(self.parent.proto)
            self.parent.tabs.pages[self.parent.tabs.currentIndex()].fullReload()
        else:
            pass


class PoTQuitButton(PoTPushbutton):
    def __init__(self, parent=None, text="Quit"):
        super().__init__(text=text)
        self.parent = parent
        self.text = text

    def onClick(self):
        reply = QMessageBox.question(
            self, "Message",
            "Are you sure you want to quit? \nNOTE: All config changes are saved when changed.",
            QMessageBox.Cancel | QMessageBox.Close,
            QMessageBox.Cancel)

        if reply == QMessageBox.Close:
            self.parent.si.send_serial_command(cmd=CMD_EXIT, argument=None)
            QApplication.quit()
        else:
            pass


class PoTSerialEntry(PoTEntry):
    def __init__(self, parent=None, text=None, parameter=None, color=None, config_name=None):
        super().__init__()
        self.parent = parent
        self.text = text
        self.parameter = parameter
        self.color = color
        self.config_name = config_name
        self.spinbox_set.setDecimals(0)

    def onEditingFinished(self):
        self.parameter = int(self.spinbox_set.value())
        self.parent.si.send_serial_command(cmd="color", argument=self.color)
        self.parent.si.send_serial_command(cmd=self.config_name, argument=self.parameter)


class PoTControlCodeEntry(PoTEntry):
    def __init__(self, parent=None, text=None, parameter=None, color=None, helpText=None, configDict=None):
        super().__init__(parent=parent, text=text, parameter=parameter)
        self.parent = parent
        self.text = text
        self.parameter = parameter
        self.color = color
        self.helpText = helpText
        self.configDict = configDict
        self.parameter = int(self.spinbox_set.value())
        self.helpText.updateValue(self.configDict[str(self.parameter)])
        self.spinbox_set.setDecimals(0)

    def reload(self):
        self.helpText.updateValue(self.configDict[str(self.parameter)])

    def onEditingFinished(self):
        self.parameter = int(self.spinbox_set.value())
        self.parent.si.send_serial_command(cmd="color", argument=self.color)
        self.parent.si.send_serial_command(cmd="control", argument=self.parameter)

        self.helpText.updateValue(self.configDict[str(self.parameter)])


class PoTEnableButton(PoTToggleButton):
    def __init__(self, parent=None, color=None, config_name=None, text=None):
        super().__init__(parent=parent, text=text)
        self.parent = parent
        self.config_name = config_name
        self.color = color

    def reload(self):
        if "TRUE" == self.state:
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.onText
            for page in self.parent.tabs.pages:
                page.enableAllWidgets()
        else:
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.offText
            for page in self.parent.tabs.pages:
                page.disableAllWidgets()

    def onClick(self):
        self.state = "FALSE" if self.state == "TRUE" else "TRUE"
        self.updateValue(self.state)
        if "TRUE" == self.state:
            for page in self.parent.tabs.pages:
                page.enableAllWidgets()
        else:
            for page in self.parent.tabs.pages:
                page.disableAllWidgets()

        if self.parent is not None:
            self.parent.si.send_serial_command(cmd="color", argument=self.color)
            self.parent.si.send_serial_command(cmd=self.config_name,
                                               argument=self.state)


class PitchbendEnableButton(PoTToggleButton):
    def __init__(self, parent=None, enabled_parameter=None, color=None, text=None):
        super().__init__(parent=parent, text=text)
        self.parent = parent
        self.color = color
        self.enabledParameter = enabled_parameter
        self.text = text

    def reload(self):
        if "TRUE" == self.state:
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.onText
            for page in self.parent.tabs.pages:
                page.enablePitchbendWidget()
        else:
            # TODO self.button.setStyleSheet(TODO)
            self.buttonText = self.offText
            for page in self.parent.tabs.pages:
                page.disablePitchbendWidget()

    def onClick(self):
        self.state = "FALSE" if self.state == "TRUE" else "TRUE"
        self.updateValue(self.state)
        if self.parent is not None:
            if "TRUE" == self.state:
                self.parent.si.send_serial_command(cmd="color", argument=self.color)
                self.enabledParameter = 255
                self.parent.si.send_serial_command(cmd="pitchbend", argument=self.enabledParameter)
                for page in self.parent.tabs.pages:
                    page.enablePitchbendWidget()
            else:
                self.parent.si.send_serial_command(cmd="color", argument=self.color)
                self.parent.si.send_serial_command(cmd="pitchbend", argument=self.enabledParameter)
                for page in self.parent.tabs.pages:
                    page.disablePitchbendWidget()


class PitchbendComboBox(PoTInputFrame):
    def __init__(self, parent=None, text=None, parameter=None, enabled_parameter=None, color=None, keys=[], values=[]):
        super().__init__(parent=parent, parameter=parameter)
        self.parent = parent
        self.text = text
        self.parameter = parameter
        self.enabledParameter = enabled_parameter
        self.color = color
        self.values = list(values)
        self.keys = list(keys)

        self.combo_box = QComboBox()
        self.combo_box.activated[str].connect(self.onChanged)
        for key in keys:
            self.combo_box.addItem(key)

        # TODO self.combo_box.setStyleSheet(TODO)

        self.layout.addWidget(self.combo_box, 0, 1, 1, 4)
        self.combo_box.setFixedWidth(90)

    def reload(self):
        if self.combo_box.currentIndex() == 0:
            for page in self.parent.tabs.pages:
                page.disablePitchbendCC()
        else:
            if self.enabledParameter < 128:
                for page in self.parent.tabs.pages:
                    page.enablePitchbendCC()

    def onChanged(self, text):
        self.combo_box.adjustSize()
        if text == "Pitch Bend":
            self.parent.si.send_serial_command(cmd="color", argument=self.color)
            self.enabledParameter = MIDI_CC_P_BEND
            self.parent.si.send_serial_command(cmd="pitchbend", argument=self.enabledParameter)
            for page in self.parent.tabs.pages:
                page.disablePitchbendCC()
        else:
            self.parent.si.send_serial_command(cmd="color", argument=self.color)
            self.parent.si.send_serial_command(cmd="pitchbend", argument=self.enabledParameter)
            for page in self.parent.tabs.pages:
                page.enablePitchbendCC()

    def updateValue(self):
        self.combo_box.setCurrentIndex(self.parameter)


class WidgetEnable(PoTWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PoTEnableButton(
                                 parent=parent,
                                 color=color,
                                 config_name="enable"
                             )
                         ],
                         )


class WidgetNoteAndMode(PoTWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PoTComboBox(
                                 parent=parent,
                                 text="Octave: ",
                                 parameter=protocol["octave"],
                                 keys=octaveDict.keys(),
                                 values=octaveDict.values(),
                                 color=color,
                                 config_name="octave"
                             ),
                             PoTComboBox(
                                 parent=parent,
                                 text="Root: ",
                                 parameter=protocol["root_note"],
                                 keys=rootNoteDict.keys(),
                                 values=rootNoteDict.keys(),
                                 color=color,
                                 config_name="root_note"
                             ),
                             PoTComboBox(
                                 parent=parent,
                                 text="Mode: ",
                                 parameter=protocol["mode"],
                                 keys=modeDict.keys(),
                                 values=modeDict.keys(),
                                 color=color,
                                 config_name = "mode"
        )
                         ],
                         )


class WidgetCCValue(PoTWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None, serial_dict=None):
        self.info_frame = PoTInfoFrame(parent=None,
                                       row=0,
                                       column=0,
                                       index=0,
                                       parameter=protocol["control"],
                                       text="",
                                       interval=0,
                                       constants=[]
                                       )

        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PoTControlCodeEntry(
                                 parent=parent,
                                 text="CC_Idx:",
                                 parameter=protocol["control"],
                                 color=color,
                                 configDict=serial_dict,
                                 helpText=self.info_frame
                             ),
                             self.info_frame  # see above, needed variable handle for helpText below
                         ],
                         )
        self.info_frame.setFixedWidth(450)
        self.info_frame.label.setFixedWidth(60)
        self.info_frame.label.setText("INFO: ")

        self.info_frame.label_info.setFixedWidth(450)
        self.info_frame.label_info.setStyleSheet("QLabel {font: 11pt Segoe UI;}")
        self.info_frame.label_info.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)


class WidgetOffsets(PoTWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PoTSerialEntry(parent=parent, text="Offset1:", parameter=protocol["offset1"],
                                            color=color, config_name="offset1"),
                             PoTSerialEntry(parent=parent, text="Offset2:", parameter=protocol["offset2"],
                                            color=color, config_name="offset2"),
                             PoTSerialEntry(parent=parent, text="Offset3:", parameter=protocol["offset3"],
                                            color=color, config_name="offset3"),
                         ],
                         )


class WidgetPitchBendValue(PoTWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PitchbendEnableButton(
                                 parent=parent,
                                 text="Enable:",
                                 enabled_parameter=protocol["pitchbend"],
                                 color=color
                             ),
                             PitchbendComboBox(
                                 parent=parent,
                                 text="Type:",
                                 parameter=protocol["pitchbend_is_CC"],
                                 enabled_parameter=protocol["pitchbend"],
                                 keys=pitchbendDict.keys(),
                                 values=pitchbendDict.values(),
                                 color=color
                             ),
                             PoTSerialEntry(parent=parent, text="CC_Idx:",
                                            parameter=protocol["pitchbend"],
                                            color=color, config_name="pitchbend"),
                         ],
                         )


class WidgetQuit(PoTWidgetCluster):
    def __init__(self, parent=None, text=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PoTQuitButton(parent=parent, text="Exit Program"),
                             PoTRestoreDefaultsButton(parent=parent, text="Restore Defaults")
                         ],
                         )
