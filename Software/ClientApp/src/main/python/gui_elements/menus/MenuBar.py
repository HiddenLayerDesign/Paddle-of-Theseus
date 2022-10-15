"""
Top MenuBar for ClientApp
=============================================================

Provides traditional top menuBar for ClientApp.
"""
from PyQt5.QtWidgets import QMenuBar, QMenu, QAction


class FullMenuBar(QMenuBar):
    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        self.setStyleSheet("background-color: white")

        # Create 'File' section
        self.fileMenu = QMenu("&File", self)
        self.openAction = QAction("&Open...", self)
        self.saveAction = QAction("&Save", self)
        self.quitAction = QAction("&Quit", self)
        self.fileMenu.addAction(self.openAction)
        self.fileMenu.addAction(self.saveAction)
        self.fileMenu.addAction(self.quitAction)

        # Create 'Tools' section
        self.toolsMenu = QMenu("&Tools", self)
        self.dumpConfigAction = QAction("&Dump Config", self)
        self.toolsMenu.addAction(self.dumpConfigAction)

        # Create 'Help' section
        self.aboutMenu = QMenu("&Help", self)
        self.aboutAction = QAction("&About", self)
        self.aboutMenu.addAction(self.aboutAction)

        # add sections to top-level menu
        self.addMenu(self.fileMenu)
        self.addMenu(self.toolsMenu)
        self.addMenu(self.aboutMenu)

        # connect QActions to callback commands
        self.openAction.triggered.connect(self.openFile)
        self.saveAction.triggered.connect(self.saveFile)
        self.quitAction.triggered.connect(self.exit)
        self.aboutAction.triggered.connect(self.about)
        self.dumpConfigAction.triggered.connect(self.dumpConfig)

    def openFile(self):
        print("Opening config from a file")
        self.parent.menuOpenFile()

    def saveFile(self):
        print("Saving current config")
        self.parent.menuSaveFile()

    def exit(self):
        print("Exiting")
        self.parent.menuQuit()

    def about(self):
        print("Displaying an about page")
        self.parent.menuAbout()

    def dumpConfig(self):
        print("Dumping configuration")
        self.parent.menuDumpConfig()
