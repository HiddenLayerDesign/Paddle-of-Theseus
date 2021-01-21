from fbs_runtime.application_context.PyQt5 import ApplicationContext
from PyQt5.QtWidgets import QMainWindow
from PyQt5 import QtGui, QtWidgets

from serialInterpreter import *


import sys

from pyqtsa.PyQtSA import *
from pyqtsa.widgetStyles import *

from gui_elements.tabs.ColorConfig import *
from gui_elements import version
from gui_elements.protocol.PoTProtocol import *


class PoTConfigApp(ApplicationContext):

    def run(self):
        pass

    def __init__(self):
        """
        Initialize the application context and create/show the app window
        """
        super().__init__()

        # set up window
        self.window = QMainWindow()
        self.window.resize(1024, 760)
        self.window.setWindowTitle("{0} {1}".format(version.__appname__, version.__version__))
        self.window.setWindowIcon(QtGui.QIcon(self.get_resource('images/favicon.ico')))
        self.window.setStyleSheet(widgetStyle_mainWindow)

        self.proto = {
            "BLUE": PoTProtocol(color="BLUE", master=self),
            "GREEN": PoTProtocol(color="GREEN", master=self),
            "RED": PoTProtocol(color="RED", master=self),
            "CYAN": PoTProtocol(color="CYAN", master=self),
            "PURPLE": PoTProtocol(color="PURPLE", master=self),
            "YELLOW": PoTProtocol(color="YELLOW", master=self),
            "WHITE": PoTProtocol(color="WHITE", master=self),
        }

        # set up custom tabs
        tabs = [
            ColorConfigTab(master=self, protocol=self.proto["BLUE"], title="Blue",
                           icon='images/tabIcon_Blue.png', index=0),
            ColorConfigTab(master=self, protocol=self.proto["GREEN"], title="Green",
                           icon='images/tabIcon_Green.png', index=1),
            ColorConfigTab(master=self, protocol=self.proto["RED"], title="Red",
                           icon='images/tabIcon_Red.png', index=2),
            ColorConfigTab(master=self, protocol=self.proto["CYAN"], title="Cyan",
                           icon='images/tabIcon_Cyan.png', index=3),
            ColorConfigTab(master=self, protocol=self.proto["YELLOW"], title="Yellow",
                           icon='images/tabIcon_Yellow.png', index=4),
            ColorConfigTab(master=self, protocol=self.proto["PURPLE"], title="Purple",
                           icon='images/tabIcon_Purple.png', index=5),
            ColorConfigTab(master=self, protocol=self.proto["WHITE"], title="White",
                           icon='images/tabIcon_White.png', index=6),
        ]

        self.si = SerialInterpreter()
        self.si.set_gui_config_from_serial(self.proto)
        self.proto["BLUE"].set_all_serial_callbacks()

        self.tabs = QSATabWidget(pages=tabs)
        self.tabs.tabBar().setTabButton(self.tabs.currentIndex(), QTabBar.LeftSide,
                                        self.tabs.pages[self.tabs.currentIndex()].button_active)

        self.tabs.setStyleSheet(widgetStyle_tabBar)

        # set up layout
        self.layout = QGridLayout()
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(10, 10, 10, 10)
        self.layout.addWidget(self.tabs, 2, 0, 1, 10)

        # set up frame
        self.frame = QtWidgets.QFrame()
        self.frame.setStyleSheet(widgetStyle_mainWindow)
        self.frame.setLayout(self.layout)

        # Start window in splash screen until SerialInterpreter finds a connection
        self.window.setCentralWidget(self.frame)
        self.window.show()


if __name__ == '__main__':
    appctxt = PoTConfigApp()  # 1. Instantiate ApplicationContext
    exit_code = appctxt.app.exec_()  # 2. Invoke appctxt.app.exec_()
    sys.exit(exit_code)
