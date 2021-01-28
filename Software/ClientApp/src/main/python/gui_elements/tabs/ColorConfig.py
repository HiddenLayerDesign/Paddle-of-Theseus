from PyQt5.QtCore import pyqtSlot

from pyqtsa.PyQtSA import *
from serialInterpreter import modeDict


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
        self.widgets = [
            WidgetEnable(master=master, protocol=protocol, text="Color is Currently:", color=color),
            WidgetNoteAndMode(master=master, protocol=protocol, text="Scale:", color=color),
            WidgetCCValue(master=master, protocol=protocol, text="MIDI Control Code:", color=color),
            WidgetOffsets(master=master, protocol=protocol, text="Offsets:", color=color),
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
            if not isinstance(widget, WidgetEnable):
                widget.setEnabled(False)

    def enableAllWidgets(self):
        for widget in self.widgets:
            if not isinstance(widget, WidgetEnable):
                widget.setEnabled(True)

    def fullReload(self):
        self.widgets[0].widgets[0].reload()


class EnableButton(QSAToggleButton):
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
        if self.parameter.name is "root":
            my_argument = self.parameter.variable.value
        elif self.parameter.name is "mode":
            my_argument = list(modeDict.keys())[self.parameter.variable.value]
        elif self.parameter.name is "offset1" or "offset2" or "offset3" or "control":
            my_argument = self.parameter.variable.value

        self.master.si.send_serial_command(cmd=self.parameter.name, argument=my_argument)


class WidgetEnable(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None, color=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             EnableButton(
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
                             PoTSerialEntry(
                                 master=master,
                                 text="Root: ",
                                 parameter=protocol.parameters["root_note"],
                                 color=color
                             ),
                             PoTSerialEntry(
                                 master=master,
                                 text="Mode: ",
                                 parameter=protocol.parameters["mode"],
                                 color=color
                             )
                         ],
                         )


class WidgetCCValue(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None, color=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             PoTSerialEntry(master=master, text="CC_Idx:", parameter=protocol.parameters["control"], color=color)
                         ],
                         )


class WidgetOffsets(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None, color=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             PoTSerialEntry(master=master, text="Offset1:", parameter=protocol.parameters["offset1"], color=color),
                             PoTSerialEntry(master=master, text="Offset2:", parameter=protocol.parameters["offset2"], color=color),
                             PoTSerialEntry(master=master, text="Offset3:", parameter=protocol.parameters["offset3"], color=color),
                         ],
                         )
