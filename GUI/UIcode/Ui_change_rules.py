# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'c:\Users\HP\Desktop\test\demo\examples\firewall\view\ui\change_rules.ui'
#
# Created by: PyQt5 UI code generator 5.15.10
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_ChangeRule(object):
    def setupUi(self, ChangeRule):
        ChangeRule.setObjectName("ChangeRule")
        ChangeRule.resize(536, 419)
        self.layoutWidget = QtWidgets.QWidget(ChangeRule)
        self.layoutWidget.setGeometry(QtCore.QRect(50, 330, 451, 51))
        self.layoutWidget.setObjectName("layoutWidget")
        self.horizontalLayout_5 = QtWidgets.QHBoxLayout(self.layoutWidget)
        self.horizontalLayout_5.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.pushButton_6 = PushButton(self.layoutWidget)
        self.pushButton_6.setMinimumSize(QtCore.QSize(120, 30))
        self.pushButton_6.setObjectName("pushButton_6")
        self.horizontalLayout_5.addWidget(self.pushButton_6)
        spacerItem = QtWidgets.QSpacerItem(
            30, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding
        )
        self.horizontalLayout_5.addItem(spacerItem)
        self.pushButton_7 = PrimaryPushButton(self.layoutWidget)
        self.pushButton_7.setMinimumSize(QtCore.QSize(120, 30))
        self.pushButton_7.setObjectName("pushButton_7")
        self.horizontalLayout_5.addWidget(self.pushButton_7)
        self.widget = QtWidgets.QWidget(ChangeRule)
        self.widget.setGeometry(QtCore.QRect(80, 30, 391, 251))
        self.widget.setObjectName("widget")
        self.gridLayout = QtWidgets.QGridLayout(self.widget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.label = StrongBodyLabel(self.widget)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.lineEdit = LineEdit(self.widget)
        self.lineEdit.setObjectName("lineEdit")
        self.lineEdit.setPlaceholderText("Please enter the modified rule ID.")
        self.gridLayout.addWidget(self.lineEdit, 0, 1, 1, 1)
        self.label_2 = StrongBodyLabel(self.widget)
        self.label_2.setObjectName("label_2")

        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)
        self.pushButton = ComboBox(self.widget)
        self.pushButton.setText("")
        self.pushButton.setObjectName("pushButton")
        item = [
            "协议类型",
            "网络接口",
            "源IP",
            "源端口",
            "目的IP",
            "目的端口",
            "开始时间",
            "结束时间",
            "规则状态",
        ]
        self.pushButton.addItems(item)
        self.gridLayout.addWidget(self.pushButton, 1, 1, 1, 1)
        self.label_3 = StrongBodyLabel(self.widget)
        self.label_3.setObjectName("label_3")
        self.gridLayout.addWidget(self.label_3, 2, 0, 1, 1)
        self.lineEdit_2 = LineEdit(self.widget)
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.lineEdit_2.setPlaceholderText("Please enter the parameter to be modified.")
        self.gridLayout.addWidget(self.lineEdit_2, 2, 1, 1, 1)

        self.retranslateUi(ChangeRule)
        QtCore.QMetaObject.connectSlotsByName(ChangeRule)

    def retranslateUi(self, ChangeRule):
        _translate = QtCore.QCoreApplication.translate
        ChangeRule.setWindowTitle(_translate("ChangeRule", "Form"))
        self.pushButton_6.setText(_translate("ChangeRule", "取消"))
        self.pushButton_7.setText(_translate("ChangeRule", "确定"))
        self.label.setText(_translate("ChangeRule", "规则ID"))
        self.label_2.setText(_translate("ChangeRule", "修改参数"))
        self.label_3.setText(_translate("ChangeRule", "修改后的值"))


from qfluentwidgets import (
    ComboBox,
    LineEdit,
    PrimaryPushButton,
    PushButton,
    StrongBodyLabel,
)
