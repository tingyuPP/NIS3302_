from PyQt5.QtGui import QColor, QIcon
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect, QApplication, QMessageBox

from UIcode.Ui_add_rules import Ui_AddRule

import subprocess
import os
import re

class AddRules(Ui_AddRule, QWidget):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        # 修改窗口名称
        self.setWindowTitle("添加规则")
        # 设置图标
        self.setWindowIcon(QIcon("./view/images/logo.png"))
        desktop = QApplication.desktop().availableGeometry()  # 获取屏幕大小
        w, h = desktop.width(), desktop.height()  # 获取屏幕宽高
        self.move(w // 2 - self.width() // 2, h // 2 - self.height() // 2)  # 居中显示
    
    def on_pushButton_6_clicked(self):
        self.close()

    def on_pushButton_7_clicked(self):
        ptc = self.pushButton.currentText()
        itf = self.lineEdit_2.text()
        sip = f"{self.spinBox.value()}.{self.spinBox_2.value()}.{self.spinBox_3.value()}.{self.spinBox_4.value()}"
        spt = self.spinBox_5.value()
        dip = f"{self.spinBox_6.value()}.{self.spinBox_10.value()}.{self.spinBox_7.value()}.{self.spinBox_8.value()}"
        dpt = self.spinBox_9.value()
        btm = self.pushButton_2.date().toString("yyyy-MM-dd") + " " + self.pushButton_3.time().toString("HH:mm:ss")
        etm = self.pushButton_4.date().toString("yyyy-MM-dd") + " " + self.pushButton_5.time().toString("HH:mm:ss")
        act = "DROP" if self.checkBox.isChecked() else "ACCEPT"

        command = ['sudo', './firewall_cli', '-a', ptc, itf, sip, str(spt), dip, str(dpt), btm, etm, act]
        result = subprocess.run(command, capture_output=True, text=True, cwd=os.getcwd())
        clean_output = re.sub(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])', '', result.stdout)
        QMessageBox.information(self, '结果', clean_output)
