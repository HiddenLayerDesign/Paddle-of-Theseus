from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QMessageBox, QApplication, QWidget, QSizePolicy, QFrame, QGridLayout

from gui_elements.common.CommonTypes import PoTRow, PoTPushbutton, PoTFiller
from src.main.resources.base.config.PoTConstants import CMD_RESTORE_DEFAULTS, CMD_EXIT


class PoTRestoreDefaultsButton(PoTPushbutton):
    def __init__(self, parent=None, text="Restore Defaults"):
        super().__init__(text=text)
        self.parent = parent
        self.text = text
        self.button.setStyleSheet('QPushButton {background-color: rgba(230, 76, 73, 100%); color: white; font: bold '
                                  '11pt "Helvetica"}'
                                  'QMessageBox {background-color: white}')

    def onClick(self):
        reply = QMessageBox.question(
            self, "Message",
            "Are you sure you want to restore defaults?\nThis cannot be undone!",
            QMessageBox.Cancel | QMessageBox.RestoreDefaults,
            QMessageBox.Cancel
        )

        if reply == QMessageBox.RestoreDefaults:
            self.parent.si.sendSerialCommand(cmd=CMD_RESTORE_DEFAULTS, argument=None)
            self.parent.protocol = self.parent.si.updateConfigFromSerial()
        else:
            pass

    def reload(self):
        pass


class PoTQuitButton(PoTPushbutton):
    def __init__(self, parent=None, text="Quit"):
        super().__init__(text=text)
        self.parent = parent
        self.text = text
        self.button.setStyleSheet('QPushButton {background-color: rgba(230, 76, 73, 100%); color: white; font: bold '
                                  '11pt "Helvetica"}')

    def onClick(self):
        reply = QMessageBox.question(
            self, "Message",
            "Are you sure you want to quit? \nNOTE: All config changes are saved when changed.",
            QMessageBox.Cancel | QMessageBox.Close,
            QMessageBox.Cancel)

        if reply == QMessageBox.Close:
            self.parent.si.sendSerialCommand(cmd=CMD_EXIT, argument=None)
            QApplication.quit()
        else:
            pass

    def reload(self):
        pass


class PoTRowDangerZone(PoTRow):
    """Provides some button actions such as restoring default config and rebooting the paddle."""

    def __init__(self, parent=None, text=None):
        self.quitButton = PoTQuitButton(parent=parent, text="Exit Program")
        self.defaultsButton = PoTRestoreDefaultsButton(parent=parent, text="Restore Defaults")

        super().__init__(
            parent=parent,
            text=text,
            widgets=[PoTFiller(), self.quitButton, self.defaultsButton]
        )
