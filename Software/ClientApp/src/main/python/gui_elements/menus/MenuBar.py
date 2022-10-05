from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPixmap, QColor, QBitmap


class FullMenuBar(QMenuBar):
    def __init__(self):
        super().__init__()

        self.openAction = QAction("&Open...", self)
        self.saveAction = QAction("&Save", self)
        self.exitAction = QAction("&Exit", self)
        self.aboutAction = QAction("&About", self)

        self.fileMenu = QMenu("&File", self)
        self.addMenu(self.fileMenu)
        self.fileMenu.addAction(self.openAction)
        self.fileMenu.addAction(self.saveAction)
        self.fileMenu.addAction(self.exitAction)
        self.aboutMenu = QMenu("&About", self)
        self.addMenu(self.aboutMenu)
        self.aboutMenu.addAction(self.aboutAction)

        self.openAction.triggered.connect(self.openFile)
        self.saveAction.triggered.connect(self.saveFile)
        self.exitAction.triggered.connect(self.exit)
        self.aboutAction.triggered.connect(self.about)

    def openFile(self):
        # Logic for opening an existing file goes here...
        print("Would have opened a file")

    def saveFile(self):
        # Logic for saving a file goes here...
        print("Would have saved")

    def exit(self):
        # Logic for cutting content goes here...
        print("Would have exited!")

    def about(self):
        # Logic for showing an about dialog content goes here...
        print("Would have displayed an about page")
