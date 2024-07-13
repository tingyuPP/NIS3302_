from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import QWidget, QGraphicsDropShadowEffect
from PyQt5 import QtGui
from PyQt5.QtCore import QTimer
from PyQt5 import QtWidgets as QWidgets


from UIcode.Ui_show_log import Ui_showlog


# class ShowLog(Ui_showlog, QWidget):

#     def __init__(self, parent=None):
#         super().__init__(parent=parent)
#         self.setupUi(self)
#         #日志文件会实时更新，所以需要定时读取
#         # Set up a QTimer to periodically check for log updates
#         self.timer = QTimer()
#         self.timer.timeout.connect(self.read_log)
#         self.timer.start(5000)  # Set the interval to 5000 milliseconds (5 seconds) - you can adjust this as needed

#         # Initialize the log reading
#         self.read_log()


        
        
        
        
#         # 改变字体
#         font = QtGui.QFont()
#         font.setFamily("Comic Sans MS")
#         font.setPointSize(12)

#         for i in range(self.listWidget.count()):
#             self.listWidget.item(i).setFont(font)

        


#     #按行读取日志文件/var/log/firewall.log，并显示在listWidget中
#     def read_log(self):
#         with open("/var/log/firewall.log", "r") as f:
#             for line in f:
#                 self.listWidget.addItem(line.strip())
    

class ShowLog(Ui_showlog, QWidget):

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        
        # Set up a QTimer to periodically check for log updates
        self.timer = QTimer()
        self.timer.timeout.connect(self.read_log)
        self.timer.start(3000)  # Set the interval to 5000 milliseconds (5 seconds) - you can adjust this as needed

        # Initialize the log reading
        self.read_log()

        # Change font
        font = QtGui.QFont()
        font.setFamily("Comic Sans MS")
        font.setPointSize(12)

        for i in range(self.listWidget.count()):
            self.listWidget.item(i).setFont(font)

    # Read log file /var/log/firewall.log line by line and display in listWidget
    def read_log(self):
        # Clear existing items in listWidget
        self.listWidget.clear()

        with open("/var/log/firewall.log", "r") as f:
            for line in f:
                #修改字体
                

                self.listWidget.addItem(line.strip())
                
                font = QtGui.QFont()
                font.setFamily("Comic Sans MS")
                font.setPointSize(12)
                self.listWidget.item(self.listWidget.count()-1).setFont(font)
                #修改颜色
                #每行日志的构成为Blocked [时间] ICMP 202.120.35.189:0 -> 192.168.146.145:0
                #将每个TCP这个单词的颜色设置为粉红色，UDP设置为天蓝色，ICMP设置为草绿色
                if "TCP" in line:
                    self.listWidget.item(self.listWidget.count()-1).setForeground(QColor(255, 192, 203))
                elif "UDP" in line:
                    self.listWidget.item(self.listWidget.count()-1).setForeground(QColor(135, 206, 250))
                elif "ICMP" in line:
                    self.listWidget.item(self.listWidget.count()-1).setForeground(QColor(154, 205, 50))
                else:
                    self.listWidget.item(self.listWidget.count()-1).setForeground(QColor(255, 255, 255))

                    









