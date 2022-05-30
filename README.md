# Allwinnerck-pi
这个项目是在CherryPi-F1C200S上开发，后续仍会自己设计硬件，目标会将其转换成邮票核心板,底板自行设计一个带摄像头，Gsensor,并带SPI小型屏幕与GPS的核心开发板，支持语音识别功能,并将大部分的GPIO引出，用FPC连接，让极客们自己去设计，增加板子的可玩性。
设计上会改为EMMC启动，尽量使用4层板来解决硬件。

# 芯片介绍

## 开发板介绍

Cherry F1C200S采用全志F1C200S ARM926EJ-S内核处理器，片内自带64MB SIP-DDR1，DDR频率可稳定运行在156Mhz，CPU主频可达720Mhz运行。
板载USB、Flash、TF卡座、40P LCD接口、咪头等。支持720P视频输出，支持H.264、MJPEG等解码。

## 开发板功能介绍

1. 内核：ARM926EJS@900MHz
2. 内存：DDR1 内置64M
3. 支持存储卡：板载自弹式TF卡座
4. 板载Flash：1Gbit SPI NAND Flash (GD5F1GQ4UAYIG,具体可以看Spec)
5. 音频：1个咪头和1个1W功放芯片
6. LCD：40pin-0.5mm的FPC座LCD接口
7. 摄像头：24pin-0.5mm的FPC座CSI接口（可接OV2640、OV5640等，默认OV2640）
8. USB：OTG USB(TYPE-C接口)
9. UART：CH340E（TYPE-C接口）

## 开发板的外观

![](/1.Docs/3.Images/fron.png)

![](/1.Docs/3.Images/back.png)

## 芯片架构

全志的低成本序列具体如下：
![](/1.Docs/3.Images/soc.png)

芯片的架构：
![](/1.Docs/3.Images/soc.png)

# 基础知识


## 开发工具链下载

> 编译工具链官网：https://www.linaro.org/
>
> 或[Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain)，以linaro为例：进入`support->downloads`可以看到下载页面，点击`GNU cross-toolchain binary archives`，可以进入对应[下载列表](https://releases.linaro.org/components/toolchain/binaries/)，可以看到各个版本的toolchain，这里我使用的`latest-7/arm-linux-gnueabi/`即`gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi`即可。
>

## 编译环境

> sudo apt-get install xz-utils nano wget unzip build-essential git bc swig libncurses5-dev libpython3-dev libssl-dev pkg-config zlib1g-dev libusb-dev libusb-1.0-0-dev python3-pip gawk bison flex 

# 硬件设计
后续的设计：
1. 核心板邮票式接口
2. LCD换成SPI屏幕
3. 添加Gsensor
4. 添加GPS



# 软件设计
1.ISP的接入
2.SD卡录像记录

# 参考
