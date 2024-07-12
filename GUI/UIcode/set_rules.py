from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect

from UIcode.Ui_set_rules import Ui_setRules
from UIcode.add_rules import addrules


class SetRules(Ui_setRules, QWidget):

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        self.pushButton.clicked.connect(self.add_rule)

    def setShadowEffect(self, card: QWidget):
        shadowEffect = QGraphicsDropShadowEffect(self)
        shadowEffect.setColor(QColor(0, 0, 0, 15))
        shadowEffect.setBlurRadius(10)
        shadowEffect.setOffset(0, 0)
        card.setGraphicsEffect(shadowEffect)

    # 当按下添加规则按钮时，开启子窗口，展示add_rules界面
    def add_rule(self):
        self.add_rules = addrules()
        self.add_rules.show()
