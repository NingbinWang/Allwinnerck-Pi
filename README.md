# Allwinnerck-pi

- [ 项目简介](#head1)
	- [ 开发板介绍](#head2)
		- [开发板功能介绍](#head3)
		- [开发板的外观](#head4)
	- [ 芯片架构](#head5)
- [ 基础知识](#head6)
		- [ F1Cxxxs芯片的上电启动顺序](#head7)
		- [ 开发工具链下载](#head8)
- [ 环境准备](#head9)
	- [ 安装依赖软件](#head10)
	- [ 安装编译工具链](#head11)
	- [ 如何编译](#head12)
- [ bootloader与kernel的移植](#head13)
- [ rootfs的制作](#head14)



# <span id="head1"> 项目简介 </span>
这个项目是在CherryPi-F1C200S上开发，后续仍会自己设计硬件，目标会将其转换成邮票核心板,底板自行设计一个带摄像头，Gsensor,并带SPI小型屏幕与GPS的核心开发板，支持语音识别功能,并将剩余的GPIO用FPC连接，让极客们自己去设计，增加板子的可玩性。

## <span id="head2"> 开发板介绍 </span>

Cherry F1C200S采用全志F1C200S ARM926EJ-S内核处理器，片内自带64MB SIP-DDR1，DDR频率可稳定运行在156Mhz，CPU主频可达720Mhz运行。
板载USB、Flash、TF卡座、40P LCD接口、咪头等。支持720P视频输出，支持H.264、MJPEG等解码。

### <span id="head3"> 开发板功能介绍 </span>

1. 内核：ARM926EJS@900MHz
2. 内存：DDR1 内置64M
3. 支持存储卡：板载自弹式TF卡座
4. 板载Flash：1Gbit SPI NAND Flash (GD5F1GQ4UAYIG,具体可以看Spec)
5. 音频：1个咪头和1个1W功放芯片
6. LCD：40pin-0.5mm的FPC座LCD接口
7. 摄像头：24pin-0.5mm的FPC座CSI接口（可接OV2640、OV5640等，默认OV2640）
8. USB：OTG USB(TYPE-C接口)
9. UART：CH340E（TYPE-C接口）

### <span id="head4"> 开发板的外观 </span>

![](/1.Docs/3.Images/fron.png)

![](/1.Docs/3.Images/back.png)

## <span id="head5"> 芯片架构 </span>

---

| 功能         | 描述                                                         |
| ------------ | ------------------------------------------------------------ |
| CPU          | ARM9 CPU architecture，16KByte D-Cache，2KByte I-Cache       |
| Memory       | SIP 64MB DDR1，SD2.0，eMMC 4.41                              |
| Video        | H.264 1920x1080@30fps decoding<br/>MPEG1/2/4 1920x1080@30fps decoding<br/>MJPEG 1280x720@30fps encoding<br/>JPEG encode size up to 8192x8192 |
| Camera       | 8-bit CMOS-sensor interface<br/>CCIR656 protocol for NTSC and PAL |
| Audio        | Integrated analog audio codec with two DAC channels and one ADC channel,maximum 192kHz DAC sample rate and 48kHz ADC sample rate<br/>One I2S/PCM interface |
| Display      | LCD RGB interface up to 1280x720@60fps<br/>TV CVBS output, support NTSC/PAL, with auto plug detecting |
| Connectivity | USB OTG, SDIO,IR, 3 x TWI, 2 x SPI, 3 x UART                 |
| OS           | Melis, Linux OS                                              |
| Package      | QFN88, 10mm x 10mm                                           |
| Process      | 40nm                                                         |
| 特点         | 支持H.264 1920x1080@30fps 解码<br/>支持MJPEG 1280x720@30fps 编码<br/>集成 64MB DDR1,集成音频CODEC<br/>低成本，低功耗，开发简单 |


全志的低成本SOC序列如下：
![](/1.Docs/3.Images/soc.png)

芯片的架构：
![](/1.Docs/3.Images/core.png)

# <span id="head6"> 基础知识 </span>

## <span id="head7"> F1Cxxxs芯片的上电启动顺序 </span>

> 芯片可以从SPI Flash或者SD-Card中启动。
>
> **参考：**[F1C100s启动时搜索SPI Flash的顺序？](https://whycan.com/t_1746.html)
>
> 1. 上电后, f1c100s内部 BROM (芯片内置，无法擦除) 启动；
> 2. 首先检查 SD0 有没有插卡, 如果有插卡就读卡 8k偏移数据，是否是合法的启动数据, 如果是BROM 引导结束, 否则进入下一步；
> 3. 检测SPI0 NOR FLASH(W25QXXX, MX25LXXX) 是否存在, 是否有合法的启动数据, 如果是BROM 引导结束, 否则进入下一步；
> 4. 检测SPI0 NAND FLASH 是否存在, 是否有合法的启动数据, 如果是BROM 引导结束, 否则进入下一步；
> 5. 因为找不到任何可以引导的介质， 系统进入usb fel 模式， 可以用USB烧录了。

## <span id="head8"> 开发工具链下载 </span>

> 编译工具链官网：https://www.linaro.org/
>
> 或[Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain)，以linaro为例：进入`support->downloads`可以看到下载页面，点击`GNU cross-toolchain binary archives`，可以进入对应[下载列表](https://releases.linaro.org/components/toolchain/binaries/)，可以看到各个版本的toolchain，这里我使用的`latest-7/arm-linux-gnueabi/`即`gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi`即可。
>

# <span id="head9"> 环境准备 </span>

## <span id="head10"> 安装依赖软件 </span>
> sudo apt-get install xz-utils vim wget unzip build-essential git bc swig libncurses5-dev libpython3-dev libssl-dev pkg-config zlib1g-dev libusb-dev libusb-1.0-0-dev python3-pip gawk bison flex 

## <span id="head11"> 安装编译工具链 </span>
主要是要将前面下载的开发工具链进行解压，并赋值到全局环境变量中，当然你也可以不使用全局环境变量，指定编译器的路径也是可以的。

下载交叉编译链：

> wget http://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/arm-linux-gnueabi/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi.tar.xz
 
 **注意：** GCC版本要大于 6；此处为获取交叉编译链为7.2.1版本，也可以自行下载其他版本。

将工具链压缩包解压

> mkdir /home/toolchain
> tar -vxf gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi.tar.xz -C /home/toolchain

配置环境变量：

> nano  ~/.bashrc

打开文件添加下面的变量：

> export PATH=$PATH:/home/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin

使环境变量立即生效:

>source ~/.bashrc 

查询版本，确认安装成功 ：

> arm-linux-gnueabi-gcc -v

## <span id="head12"> 如何编译 </span>

进入源码目录, 执行下述命令进行编译:

> make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-

在uboot顶层Makefile的250行左右，添加默认编译器，就可以直接用**make**编译省去后面的参数了:

>ifeq ($(HOSTARCH),$(ARCH))
>CROSS_COMPILE ?= 
>endif
>
>ARCH ?= arm
>CROSS_COMPILE ?= arm-linux-gnueabi-

内核编译也是同理。

# <span id="head13"> bootloader与kernel的移植 </span>
此部分主要可以去看4.Firmware中的README，里面会详细介绍整个的移植过程，并且代码都会开源的。

# <span id="head13"> rootfs的制作 </span>

使用debin的ubuntu core作为内核的rootfs

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
