import json
from pyqtsa.PyQtSA import *
from gui_elements.protocol.PoTConstants import *
from serialInterpreter import CMD_EXIT, CMD_RESTORE_DEFAULTS


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


class ColorConfigTab(QSATab):
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


class PoTRestoreDefaultsButton(QSAPushbutton):
    def __init__(self, parent=None, parameter=None, text="Restore Defaults"):
        super().__init__(parent=parent, parameter=parameter, text=text)
        self.button.text = text

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


class PoTQuitButton(QSAPushbutton):
    def __init__(self, parent=None, parameter=None, text="Quit"):
        super().__init__(parent=parent, parameter=parameter, text=text)
        self.button.text = text

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


class PoTSerialEntry(QSAEntry):
    def __init__(self, parent=None, text=None, parameter=None, color=None, config_name=None):
        super().__init__(parent=parent, text=text, parameter=parameter)
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


class PoTControlCodeEntry(QSAEntry):
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


class PoTEnableButton(QSAToggleButton):
    def __init__(self, parent=None, text=None, parameter=None, color=None, config_name=None):
        super().__init__(parent=parent, text=text, parameter=parameter)
        self.parent = parent
        self.color = color
        self.parameter = parameter
        self.config_name = config_name

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


class PitchbendEnableButton(QSAToggleButton):
    def __init__(self, parent=None, text=None, parameter=None, enabled_parameter=None, color=None):
        super().__init__(parent=parent, text=text, parameter=parameter)
        self.parent = parent
        self.color = color
        self.parameter = parameter
        self.enabledParameter = enabled_parameter

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


class WidgetEnable(QSAWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PoTEnableButton(
                                 parent=parent,
                                 text="Enable:",
                                 parameter=protocol["enable"],
                                 color=color,
                                 config_name="enable"
                             )
                         ],
                         )


class WidgetNoteAndMode(QSAWidgetCluster):
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


class WidgetCCValue(QSAWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None, serial_dict=None):
        self.info_frame = QSAInfoFrame(parent=None,
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


class WidgetOffsets(QSAWidgetCluster):
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


class WidgetPitchBendValue(QSAWidgetCluster):
    def __init__(self, parent=None, text=None, protocol=None, color=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PitchbendEnableButton(
                                 parent=parent,
                                 text="Enable:",
                                 parameter=protocol["pitchbend_enable"],
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


class WidgetQuit(QSAWidgetCluster):
    def __init__(self, parent=None, text=None):
        super().__init__(parent=parent,
                         text=text,
                         widgets=[
                             PoTQuitButton(parent=parent, text="Exit Program"),
                             PoTRestoreDefaultsButton(parent=parent, text="Restore Defaults")
                         ],
                         )
