from fbs_runtime.application_context.PyQt5 import ApplicationContext
from PyQt5.QtWidgets import QMainWindow
from serialInterpreter import serialInterpreter

import PyQt5
import requests
import sys


class PoTConfigApp(ApplicationContext):

    def __init__(self):
        """
        Initialize the application context and create/show the app window
        """
        super().__init__()

        self.si = serialInterpreter()
        self.window = QMainWindow()
        self.window.resize(250, 150)
        self.window.show()
        # self.get_resource("images/test.png")


if __name__ == '__main__':
    appctxt = PoTConfigApp()  # 1. Instantiate ApplicationContext
    exit_code = appctxt.app.exec_()  # 2. Invoke appctxt.app.exec_()
    sys.exit(exit_code)
