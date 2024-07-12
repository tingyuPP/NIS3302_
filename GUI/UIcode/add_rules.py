from PyQt5.QtGui import QColor, QIcon
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect

from UIcode.Ui_add_rules import Ui_AddRule


class addrules(Ui_AddRule, QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        # 修改窗口名称
        self.setWindowTitle("添加规则")
        # 设置图标
        self.setWindowIcon(QIcon("./view/images/logo.png"))
