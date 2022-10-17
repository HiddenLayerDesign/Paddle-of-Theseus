import json
import sys

from PyQt5.QtGui import QPixmap
from fbs_runtime.application_context.PyQt5 import ApplicationContext
from PyQt5.QtWidgets import QMainWindow, QSplashScreen, QGridLayout, QMessageBox, QTabBar, QFileDialog, QFrame, QLabel
from PyQt5.QtCore import Qt
from PyQt5 import QtGui, QtCore
from copy import deepcopy

from gui_elements.PoTColorTab import ColorConfigTab, PoTTabWidget
from gui_elements.common.PoTStyleSheets import PoTStyleQSplashScreen, PoTStyleQLabelLarge
from gui_elements.menus.MenuBar import FullMenuBar
from gui_elements import version
from gui_elements.version import __appname__, __version__, __date__

from serialInterpreter import SerialInterpreter

from gui_elements.common.PoTConstants import base_config_dict


class PoTConfigApp(ApplicationContext):

    def run(self):
        pass

    def __init__(self):
        """Initialize the application context and create/show the app window"""
        super().__init__()

        # set up window
        self.window = QMainWindow()
        self.window.setFixedSize(1000, 550)
        self.window.setWindowTitle("{0} {1}".format(version.__appname__, version.__version__))
        self.window.setWindowIcon(QtGui.QIcon(self.get_resource('images/favicon.ico')))

        self.protocol = {
            "BLUE": deepcopy(base_config_dict),
            "GREEN": deepcopy(base_config_dict),
            "RED": deepcopy(base_config_dict),
            "CYAN": deepcopy(base_config_dict),
            "PURPLE": deepcopy(base_config_dict),
            "YELLOW": deepcopy(base_config_dict),
            "WHITE": deepcopy(base_config_dict)
        }

        # set up custom rows for each possible ColorConfig
        tabs = [
            ColorConfigTab(parent=self, title="White", icon='images/tabIcon_White.png', index=0, color="WHITE"),
            ColorConfigTab(parent=self, title="Cyan", icon='images/tabIcon_Cyan.png', index=1, color="CYAN"),
            ColorConfigTab(parent=self, title="Purple", icon='images/tabIcon_Purple.png', index=2, color="PURPLE"),
            ColorConfigTab(parent=self, title="Blue", icon='images/tabIcon_Blue.png', index=3, color="BLUE"),
            ColorConfigTab(parent=self, title="Yellow", icon='images/tabIcon_Yellow.png', index=4, color="YELLOW"),
            ColorConfigTab(parent=self, title="Green", icon='images/tabIcon_Green.png', index=5, color="GREEN"),
            ColorConfigTab(parent=self, title="Red", icon='images/tabIcon_Red.png', index=6, color="RED")
        ]

        # Set up splash screen for until the serial connection is established
        self.splash = QSplashScreen(QPixmap(self.get_resource("images/splash_screen.jpg")))
        self.splash.setWindowFlags(Qt.WindowStaysOnTopHint)
        self.splash.setStyleSheet(PoTStyleQSplashScreen)
        self.splash.showMessage("Waiting for serial connection...", Qt.AlignCenter | Qt.AlignBottom, color=Qt.white)
        self.splash.show()

        # When connection is finally made, close the splash screen and go to the regular GUI
        self.si = SerialInterpreter(self)
        self.tabs = PoTTabWidget(pages=tabs)
        self.protocol = self.si.updateConfigFromSerial()
        self.splash.close()

        # Set up rows widget
        self.tabs.setCurrentIndex(0)
        self.current_tab = self.tabs.pages[self.tabs.currentIndex()]
        self.tabs.tabBar().setTabButton(self.tabs.currentIndex(), QTabBar.LeftSide,
                                        self.current_tab.button_active)

        self.configureTab()
        self.tabs.currentChanged.connect(self.configureTab)

        self.name_and_logo = QFrame()
        self.name_and_logo.layout = QGridLayout()
        self.name_and_logo.name = QLabel()
        self.name_and_logo.name.setStyleSheet(PoTStyleQLabelLarge)
        self.name_and_logo.name.setText("Paddle of Theseus Config Tool")
        self.name_and_logo.name.setContentsMargins(40, 10, 0, 10)
        self.name_and_logo.logo = QLabel()
        self.name_and_logo.logo.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)
        self.name_and_logo.logo.setPixmap(QPixmap(self.get_resource("images/Hidden_Layer_Logo.png")))
        self.name_and_logo.layout.addWidget(self.name_and_logo.logo, 0, 0, 1, 1)
        self.name_and_logo.layout.addWidget(self.name_and_logo.name, 0, 1, 1, 5)
        self.name_and_logo.setLayout(self.name_and_logo.layout)

        # set up layout
        self.layout = QGridLayout()
        self.layout.setContentsMargins(10, 10, 10, 10)
        self.layout.addWidget(self.name_and_logo, 0, 0, 1, 10)
        self.layout.addWidget(self.tabs, 2, 0, 1, 10)

        # set up frame
        self.frame = QFrame()
        self.frame.setLayout(self.layout)
        # self.frame.setStyleSheet("background-color: transparent")

        # set up menu bar
        self.menu_bar = FullMenuBar(self)
        self.window.setMenuBar(self.menu_bar)

        # Start window in splash screen until SerialInterpreter finds a connection
        self.window.setCentralWidget(self.frame)
        self.window.show()

        # Create "about" message box
        self.aboutSplash = QMessageBox()
        self.aboutSplash.setWindowTitle(f"About {__appname__}")
        self.aboutSplash.setIcon(QMessageBox.Information)
        self.aboutSplash.setText(f"{__appname__}\n{__version__}\n{__date__[:4]}-{__date__[4:6]}-{__date__[6:]}")
        self.aboutSplash.setInformativeText("Written by Chase E. Stewart for Hidden Layer Design")
        self.aboutSplash.setMinimumWidth(200)
        self.aboutSplash.setMaximumWidth(800)

    def configureTab(self):
        """Update polled loop timers and hidden protected parameters when changing between rows"""
        self.current_tab = self.tabs.pages[self.tabs.currentIndex()]

        self.tabs.tabBar().setTabButton(self.tabs.index_previous, QTabBar.LeftSide,
                                        self.tabs.pages[self.tabs.index_previous].button_inactive)

        self.tabs.tabBar().setTabButton(self.tabs.currentIndex(), QTabBar.LeftSide,
                                        self.tabs.pages[self.tabs.currentIndex()].button_active)

        self.protocol = self.si.updateConfigFromSerial()
        self.current_tab.fullReload()

        if self.protocol[self.current_tab.color]["pitchbend"] == 255:
            self.current_tab.disablePitchbendWidget()
        elif self.protocol[self.current_tab.color]["pitchbend"] == 224:
            self.current_tab.enablePitchbendWidget()
            self.current_tab.disablePitchbendCC()
        else:
            self.current_tab.enablePitchbendWidget()
            self.current_tab.enablePitchbendCC()

        self.window.repaint()
        self.tabs.index_previous = self.tabs.currentIndex()

    # noinspection PyMethodMayBeStatic
    def menuQuit(self):
        QtCore.QCoreApplication.quit()

    def menuAbout(self):
        self.aboutSplash.show()

    # noinspection PyMethodMayBeStatic
    def updateConfigFromFile(self, json_str):
        result_obj = json.loads(json_str)

        config_dict = {}
        for key in result_obj.keys():
            config_dict[key] = {}
            this_config = result_obj[key]
            config_dict[key]["control"] = int(this_config["control"])
            config_dict[key]["offset1"] = int(this_config["offset1"])
            config_dict[key]["offset2"] = int(this_config["offset2"])
            config_dict[key]["offset3"] = int(this_config["offset3"])
            config_dict[key]["octave"] = int(this_config["octave"])
            config_dict[key]["root_note"] = str(this_config["root_note"])
            config_dict[key]["mode"] = str(this_config["mode"])
            config_dict[key]["enable"] = str(this_config["enable"])
            config_dict[key]["pitchbend"] = int(this_config["pitchbend"])

        return config_dict

    def menuOpenFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        open_file_name, _ = QFileDialog.getOpenFileName(self.window, "Load paddle config (*.ptc)", "",
                                                        "Paddle Config JSON Files (*.ptc)", options=options)
        if open_file_name:
            print(open_file_name)

            with open(open_file_name, "r") as f:
                try:
                    self.protocol = self.updateConfigFromFile(f.read())
                    self.si.pushConfigOverSerial()
                except Exception as e:
                    print(e)

    def menuSaveFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        save_file_name, _ = QFileDialog.getSaveFileName(self.window, "Save paddle config (*.ptc)", "",
                                                        "Paddle Config JSON Files (*.ptc)", options=options)
        if save_file_name:
            if save_file_name[-4:] != ".ptc":
                print("Appending suffix .ptc")
                save_file_name += ".ptc"
            with open(save_file_name, "w") as f:
                try:
                    print(f'Saving to "{save_file_name}"')
                    f.write(json.dumps(self.protocol, indent=4) + "\n")
                except Exception as e:
                    print(e)

    def menuDumpConfig(self):
        print(self.protocol)


"""Run the program when `main.py` is invoked"""
if __name__ == '__main__':
    app_context = PoTConfigApp()
    exit_code = app_context.app.exec_()
    sys.exit(exit_code)
