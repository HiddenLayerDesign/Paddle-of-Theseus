import json

from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QFrame, QLabel, QGroupBox, QGridLayout, QWidget, QSizePolicy, QTabBar, QTabWidget, \
    QGraphicsDropShadowEffect
from PyQt5.QtCore import Qt

from gui_elements.common.PoTCommonTypes import PoTSerialEntry
from gui_elements.common.PoTStyleSheets import PoTStyleQLabelStandard, PoTStyleTabQLabel, PoTStyleTabQGroupBox, \
    PotStyleFullTab
from gui_elements.rows.PoTRowCC import PoTRowCC
from gui_elements.rows.PoTRowDangerZone import PoTRowDangerZone
from gui_elements.rows.PoTRowEnable import PoTRowEnable
from gui_elements.rows.PoTRowNoteMode import PoTRowNoteMode
from gui_elements.rows.PoTRowOffsets import PoTRowOffsets
from gui_elements.rows.PoTRowPitchBend import PoTRowPitchBend, PitchbendEnableButton


class PoTColorTab(QFrame):
    """A GUI region which contains all parameters relevant to one subsystem"""

    def __init__(self, serial=None, parent=None, title="", icon="", index=0, widgets=[], widthActive=160,
                 widthInactive=80):
        super().__init__()

        self.serial = serial
        self.parent = parent

        self.setContextMenuPolicy(Qt.PreventContextMenu)

        self.title = title
        self.label_title = QLabel(title)
        self.label_title.setAttribute(Qt.WA_TranslucentBackground)
        self.label_title.setStyleSheet(PoTStyleQLabelStandard)

        self.index = index
        self.widthActive = widthActive
        self.widthInactive = widthInactive

        # A simple visual representation of the subsystem
        self.iconfile = self.parent.get_resource(icon)
        self.icon = QLabel()
        self.icon.setAttribute(Qt.WA_TranslucentBackground)
        self.icon.setStyleSheet(PoTStyleTabQLabel)

        if len(self.iconfile):
            self.iconpix = QPixmap(self.iconfile)
            self.icon.setPixmap(self.iconpix)

        # Build the look of the tab for when it is active
        self.button_active = QGroupBox()
        self.button_active.setFixedHeight(46)
        self.button_active.setFixedWidth(self.widthActive)
        self.layout_buttonactive = QGridLayout()
        self.layout_buttonactive.addWidget(self.label_title, 0, 0)
        self.button_active.setLayout(self.layout_buttonactive)
        self.button_active.setStyleSheet(PoTStyleTabQGroupBox)

        self.button_active.setContextMenuPolicy(Qt.CustomContextMenu)
        self.button_active.customContextMenuRequested.connect(self.contextMenuEvent)

        # Build the look of the tab for when it is inactive
        self.button_inactive = QGroupBox()
        self.button_inactive.setFixedHeight(46)
        self.button_inactive.setFixedWidth(self.widthInactive)
        self.layout_buttoninactive = QGridLayout()
        self.button_inactive.setLayout(self.layout_buttoninactive)
        self.layout_buttoninactive.addWidget(self.icon, 0, 0)
        self.button_inactive.setStyleSheet(PoTStyleTabQGroupBox)
        self.button_inactive.setAttribute(Qt.WA_TranslucentBackground)
        self.button_inactive.setToolTip(self.title)

        # Add rows to the tab window
        self.widgets = widgets

        self.layout = QGridLayout()
        self.layout.setSpacing(2)
        self.layout.setContentsMargins(10, 10, 10, 10)

        row = 0
        for widget in self.widgets:
            self.layout.addWidget(widget, row, 0, 1, 5)
            try:
                widget.setAlignment(Qt.AlignRight | Qt.AlignTop)
            except:
                continue
            row += 1

        # Fill the bottom with a variable-size widget to keep all others pushed against the top of the region
        self.filler = QWidget()
        self.filler.setSizePolicy(QSizePolicy.Expanding | QSizePolicy.Preferred,
                                  QSizePolicy.Expanding | QSizePolicy.Preferred)
        self.filler.setAttribute(Qt.WA_TranslucentBackground)
        self.filler.setContextMenuPolicy(Qt.PreventContextMenu)
        self.layout.addWidget(self.filler, row, 0, 1, 5)
        self.setLayout(self.layout)

    def onClick(self):
        return


class PoTTabWidget(QTabWidget):
    """A container object for adding rows to"""

    def __init__(self, pages=None):
        super().__init__()

        self.effect = QGraphicsDropShadowEffect()
        self.effect.setColor(Qt.black)
        self.effect.setBlurRadius(15)
        self.effect.setXOffset(-2)
        self.effect.setYOffset(2)
        self.setGraphicsEffect(self.effect)

        self.setTabShape(QTabWidget.Rounded)
        self.setStyleSheet(PotStyleFullTab)

        self.pages = pages
        self.index_previous = 0
        for i, page in enumerate(pages):
            self.addTab(page, "")
            if page.icon:
                self.tabBar().setTabButton(i, QTabBar.LeftSide, page.button_inactive)
            page.parent = self


class ColorConfigTab(PoTColorTab):
    def __init__(self, parent=None, title="", icon="", index=0, color=None):
        self.color = color
        self.parent = parent
        with open(parent.get_resource("config/midi_control_codes.json")) as f:
            self.MIDI_descriptions = json.loads(f.read())

        self.widgets = [
            PoTRowEnable(parent=parent, text="Color is Currently:", color=self.color),
            PoTRowNoteMode(parent=parent, text="Scale:", color=self.color),
            PoTRowCC(parent=parent, text="MIDI Control Code:", color=self.color, serial_dict=self.MIDI_descriptions),
            PoTRowPitchBend(parent=parent, text="Pitch Bend:", color=self.color),
            PoTRowOffsets(parent=parent, text="Offsets:", color=self.color),
            PoTRowDangerZone(parent=parent, text="Danger Section:")
        ]
        super().__init__(parent=parent, title=title, icon=icon, index=index, widgets=self.widgets)
        self.button_active.setContextMenuPolicy(Qt.PreventContextMenu)

    def disableAllWidgets(self):
        for widget in self.widgets:
            if not isinstance(widget, PoTRowEnable) and not isinstance(widget, PoTRowDangerZone):
                widget.setVisible(False)

    def enableAllWidgets(self):
        for widget in self.widgets:
            if not isinstance(widget, PoTRowEnable) and not isinstance(widget, PoTRowDangerZone):
                widget.setVisible(True)

    def enablePitchbendWidget(self):
        for widget in self.widgets:
            if isinstance(widget, PoTRowPitchBend):
                for sub_component in widget.widgets:
                    if not isinstance(sub_component, PitchbendEnableButton):
                        sub_component.setVisible(True)
                    else:
                        sub_component.button.setText(sub_component.onText)

    def disablePitchbendWidget(self):
        for widget in self.widgets:
            if isinstance(widget, PoTRowPitchBend):
                for sub_component in widget.widgets:
                    if not isinstance(sub_component, PitchbendEnableButton):
                        sub_component.setVisible(False)
                    else:
                        sub_component.button.setText(sub_component.offText)

    def enablePitchbendCC(self):
        for widget in self.widgets:
            if isinstance(widget, PoTRowPitchBend):
                for sub_component in widget.widgets:
                    if isinstance(sub_component, PoTSerialEntry):
                        sub_component.setEnabled(True)

    def disablePitchbendCC(self):
        for widget in self.widgets:
            if isinstance(widget, PoTRowPitchBend):
                for sub_component in widget.widgets:
                    if isinstance(sub_component, PoTSerialEntry):
                        sub_component.setEnabled(False)

    def fullReload(self):
        for widget in self.widgets:
            for sub_component in widget.widgets:
                sub_component.reload()
