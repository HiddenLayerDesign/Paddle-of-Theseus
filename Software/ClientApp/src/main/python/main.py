from fbs_runtime.application_context.PyQt5 import ApplicationContext
from PyQt5.QtWidgets import QMainWindow
from PyQt5 import QtGui
from copy import deepcopy

from serialInterpreter import *

from gui_elements.tabs.ColorConfig import *
from gui_elements import version
from gui_elements.menus.MenuBar import *
from gui_elements.version import __appname__, __version__, __date__


class PoTConfigApp(ApplicationContext):

    def run(self):
        pass

    def __init__(self):
        """Initialize the application context and create/show the app window"""
        super().__init__()

        # set up window
        self.window = QMainWindow()
        self.window.resize(1024, 760)
        self.window.setWindowTitle("{0} {1}".format(version.__appname__, version.__version__))
        self.window.setWindowIcon(QtGui.QIcon(self.get_resource('images/favicon.ico')))
        # TODO self.window.setStyleSheet(TODO)

        self.proto = {
            "BLUE": deepcopy(base_config_dict),
            "GREEN": deepcopy(base_config_dict),
            "RED": deepcopy(base_config_dict),
            "CYAN": deepcopy(base_config_dict),
            "PURPLE": deepcopy(base_config_dict),
            "YELLOW": deepcopy(base_config_dict),
            "WHITE": deepcopy(base_config_dict)
        }

        # set up custom tabs for each possible ColorConfig
        tabs = [
            ColorConfigTab(parent=self, protocol=self.proto["BLUE"], title="Blue",
                           icon='images/tabIcon_Blue.png', index=0, color="BLUE"),
            ColorConfigTab(parent=self, protocol=self.proto["GREEN"], title="Green",
                           icon='images/tabIcon_Green.png', index=1, color="GREEN"),
            ColorConfigTab(parent=self, protocol=self.proto["RED"], title="Red",
                           icon='images/tabIcon_Red.png', index=2, color="RED"),
            ColorConfigTab(parent=self, protocol=self.proto["CYAN"], title="Cyan",
                           icon='images/tabIcon_Cyan.png', index=3, color="CYAN"),
            ColorConfigTab(parent=self, protocol=self.proto["YELLOW"], title="Yellow",
                           icon='images/tabIcon_Yellow.png', index=4, color="YELLOW"),
            ColorConfigTab(parent=self, protocol=self.proto["PURPLE"], title="Purple",
                           icon='images/tabIcon_Purple.png', index=5, color="PURPLE"),
            ColorConfigTab(parent=self, protocol=self.proto["WHITE"], title="White",
                           icon='images/tabIcon_White.png', index=6, color="WHITE"),
        ]

        # Set up splash screen for until the serial connection is established
        self.splash = QSplashScreen(QPixmap(self.get_resource("images/splash_screen.jpg")))
        self.splash.setStyleSheet("QSplashScreen {font: 32pt Segoe UI;}")
        self.splash.showMessage("Waiting for serial connection...", Qt.AlignCenter | Qt.AlignBottom, color=Qt.white)
        self.splash.show()

        # When connection is finally made, close the splash screen and go to the regular GUI
        self.si = SerialInterpreter(self)
        self.si.set_gui_config_from_serial(self.proto)
        self.splash.close()

        # Set up tabs widget
        self.tabs = PoTTabWidget(pages=tabs)
        self.tabs.setCurrentIndex(0)
        self.tabs.tabBar().setTabButton(self.tabs.currentIndex(), QTabBar.LeftSide,
                                        self.tabs.pages[self.tabs.currentIndex()].button_active)
        self.tabs.pages[0].fullReload()
        self.tabs.currentChanged.connect(self.configureTab)
        # TODO self.tabs.setStyleSheet(TODO)

        # set up layout
        self.layout = QGridLayout()
        self.layout.setSpacing(5)
        self.layout.setContentsMargins(10, 10, 10, 10)
        self.layout.addWidget(self.tabs, 2, 0, 1, 10)

        # set up frame
        self.frame = QtWidgets.QFrame()
        # TODO self.frame.setStyleSheet(TODO)
        self.frame.setLayout(self.layout)

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

    def configureTab(self):
        """Update polled loop timers and hidden protected parameters when changing between tabs
        NOTE: I don't understand this but the library uses it and I found I needed to as well
        """
        self.window.repaint()

        self.tabs.tabBar().setTabButton(self.tabs.index_previous, QTabBar.LeftSide,
                                        self.tabs.pages[self.tabs.index_previous].button_inactive)

        self.tabs.tabBar().setTabButton(self.tabs.currentIndex(), QTabBar.LeftSide,
                                        self.tabs.pages[self.tabs.currentIndex()].button_active)

        self.tabs.pages[self.tabs.currentIndex()].fullReload()
        self.tabs.index_previous = self.tabs.currentIndex()

    def exit(self):
        QtCore.QCoreApplication.quit()

    def about(self):
        self.aboutSplash.show()

    def openFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        open_file_name, _ = QFileDialog.getOpenFileName(self.window, "Load paddle config (*.JSON)", "",
                                                        "All Files (*);;JSON Files (*.json)", options=options)
        if open_file_name:
            print(open_file_name)
            # with open(open_file_name, "w") as f:
            #     try:
            #         load_json = json.loads(f.read())
            #         self.si.set_gui_config_from_serial(load_json)
            #     except Exception as e:
            #         print(e)

    def saveFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        save_file_name, _ = QFileDialog.getSaveFileName(self.window, "Save paddle config (*.JSON)", "",
                                                        "All Files (*);;JSON Files (*.JSON)", options=options)
        if save_file_name:
            print(save_file_name)
            # with open(save_file_name, "w") as f:
            #     try:
            #         f.write(json.dumps(self.proto))
            #     except Exception as e:
            #         print(e)


"""Run the program when `main.py` is invoked"""
if __name__ == '__main__':
    app_context = PoTConfigApp()  # 1. Instantiate ApplicationContext
    exit_code = app_context.app.exec_()  # 2. Invoke appctxt.app.exec_()
    sys.exit(exit_code)
