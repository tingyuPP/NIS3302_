from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import QWidget, QFileDialog

from UIcode.Ui_set_rules import Ui_setRules
from UIcode.add_rules import addrules
from UIcode.del_rule import DelRules
from UIcode.change_rules import ChangeRules


class SetRules(Ui_setRules, QWidget):

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setupUi(self)
        self.pushButton.clicked.connect(self.add_rule)
        self.pushButton_2.clicked.connect(self.del_rule)
        self.pushButton_3.clicked.connect(self.change_rule)
        self.pushButton_4.clicked.connect(self.Select_a_single_file)
        self.pushButton_5.clicked.connect(self.Select_a_single_directory)

    # 当按下添加规则按钮时，开启子窗口，展示add_rules界面
    def add_rule(self):
        self.add_rules = addrules()
        self.add_rules.show()

    # 当按下删除规则按钮时，开启子窗口，展示del_rule界面
    def del_rule(self):
        self.del_rules = DelRules()
        self.del_rules.show()

    # 当按下修改规则按钮时，开启子窗口，展示change_rules界面
    def change_rule(self):
        self.change_rules = ChangeRules()
        self.change_rules.show()

    def Select_a_single_file(self):
        file_path, _ = QFileDialog.getOpenFileName(
            self, "选择文件", "", "文本文件(*.txt);;所有文件(*.*)"
        )
        if file_path:
            print(file_path)

    def Select_a_single_directory(self):
        dir_path = QFileDialog.getExistingDirectory(
            self, "选择目录", "", QFileDialog.ShowDirsOnly
        )
        if dir_path:
            print("选择的目录路径：", dir_path)
