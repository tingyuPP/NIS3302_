import sys

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QApplication
from qfluentwidgets import (
    NavigationItemPosition,
    SplitFluentWindow,
    FluentTranslator,
)
from qfluentwidgets import SplashScreen

from PyQt5.QtCore import QTimer, QSize, QEventLoop

from qfluentwidgets import FluentIcon as FIF
from resource.set_rules import SetRules
from resource.show_log import ShowLog


class Window(SplitFluentWindow):

    def __init__(self):
        super().__init__()

        # create sub interface

        self.set_rules = SetRules(self)
        self.show_log = ShowLog(self)

        self.initNavigation()
        self.initWindow()

    def initNavigation(self):  # add sub interface
        self.addSubInterface(self.set_rules, FIF.LABEL, "设置规则")
        self.addSubInterface(self.show_log, FIF.MESSAGE, "显示日志")

        self.navigationInterface.addItem(
            routeKey="settingInterface",
            icon=FIF.SETTING,
            text="设置",
            position=NavigationItemPosition.BOTTOM,
        )

        self.navigationInterface.setExpandWidth(280)

    def initWindow(self):
        self.resize(950, 700)
        self.setWindowIcon(QIcon("./view/images/logo.png"))
        self.setWindowTitle("Firewall Management System")

        # 1. 创建启动页面
        icon = QIcon("./view/images/start.png")
        self.splashScreen = SplashScreen(icon, self)
        self.splashScreen.setIconSize(QSize(102, 102))

        desktop = QApplication.desktop().availableGeometry()  # 获取屏幕大小
        w, h = desktop.width(), desktop.height()  # 获取屏幕宽高
        self.move(w // 2 - self.width() // 2, h // 2 - self.height() // 2)  # 居中显示

        # 2. 在创建其他子页面前先显示主界面
        self.show()

        # 3. 创建子界面
        self.createSubInterface()

        # 4. 隐藏启动页面
        self.splashScreen.finish()

    def createSubInterface(self):
        loop = QEventLoop(self)
        QTimer.singleShot(2000, loop.quit)
        loop.exec()


if __name__ == "__main__":
    QApplication.setHighDpiScaleFactorRoundingPolicy(
        Qt.HighDpiScaleFactorRoundingPolicy.PassThrough
    )
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)

    # setTheme(Theme.DARK)

    app = QApplication(sys.argv)

    # install translator
    translator = FluentTranslator()
    app.installTranslator(translator)

    w = Window()
    w.show()
    app.exec_()
