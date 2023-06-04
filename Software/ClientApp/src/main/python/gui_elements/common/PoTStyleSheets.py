PoTStandardFont = "Grotesque"

# QLabel text formats
PoTStyleQLabelStandard = f'QLabel {{ font: 12pt "{PoTStandardFont}";}}'
PoTStyleQLabelBold = f'QLabel {{font: bold 12pt "{PoTStandardFont}";}}'
PoTStyleQLabelLarge = f'QLabel {{font: 13pt "{PoTStandardFont}";}}'

# Various other widget formats
PoTStyleQSplashScreen = f'QSplashScreen {{font: 32pt "{PoTStandardFont}";}}'
PoTStyleQSpinBox = f'QSpinBox {{font: 11pt "{PoTStandardFont}";}}'
PoTStyleQComboBox = f'QComboBox {{font: 11pt "{PoTStandardFont}";}} QLabel {{font: bold 9pt "{PoTStandardFont}";}}'

PoTStyleQPushbuttonGreen = (
    f'QPushButton {{'
    f'background-color: rgba(94, 230, 73, 100%);'
    f'border: 1px solid grey;'
    f'font: bold 11pt "{PoTStandardFont}";'
    f'border-radius: 4px;'
    f'border-left-color: solid grey;'
    f'border-top-color: solid grey;'
    f'}}'
)

PoTStyleQPushbuttonRed = (
    f'QPushButton {{'
    f'background-color: rgba(230, 76, 73, 100%);'
    f'color: white;'    
    f'font: bold 11pt "{PoTStandardFont}";'
    f'border-radius: 4px;'
    f'border: 1px solid grey;'
    f'border-left-color: solid grey;'
    f'border-top-color: solid grey;'
    f'}}'
)

# Widgets related to tab styling
PoTStyleTabQLabel = f'QLabel {{border: 0px; background-color:rgba(0,0,0,0%);}}'
PoTStyleTabQGroupBox = f'QGroupBox {{border: 0px; background-color: rgba(0,0,0,0%);}}'
PotStyleFullTab = (
    f'QTabWidget::pane {{'        
    f'background-color: white;'
    f'border: none;'
    f'border-bottom-left-radius: 7px;'
    f'border-bottom-right-radius: 7px;'
    f'border-top-right-radius: 7px;'
    f'}}'
    f'QTabBar::tab:selected {{'
    f'background-color: white;'
    f'border-bottom-color: 1px white;'
    f'border-top-color: 1px solid grey;'
    f'border-left-color: 1px solid grey;'
    f'border-right-color: 1px solid grey;'
    f'padding: 6px;'
    f'border-top-left-radius: 7px;'
    f'border-top-right-radius: 7px;'
    f'}}'
    f'QTabBar::tab {{'
    f'background-color: rgba(220,220,220,100%);'
    f'border: 1px solid grey;'
    f'padding: 6px;'
    f'border-top-left-radius: 7px;'
    f'border-top-right-radius: 7px;'
    f'}}'
)

PoTStyleRowQGroupBox = (
    f'QGroupBox {{'
    f'background-color: qlineargradient(x1: 0, y1: 0, x2:0, y2: 1, stop: 0 #FFFFFF stop: 1 #E0E0E0);'
    f'border: 1px solid grey;'
    f'border-radius: 3px;'
    f'}}'
)
