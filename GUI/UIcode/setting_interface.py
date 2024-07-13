# coding:utf-8
from qfluentwidgets import (
    SettingCardGroup,
    SwitchSettingCard,
    FolderListSettingCard,
    OptionsSettingCard,
    PushSettingCard,
    HyperlinkCard,
    PrimaryPushSettingCard,
    ScrollArea,
    ComboBoxSettingCard,
    ExpandLayout,
    Theme,
    CustomColorSettingCard,
    setTheme,
    setThemeColor,
    RangeSettingCard,
    isDarkTheme,
)
from qfluentwidgets import FluentIcon as FIF
from qfluentwidgets import InfoBar
from PyQt5.QtCore import Qt, pyqtSignal, QUrl, QStandardPaths
from PyQt5.QtGui import QDesktopServices
from PyQt5.QtWidgets import QWidget, QLabel, QFileDialog
from UIcode.config import cfg, HELP_URL, AUTHOR, VERSION, YEAR,BLOG_URL


class SettingInterface(ScrollArea):
    """Setting interface"""

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.scrollWidget = QWidget()
        self.expandLayout = ExpandLayout(self.scrollWidget)

        # setting label
        # self.settingLabel = QLabel(self.tr("Settings"), self)

        # personalization
        self.personalGroup = SettingCardGroup(self.tr("个性化"), self.scrollWidget)
        # self.themeCard = OptionsSettingCard(
        #     cfg.themeMode,
        #     FIF.BRUSH,
        #     self.tr("应用主题"),
        #     self.tr("Change the appearance of your application"),
        #     texts=[self.tr("Light"), self.tr("Dark"), self.tr("使用系统设置")],
        #     parent=self.personalGroup,
        # )
        self.themeColorCard = CustomColorSettingCard(
            cfg.themeColor,
            FIF.PALETTE,
            self.tr("主题颜色"),
            self.tr("Change the theme color of you application"),
            self.personalGroup,
        )
        self.zoomCard = OptionsSettingCard(
            cfg.dpiScale,
            FIF.ZOOM,
            self.tr("缩放"),
            self.tr("Change the size of widgets and fonts"),
            texts=[
                "100%",
                "125%",
                "150%",
                "175%",
                "200%",
                self.tr("Use system setting"),
            ],
            parent=self.personalGroup,
        )

        # application
        self.aboutGroup = SettingCardGroup(self.tr("关于"), self.scrollWidget)
        self.helpCard = HyperlinkCard(
            HELP_URL,
            self.tr("查看帮助文档"),
            FIF.HELP,
            self.tr("帮助"),
            self.tr("Discover new features and learn useful tips about firewall"),
            self.aboutGroup,
        )
        self.aboutCard = HyperlinkCard(
            BLOG_URL,
            self.tr("GitHub主页"),
            FIF.GITHUB,
            self.tr("关于"),
            "© "
            + self.tr("Copyright")
            + f" {YEAR}, {AUTHOR}. "
            + self.tr("Version")
            + " "
            + VERSION,
            self.aboutGroup,
        )

        self.__initWidget()

    def __initWidget(self):
        self.resize(950, 700)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setViewportMargins(0, 80, 0, 20)  # 设置滚动条的位置
        self.setWidget(self.scrollWidget)
        self.setWidgetResizable(True)
        self.setObjectName("settingInterface")

        # initialize style sheet
        self.scrollWidget.setObjectName("scrollWidget")
        # self.settingLabel.setObjectName("settingLabel")
        # StyleSheet.SETTING_INTERFACE.apply(self)  # 设置样式

        # initialize layout
        self.__initLayout()
        self.__connectSignalToSlot()

    def __initLayout(self):
        # self.settingLabel.move(36, 30)

        # add cards to group

        # self.personalGroup.addSettingCard(self.themeCard)
        self.personalGroup.addSettingCard(self.themeColorCard)
        self.personalGroup.addSettingCard(self.zoomCard)

        self.aboutGroup.addSettingCard(self.helpCard)

        self.aboutGroup.addSettingCard(self.aboutCard)

        # add setting card group to layout
        self.expandLayout.setSpacing(28)
        self.expandLayout.setContentsMargins(36, 10, 36, 0)

        self.expandLayout.addWidget(self.personalGroup)

        self.expandLayout.addWidget(self.aboutGroup)

    def __showRestartTooltip(self):
        """show restart tooltip"""
        InfoBar.success(
            self.tr("设置已更新"),
            self.tr("重启应用程序以应用更改"),
            duration=1500,
            parent=self,
        )

    def __connectSignalToSlot(self):
        """connect signal to slot"""
        cfg.appRestartSig.connect(self.__showRestartTooltip)

        # personalization
        # self.themeCard.optionChanged.connect(lambda ci: setTheme(cfg.get(ci)))
        self.themeColorCard.colorChanged.connect(lambda c: setThemeColor(c))
