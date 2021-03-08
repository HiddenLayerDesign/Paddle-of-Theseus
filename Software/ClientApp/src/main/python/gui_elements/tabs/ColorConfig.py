import json

from PyQt5.QtCore import pyqtSlot

from pyqtsa.PyQtSA import *
from gui_elements.protocol.PoTProtocol import modeDict, rootNoteDict
from serialInterpreter import CMD_EXIT, CMD_RESTORE_DEFAULTS


class ColorConfigTab(QSATab):
    def __init__(
            self,
            master=None,
            protocol=None,
            title="",
            icon="",
            index=0,
            color=""
    ):

        with open(master.get_resource("config/midi_control_codes.json")) as f:
            self.MIDI_descriptions = json.loads(f.read())

        self.widgets = [
            WidgetEnable(master=master, protocol=protocol, text="Color is Currently:", color=color),
            WidgetNoteAndMode(master=master, protocol=protocol, text="Scale:", color=color),
            WidgetCCValue(master=master, protocol=protocol, text="MIDI Control Code:", color=color, serial_dict=self.MIDI_descriptions),
            WidgetOffsets(master=master, protocol=protocol, text="Offsets:", color=color),
            WidgetQuit(master=master, protocol=protocol, text="Danger Section:", color=color)
        ]

        super().__init__(
            master=master,
            protocol=protocol,
            title=title,
            icon=icon,
            index=index,
            widgets=self.widgets
        )
        self.button_active.setContextMenuPolicy(Qt.PreventContextMenu)

    def disableAllWidgets(self):
        for widget in self.widgets:
            if not isinstance(widget, WidgetEnable) and not isinstance(widget, WidgetQuit):
                widget.setEnabled(False)

    def enableAllWidgets(self):
        for widget in self.widgets:
            if not isinstance(widget, WidgetEnable) and not isinstance(widget, WidgetQuit):
                widget.setEnabled(True)

    def fullReload(self):
        self.widgets[0].widgets[0].reload()


class PoTComboBox(QSAVariableFrame):
    def __init__(self, master=None, text=None, parameter=None, color=None, keys=[], values=[]):
        super().__init__(master=master, parameter=parameter)
        self.master = master
        self.text = text
        self.parameter = parameter
        self.color = color
        self.values = list(values)
        self.keys = list(keys)

        self.combo_box = QComboBox()
        self.combo_box.activated[str].connect(self.onChanged)
        for key in keys:
            self.combo_box.addItem(key)

        self.combo_box.setStyleSheet(widgetStyle_spinboxActual)
        self.parameter.variable.bind_to(self.updateValue)

        self.layout.addWidget(self.combo_box, 0, 1, 1, 4)
        self.combo_box.setFixedWidth(90)

    def onChanged(self, text):
        self.combo_box.adjustSize()
        self.master.si.send_serial_command(cmd="color", argument=self.color)
        my_argument = self.values[self.keys.index(text)]
        self.master.si.send_serial_command(cmd=self.parameter.name, argument=my_argument)

    def updateValue(self, value=None):
        self.combo_box.setCurrentIndex(self.parameter.variable.value)


class PoTRestoreDefaultsButton(QSAPushbutton):
    def __init__(self, master=None,
                 parameter=None,
                 text="Restore Defaults"):
        super().__init__(master=master, parameter=parameter, text=text)
        self.button.text = text

    def onClick(self):
        reply = QMessageBox.question(
            self, "Message",
            "Are you sure you want to restore defaults?\nThis cannot be undone!",
            QMessageBox.Cancel | QMessageBox.RestoreDefaults,
            QMessageBox.Cancel
        )

        if reply == QMessageBox.RestoreDefaults:
            self.master.si.send_serial_command(cmd=CMD_RESTORE_DEFAULTS, argument=None)
            self.master.si.set_gui_config_from_serial(self.master.proto)
        else:
            pass


class PoTQuitButton(QSAPushbutton):
    def __init__(self, master=None,
                 parameter=None,
                 text="Quit"):
        super().__init__(master=master, parameter=parameter, text=text)
        self.button.text = text

    def onClick(self):
        reply = QMessageBox.question(
            self, "Message",
            "Are you sure you want to quit? \nNOTE: All config changes are saved when changed.",
            QMessageBox.Cancel | QMessageBox.Close,
            QMessageBox.Cancel)

        if reply == QMessageBox.Close:
            self.master.si.send_serial_command(cmd=CMD_EXIT, argument=None)
            QApplication.quit()
        else:
            pass


class PoTEnableButton(QSAToggleButton):
    def __init__(self, master=None, text=None, parameter=None, color=None):
        super().__init__(master=master, text=text, parameter=parameter)
        self.master = master
        self.color = color
        self.parameter = parameter

    def reload(self):
        if 1 == self.state.value:
            self.button.setStyleSheet(widgetStyle_toggleButtonEnable)
            self.buttonText = self.onText
            for page in self.master.tabs.pages:
                page.enableAllWidgets()
        else:
            self.button.setStyleSheet(widgetStyle_toggleButtonDisable)
            self.buttonText = self.offText
            for page in self.master.tabs.pages:
                page.disableAllWidgets()

    def onClick(self):
        if self.master is not None:
            self.master.si.send_serial_command(cmd="color", argument=self.color)
            self.master.si.send_serial_command(cmd=self.parameter.name, argument="TRUE" if
            0 == self.state.value else "FALSE")

            if 1 == self.state.value:
                self.button.setStyleSheet(widgetStyle_toggleButtonEnable)
                self.buttonText = self.onText
                for page in self.master.tabs.pages:
                    page.disableAllWidgets()
            else:
                self.button.setStyleSheet(widgetStyle_toggleButtonDisable)
                self.buttonText = self.offText
                for page in self.master.tabs.pages:
                    page.enableAllWidgets()


class PoTSerialEntry(QSAEntry):
    def __init__(self, master=None, text=None, parameter=None, color=None):
        super().__init__(master=master, text=text, parameter=parameter)
        self.master = master
        self.text = text
        self.parameter = parameter
        self.color = color

    def onEditingFinished(self):
        self.parameter.variable.value = int(self.spinbox_set.value())
        self.master.si.send_serial_command(cmd="color", argument=self.color)
        self.master.si.send_serial_command(cmd=self.parameter.name, argument=self.parameter.variable.value)


class PoTControlCodeEntry(QSAEntry):
    def __init__(self, master=None, text=None, parameter=None, color=None, helpText=None, configDict=None):
        super().__init__(master=master, text=text, parameter=parameter)
        self.master = master
        self.text = text
        self.parameter = parameter
        self.color = color
        self.helpText = helpText
        self.configDict = configDict
        self.parameter.variable.value = int(self.spinbox_set.value())
        self.helpText.updateValue(self.configDict[str(self.parameter.variable.value)])

    def onEditingFinished(self):
        self.parameter.variable.value = int(self.spinbox_set.value())
        self.master.si.send_serial_command(cmd="color", argument=self.color)
        self.master.si.send_serial_command(cmd=self.parameter.name, argument=self.parameter.variable.value)

        self.helpText.updateValue(self.configDict[str(self.parameter.variable.value)])


class WidgetEnable(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None, color=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             PoTEnableButton(
                                 master=master,
                                 text="Enable:",
                                 parameter=protocol.parameters["enable"],
                                 color=color
                             )
                         ],
                         )


class WidgetNoteAndMode(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None, color=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             PoTComboBox(
                                 master=master,
                                 text="Root: ",
                                 parameter=protocol.parameters["root_note"],
                                 keys=rootNoteDict.keys(),
                                 values=rootNoteDict.keys(),
                                 color=color
                             ),
                             PoTComboBox(
                                 master=master,
                                 text="Mode: ",
                                 parameter=protocol.parameters["mode"],
                                 keys=modeDict.keys(),
                                 values=modeDict.keys(),
                                 color=color
                             )
                         ],
                         )


class WidgetCCValue(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None, color=None, serial_dict=None):
        self.info_frame = QSAInfoFrame(master=None,
                                       row=0,
                                       column=0,
                                       index=0,
                                       parameter=protocol.parameters["control"],
                                       text="",
                                       interval=0,
                                       constants=[]
                            )

        super().__init__(master=master,
                         text=text,
                         widgets=[
                             PoTControlCodeEntry(
                                 master=master,
                                 text="CC_Idx:",
                                 parameter=protocol.parameters["control"],
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
    def __init__(self, master=None, text=None, protocol=None, color=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             PoTSerialEntry(master=master, text="Offset1:", parameter=protocol.parameters["offset1"],
                                            color=color),
                             PoTSerialEntry(master=master, text="Offset2:", parameter=protocol.parameters["offset2"],
                                            color=color),
                             PoTSerialEntry(master=master, text="Offset3:", parameter=protocol.parameters["offset3"],
                                            color=color),
                         ],
                         )


class WidgetQuit(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None, color=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             PoTQuitButton(master=master, text="Exit Program"),
                             PoTRestoreDefaultsButton(master=master, text="Restore Defaults")
                         ],
                         )
