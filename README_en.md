**Read this in other languages: [English](README_en.md), [中文](README.md).**

# NIS3302-Kernel module-based package firewall

## Instructions

This system provides two operating modes: command line interface and graphical interface. Just download the kernel module from the bin directory, along with the associated executable files.

## Kernel module usage

Install module:

```shell
sudo insmod firewall_core.ko
```

Viewing Module Information:

```shell
sudo lsmod | grep "firewall_core"
```

remove module:

```shell
sudo rmmod firewall_core
```

## The command line mode 

command format：

```shell
sudo ./firewall_cli [-operation] <parameter>
```

- `-a`：**Add rules.** The following **nine** parameters describe the rule information: protocol type (**tcp, udp, icmp, all**), network interface type (**eth0, eth1,wlan0**), source ip address, source port number, destination ip address, destination port number, source IP address, and destination port number. Start time ( **Format: "YYYY-MM-DD HH: MM: SS", with quotation marks!** ), end time, perform action (0 means block, 1 means allow to pass). If there is no special requirement, enter a default value of '$'.
- `-d`：**Delete a rule.** The following parameter indicates that you want to delete the rule whose id is the corresponding number.
- `-m`：**Modify a rule.** It is followed by **three** parameters, which represent the id of the rule to be modified, the corresponding parameter to be modified, and the modified value. Parameter names are protocol type, network interface type, source ip address, source port number, destination ip address, destination port number, start time, end time, and action
- `-l`：Lists the current rules.
- `-r`：Import rules from the specified file. The following argument is the name of the specified file.
- `-l`：Saves the rule to the specified file. The following argument is the name of the specified file.
- `-w`：Writes the current rule to the device file.
- `-h`：Show use help.

## Graphical interface
Graphical interface based on [pyqt5](https://pypi.org/project/PyQt5/), the [pyqt - fluent - widgets](https://qfluentwidgets.com/zh), the component architecture, if use the source code to run, To create a python virtual environment:

The python virtual environment is created using venv, which is a built-in tool of Python 3.3 and above, so no additional installation is required.

**Note** : The graphical interface needs to add that sudo runs under root privileges, and users may experience an error that the package cannot be found. Please refer to [Solution](https://blog.csdn.net/weixin_39589455/article/details/136092916)

Run it from the Ubuntu command line:

```shell
python -m venv demo 	#demo is the name of the virtual environment and can be changed
```

Then run the following command to activate the virtual environment:

```shell
source demo/bin/activate 	#Be sure to change it to your virtual environment name
```

In this case, the command line should display as:

```shell
(demo) xxx@xxx-virtual-machine:~/Desktop/demo/bin$ 
```

Then install [pyqt-fluent-widgets](https://qfluentwidgets.com/zh):

```shell
pip install PyQt-Fluent-Widgets -i https://pypi.org/simple/
```

Then copy the GUI file to the virtual environment (that is, the demo folder) and go to the GUI folder:

```shell
cd .. && cd GUI
```

Put the command-line executable in the same directory as`demo.py`.

Run `demo.py`:

```shell
sudo python demo.py
```

**Below are some running pictures**

**Startup interface**

![image-20240713162303221](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713162303221.png)

**Rule editing interface**

![image-20240713161310946](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161310946.png)

**Log viewing interface**

![image-20240713161357476](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161357476.png)

**Setting interface**

![image-20240713161538585](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161538585.png)

**Add rules**

![image-20240713161909286](https://cdn.jsdelivr.net/gh/zlh123123/MyPictures/image-20240713161909286.png)

## methods of compiling

todo