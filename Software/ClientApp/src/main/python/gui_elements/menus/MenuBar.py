"""
Top MenuBar for ClientApp
=============================================================

Provides traditional top menuBar for ClientApp.
"""

from PyQt5.QtWidgets import *


class FullMenuBar(QMenuBar):
    def __init__(self, parent):
        super().__init__()
        self.parent = parent

        # Create 'File' section
        self.fileMenu = QMenu("&File", self)
        self.openAction = QAction("&Open...", self)
        self.saveAction = QAction("&Save", self)
        self.exitAction = QAction("&Quit", self)
        self.fileMenu.addAction(self.openAction)
        self.fileMenu.addAction(self.saveAction)
        self.fileMenu.addAction(self.exitAction)

        # Create 'Help' section
        self.aboutMenu = QMenu("&Help", self)
        self.aboutAction = QAction("&About", self)
        self.aboutMenu.addAction(self.aboutAction)

        # add sections to top-level menu
        self.addMenu(self.aboutMenu)
        self.addMenu(self.fileMenu)

        # connect QActions to callback commands
        self.openAction.triggered.connect(self.openFile)
        self.saveAction.triggered.connect(self.saveFile)
        self.exitAction.triggered.connect(self.exit)
        self.aboutAction.triggered.connect(self.about)

    def openFile(self):
        # Logic for opening an existing file goes here...
        print("Would have opened a file")
        self.parent.openFile()

    def saveFile(self):
        # Logic for saving a file goes here...
        print("Would have saved")
        self.parent.saveFile()

    def exit(self):
        # Logic for cutting content goes here...
        print("Would have exited!")
        self.parent.exit()

    def about(self):
        # Logic for showing an about dialog content goes here...
        print("Would have displayed an about page")
        self.parent.about()
