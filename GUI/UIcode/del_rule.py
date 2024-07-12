from PyQt5.QtGui import QColor, QIcon
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect

from UIcode.Ui_del_rules import Ui_DelRule


class DelRules(Ui_DelRule, QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        # 修改窗口名称
        self.setWindowTitle("删除规则")
        # 设置图标
        self.setWindowIcon(QIcon("./view/images/logo.png"))
