from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect
from qfluentwidgets import FluentIcon, setFont, InfoBarIcon

from resource.Ui_show_log import Ui_showlog


class ShowLog(Ui_showlog, QWidget):

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)

    def setShadowEffect(self, card: QWidget):
        shadowEffect = QGraphicsDropShadowEffect(self)
        shadowEffect.setColor(QColor(0, 0, 0, 15))
        shadowEffect.setBlurRadius(10)
        shadowEffect.setOffset(0, 0)
        card.setGraphicsEffect(shadowEffect)
