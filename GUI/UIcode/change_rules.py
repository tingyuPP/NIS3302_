from PyQt5.QtGui import QColor, QIcon
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect

from UIcode.Ui_change_rules import Ui_ChangeRule


class ChangeRules(Ui_ChangeRule, QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        # 修改窗口名称
        self.setWindowTitle("修改规则")
        # 设置图标
        self.setWindowIcon(QIcon("./view/images/logo.png"))
