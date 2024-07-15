from PyQt5.QtGui import QColor, QIcon
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect, QApplication, QMessageBox
from PyQt5.QtCore import pyqtSlot
from UIcode.Ui_change_rules import Ui_ChangeRule

import os
import subprocess
import re

class ChangeRules(Ui_ChangeRule, QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        # 修改窗口名称
        self.setWindowTitle("修改规则")
        # 设置图标
        self.setWindowIcon(QIcon("./view/images/logo.png"))
        desktop = QApplication.desktop().availableGeometry()  # 获取屏幕大小
        w, h = desktop.width(), desktop.height()  # 获取屏幕宽高
        self.move(w // 2 - self.width() // 2, h // 2 - self.height() // 2)  # 居中显示
    
    def on_pushButton_6_clicked(self):
        self.close()
    
    @pyqtSlot()
    def on_pushButton_7_clicked(self):
        rule_id = self.lineEdit.text()
        selectedText = self.pushButton.currentText()
        if selectedText == '协议类型':
            parameter = 'ptc'
        elif selectedText == '网络接口':
            parameter = 'itf'
        elif selectedText == '源IP':
            parameter = 'sip'
        elif selectedText == '源端口':
            parameter = 'spt'
        elif selectedText == '目的IP':
            parameter = 'dip'
        elif selectedText == '目的端口':
            parameter = 'dpt'
        elif selectedText == '开始时间':
            parameter = 'btm'
        elif selectedText == '结束时间':
            parameter = 'etm'
        elif selectedText == '规则状态':
            parameter = 'act'
        value = self.lineEdit_2.text()
        command = ['sudo', './firewall_cli', '-m', rule_id, parameter, value]
        result = subprocess.run(command, capture_output=True, text = True, cwd = os.getcwd())
        clean_output = re.sub(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])', '', result.stdout)
        QMessageBox.information(self, '结果', clean_output)