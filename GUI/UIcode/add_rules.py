from PyQt5.QtGui import QColor, QIcon
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect, QApplication, QMessageBox
from PyQt5.QtCore import pyqtSlot ,QTime
from UIcode.Ui_add_rules import Ui_AddRule


from datetime import datetime
import subprocess
import os
import re


class addrules(Ui_AddRule, QWidget):
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


    @pyqtSlot()
    def on_pushButton_7_clicked(self):
        ptc = self.pushButton.currentText()
        if(ptc=="ALL"):
            ptc = "$"
        else:
            ptc = ptc.lower()
        itf = self.lineEdit_2.text()
        #如果源ip为0.0.0.0，则默认为所有ip
        if self.spinBox.value() == 0 and self.spinBox_2.value() == 0 and self.spinBox_3.value() == 0 and self.spinBox_4.value() == 0:
            sip = "$"
        else:
            sip = f"{self.spinBox.value()}.{self.spinBox_2.value()}.{self.spinBox_3.value()}.{self.spinBox_4.value()}"
        
        if self.spinBox_5.value() == 0:
            spt = "$"
        else:
            spt = self.spinBox_5.value()
        
        #如果目的ip为0.0.0.0，则默认为所有ip
        if self.spinBox_6.value() == 0 and self.spinBox_10.value() == 0 and self.spinBox_7.value() == 0 and self.spinBox_8.value() == 0:
            dip = "$"
        else:

            dip = f"{self.spinBox_6.value()}.{self.spinBox_10.value()}.{self.spinBox_7.value()}.{self.spinBox_8.value()}"

        if self.spinBox_9.value() == 0:
            dpt = "$"
        else:
            dpt = self.spinBox_9.value()


        current_time = datetime.now()
        formatted_time = current_time.strftime("%Y-%m-%d %H:%M:%S")
        one_year_later = current_time.replace(year=current_time.year + 1)

        #如果pushButton_2为空，则默认为当前时间
        if self.pushButton_2.getDate().toString("yyyy-MM-dd") == "":
            btm = formatted_time
        else:
            btm = self.pushButton_2.getDate().toString("yyyy-MM-dd") + " " + self.pushButton_3.getTime().toString("HH:mm:ss")
        #如果pushButton_4为空，则默认为当前时间的一年后
        if self.pushButton_4.getDate().toString("yyyy-MM-dd") == "":
            etm = one_year_later.strftime("%Y-%m-%d %H:%M:%S")
        else:
            etm = self.pushButton_4.getDate().toString("yyyy-MM-dd") + " " + self.pushButton_5.getTime().toString("HH:mm:ss")

        act = "0" if self.checkBox.isChecked() else "1"

        command = ['sudo', './firewall_cli', '-a', ptc, itf, sip, str(spt), dip, str(dpt), btm, etm, act]
        result = subprocess.run(command, capture_output=True, text=True, cwd=os.getcwd())
        clean_output = re.sub(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])', '', result.stdout)
        QMessageBox.information(self, '结果', clean_output)
