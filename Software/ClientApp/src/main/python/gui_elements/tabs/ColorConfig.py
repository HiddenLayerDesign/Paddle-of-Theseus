from pyqtsa.PyQtSA import *
from pyqtsa.widgetStyles import *


class ColorConfigTab(QSATab):
    def __init__(
            self,
            master=None,
            protocol=None,
            title="",
            icon="",
            index=0
    ):
        super().__init__(
            master=master,
            protocol=protocol,
            title=title,
            icon=icon,
            index=index,
            widgets=[
                WidgetEnable(master=master, protocol=protocol, text="Color enabled:"),
                WidgetNoteAndMode(master=master, protocol=protocol, text="Scale:"),
                WidgetCCValue(master=master, protocol=protocol, text="MIDI Control Code:"),
                WidgetOffsets(master=master, protocol=protocol, text="Offsets:"),
            ]
        )
        self.button_active.setContextMenuPolicy(Qt.PreventContextMenu)


class WidgetEnable(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[QSAToggleButtonLive(master=master, text="Enable:", parameter=protocol.parameters["enable"])],
                         )


class WidgetNoteAndMode(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             QSAEntry(
                                 master=master,
                                 text="Root: ",
                                 parameter=protocol.parameters["root_note"]
                             ),
                             QSAEntry(
                                 master=master,
                                 text="Mode: ",
                                 parameter=protocol.parameters["mode"]
                             )
                         ],
        )


class WidgetCCValue(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             QSAEntry(master=master, text="CC_Idx:", parameter=protocol.parameters["control"])
                         ],
                         )


class WidgetOffsets(QSAWidgetCluster):
    def __init__(self, master=None, text=None, protocol=None):
        super().__init__(master=master,
                         text=text,
                         widgets=[
                             QSAEntry(master=master, text="Offset1:", parameter=protocol.parameters["offset1"]),
                             QSAEntry(master=master, text="Offset2:", parameter=protocol.parameters["offset2"]),
                             QSAEntry(master=master, text="Offset3:", parameter=protocol.parameters["offset3"]),
                         ],
                         )
