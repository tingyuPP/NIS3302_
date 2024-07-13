
from PyQt5.QtGui import QColor, QIcon
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect, QApplication, QMessageBox

from UIcode.Ui_del_rules import Ui_DelRule
import subprocess
import os
import re


class DelRules(Ui_DelRule, QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        # 修改窗口名称
        self.setWindowTitle("删除规则")
        # 设置图标
        self.setWindowIcon(QIcon("./view/images/logo.png"))
        desktop = QApplication.desktop().availableGeometry()  # 获取屏幕大小
        w, h = desktop.width(), desktop.height()  # 获取屏幕宽高
        self.move(w // 2 - self.width() // 2, h // 2 - self.height() // 2)  # 居中显示
    
    def on_pushButton_7_clicked(self):
        rule_id = self.lineEdit.text()
        command = ['sudo', './firewall_cli', '-d', rule_id]
        result = subprocess.run(command, capture_output=True, text = True, cwd = os.getcwd())
        clean_output = re.sub(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])', '', result.stdout)
        QMessageBox.information(self, '结果', clean_output)
    
    def on_pushButton_6_clicked(self):
        self.close()
