import sys

from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtCore import Qt
from resource.Ui_set_rules import Ui_Form


class Demo(QWidget, Ui_Form):
    def __init__(self):
        super(Demo, self).__init__()
        # self.setupUi(self)

        # self.setWindowFlags(Qt.FramelessWindowHint)  # 1
        # self.setAttribute(Qt.WA_TranslucentBackground)  # 2

        # self.close_btn.clicked.connect(self.close)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    demo = Demo()
    demo.show()
    sys.exit(app.exec_())
