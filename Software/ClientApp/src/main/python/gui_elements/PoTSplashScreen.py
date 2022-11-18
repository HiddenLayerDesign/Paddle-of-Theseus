from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QSplashScreen

from gui_elements.common.PoTStyleSheets import PoTStyleQSplashScreen


class PoTSplashScreen(QSplashScreen):
    def __init__(self, parent):
        self.parent = parent
        super().__init__(QPixmap(self.parent.get_resource("images/splash_screen.jpg")))
        self.setWindowFlags(Qt.WindowStaysOnTopHint)
        self.setStyleSheet(PoTStyleQSplashScreen)
        self.showMessage("Waiting for serial connection...", Qt.AlignCenter | Qt.AlignBottom, color=Qt.white)

    def mousePressEvent(self, arg):
        pass
