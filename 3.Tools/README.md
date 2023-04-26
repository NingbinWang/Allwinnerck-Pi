# Tools的使用
这里主要是Windows下是如何进行烧录的

# 程序烧录

## 烧写到flash中
### Windows下安装驱动
工具软件文件夹中找到zadig-2.7.exe并打开

### 安装 FEL mode Driver
![](/1.Docs/3.Images/fel1.png)
![](/1.Docs/3.Images/fel2.png)

### 安装DFU mode Driver
![](/1.Docs/3.Images/dfu1.png)
![](/1.Docs/3.Images/dfu2.png)

## 刷机工具SUNXI-FEL
该工具是全志soc在所有flash中无法boot时，芯片自动进入FEL刷机状态，通过USB通信。
这部分会用到压缩包中的sunxi-fel.exe
不插TF卡，按住BOOT按钮后插入USB线；或者先插入USB，保持按住BOOT按钮姿势时短按下RST键，可主动进入USB刷机模式，电脑检测到新的USB烧写设备。
sunxi-fel使用时必须在命令行下，例如可以安装个git工具（https://git-scm.com/downloads）代替windows自带的终端。 正常工作的FEL设备，我们可以通过命令测试它：

> ./sunxi-fel.exe -l
> USB device 001:034   Allwinner F1C100s

## 刷机工具DFU-UTIL
此部分会用到dfu-util.exe

1. 让模块处于FEL模式。
2. 运行from-fel-to-dfu.bat脚本，双击运行。或者在Windows命令行运行脚本中的命令。
3. 此时，模块会打印一些log，当看到这些时，表明模块已处于DFU模式。
4. 会在设备管理中看到USB download gadget
5. 准备好NAND镜像，然后在Windows命令行中通过如下命令刷机

> ./dfu-util.exe -R -a all -D images\sysimage-nand.img

## TF卡烧录
准备一张TF卡和读卡器接到电脑上，打开Win32DiskImager软件，选择相应的镜像文件，点击Write开始烧写

![](/1.Docs/3.Images/windisk.png)

## 固件制作
直接执行scripts中的createimage.sh即可，主要在windows下，同时还需要lib库。

具体可参考

https://github.com/pengutronix/genimage

实际需要编译的

https://github.com/libconfuse/libconfuse


> tar xf confuse-3.2.2.tar.xz

> cd confuse-3.2.2/

> ./configure && make -j9

> sudo make install