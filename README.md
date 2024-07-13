# NIS3302 - 基于内核模块的包防火墙

## 使用说明

本系统提供了命令行界面和图形界面两种操作模式。只需下载bin目录中的内核模块，以及相关可执行文件即可。

## 内核模块使用

安装模块：

```shell
sudo insmod firewall_core.ko
```

查看模块信息：

```shell
sudo lsmod | grep "firewall_core"
```

删除模块

```shell
sudo rmmod firewall_core
```

## 命令行模式使用

命令格式：

```shell
sudo ./firewall_cli [-操作] <参数>
```

- `-a`：添加规则。后接九个参数用以描述规则信息，分别为协议类型（tcp，udp，icmp，all四种选项），网络接口类型（eth0，eth1,wlan0三种选项），源ip地址，源端口号，目的ip地址，目的端口号，开始时间（**格式为"YYYY-MM-DD HH：MM：SS"，要带上引号！**），结束时间，执行动作（0代表拦截，1代表允许通过）。若无特别要求，则输入默认值为'$'。
- `-d`：删除规则。后接一个参数代表要删除id为对应数字的规则。
- `-m`：修改规则。后接三个参数，分别代表要修改的规则id，要修改的对应参数，修改后的值。参数名称依次为ptc,itf,sip,spt,dip,dpt,btm,etm,act。
- `-l`：列出当前规则。
- `-r`：从指定文件中导入规则。后接一个参数为指定文件的文件名。
- `-l`：将规则保存到指定文件。后接一个参数为指定文件的文件名。
- `-w`：将当前规则写入设备文件。
- `-h`：显示使用帮助。

### **图形化界面使用**
图形界面基于[pyqt5](https://pypi.org/project/PyQt5/)构建，采用[pyqt-fluent-widgets](https://qfluentwidgets.com/zh)组件架构，若采用源码运行，须建立python虚拟环境。

<<<<<<< HEAD
**注意**：图形界面使用需带sudo以root权限运行，有可能会出现找不到包的问题。解决方案请参考 [解决方案](https://blog.csdn.net/weixin_39589455/article/details/136092916)
=======
python虚拟环境采用`venv`创建，该模块为python3.3以上自带工具，故不需额外安装。

在Ubuntu命令行中运行

```shell
python -m venv demo 	#demo为虚拟环境名称，可修改
```

随后运行下面的命令以激活虚拟环境

```shell
source demo/bin/activate 	#注意修改为你的虚拟环境名称哦
```

此时命令行应显示为

```shell
(demo) xxx@xxx-virtual-machine:~/Desktop/demo/bin$ 
```

随后安装[pyqt-fluent-widgets](https://qfluentwidgets.com/zh)组件库

```shell
pip install PyQt-Fluent-Widgets -i https://pypi.org/simple/
```

然后将GUI文件拷贝至该虚拟环境中（即demo文件夹中），并进入GUI文件夹

```shell
cd .. && cd GUI
```

运行`demo.py`即可

```shell
python demo.py
```

**以下为部分运行图片展示**

**启动界面**

![image-20240713162303221](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713162303221.png)

**规则编辑界面**

![image-20240713161310946](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161310946.png)

**日志查看界面**

![image-20240713161357476](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161357476.png)

**设置界面**

![image-20240713161538585](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161538585.png)

**添加规则**

![image-20240713161909286](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161909286.png)

>>>>>>> 594510957f2074eeee4516c932ed18affa55b51b
### 编译方法

todo

