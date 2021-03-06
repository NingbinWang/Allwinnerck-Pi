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
- [ 固件烧录](#head14)
	- [ TF卡烧录模式](#head15)
	- [ Windows烧录模式](#head16)
- [ 构建rootfs](#head17)
	- [ 构建Debian文件系统](#head18)
	- [ 使用buildroot构建文件系统](#head19)
	- [ 构建yocta文件系统](#head20)
- [ 新硬件设计](#head21)

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
此部分主要可以去看4.Firmware中的README，里面会详细介绍整个的移植过程，并且代码都已经开源的。

# <span id="head14"> 固件烧录 </span>

## <span id="head15"> TF卡烧录模式 </span>

在虚拟机中直接使用GParted等软件图形化进行分区与格式化
![](/1.Docs/3.Images/GPared.png)

当然也可以使用fdisk命令

```
sudo fdisk -l
```

若自动挂载了TF设备，先卸载（有多个分区则全部卸载）

```
sudo umount /dev/sdb1...
```

进行分区操作


```
sudo fdisk /dev/sdb
```

操作步骤如下：

1. 若已存分区即按 d 删除各个分区

2. 通过 n 新建分区，第一分区暂且申请为1M用于储存uboot-with-spl，第二分区32M用于储存Linux内核，剩下的空间都给root-fs

	> * **第一分区操作**：p 主分区、默认 1 分区、默认2048、+16M
	>
	>   ```
	>   n p [Enter] [Enter] [Enter] +1M
	>   ```
	>
	> * **第二分区操作**：p 主分区、2 分区、默认2048、+128M
	>
	>   ```
	>   n p [Enter] [Enter] [Enter] +128M
	>   ```
	>
	> * **第三分区操作**：p 主分区、3 分区、默认2048，剩下的全部分配
	>
	>   ```
	>   n p [Enter] [Enter] [Enter] [Enter]
	>   ```
	>
	> * w 保存写入并退出


分区格式化：

```
sudo mkfs.vfat /dev/sdb1 # 将第1分区格式化成FAT
sudo mkfs.vfat /dev/sdb2 # 将第2分区格式化成FAT  
sudo mkfs.ext4 /dev/sdb3 # 将第3分区格式化成EXT4
```

> **格式说明：**
>
> * EXT4：只用于Linux系统的内部磁盘
> * NTFS：与Windows共用的磁盘
> * FAT：所有系统和设备共用的磁盘

这里就需要大家将kernel中的设备树放到sdb2中，还有kernel中生成的zImage(在/arch/arm/boot/里面找)也拷贝进去。

使用dd将`u-boot-sunxi-with-spl.bin`烧写进第一分区：

```
sudo dd if=path/to/u-boot-sunxi-with-spl.bin of=/dev/sdb bs=1024 seek=8
sync
```

> **注意：**这里的`bs=1024 seek=8`是添加了8192字节的偏移，之所以要加8K偏移是因为FSBL也就是bootROM里面硬写死了会从设备的8K地址处加载SPL，然后进入uboot。因此上面烧写的时候，指定的偏移地址一定是**相对于储存设备硬件的偏移，而不是相对于分区的偏移**！

然后将rootfs整个拷贝到sdb3中即可完成烧录

## <span id="head16"> Windows烧录模式 </span>

全志还提供一套在windows下的烧录方案：

具体可以到本项目的tools目录下查看具体的使用方法。

# <span id="head17"> 构建rootfs </span>

## <span id="head18"> 构建Debian文件系统 </span>

构建文件系统之前，需要知道我们想要构建哪个版本的文件系统，这里从[Debian 全球镜像站](https://www.debian.org/mirror/list.zh-cn.html)选择访问速度快的源，这里使用华为源：mirrors.huaweicloud.com。

> **注意：**选择的源需要支持硬件架构`armel`，因为F1Cxxxs是`armel`架构的芯片。
>
> 关于不同架构的区别：
>
> - **armel**：`arm eabi little endian`的缩写，针对旧的 32 位 ARM 处理器，而不支持硬件浮点单元（FPU）
> - **armhf：**`arm hard float`的缩写，仅适用于较新的 32 位 ARM 处理器，其至少实现了 ARMv7 架构，且支持 ARM 矢量浮点规范（VFPv3）第 3 版
> - **arm64：**适用于 64 位 ARM 处理器，64位的arm默认就是hf的，其至少实现了 ARMv8 架构

然后就是debian的版本，使用最新的`buster`：

```
debootstrap --foreign --verbose --arch=armel  buster rootfs-debian http://mirrors.huaweicloud.com/debian/
cd rootfs-debian
sudo mount --bind /dev dev/
sudo mount --bind /sys sys/
sudo mount --bind /proc proc/
sudo mount --bind /dev/pts dev/pts/
cd ..
cp /usr/bin/qemu-arm-static rootfs-debian/usr/bin/
sudo chmod +x rootfs-debian/usr/bin/qemu-arm-static
sudo LC_ALL=C LANGUAGE=C LANG=C chroot rootfs-debian /debootstrap/debootstrap --second-stage --verbose
sudo LC_ALL=C LANGUAGE=C LANG=C chroot rootfs-debian
```

此时你就可以进入chroot环境了

```
apt install net-tools usbutils ssh

passwd root
# 修改密码

nano /etc/ssh/sshd_config
# 添加SSH权限，修改为PermitRootLogin yes
```

文件系统中的`/etc/init.d`负责linux的服务的开启和关闭等，为了能使系统开机自动运行一些脚本和命令，这里介绍如何新添加一个自启动项。

首先我们创建一个文件`/etc/init.d/runOnBoot`，内容如下：

```
#!/bin/sh /etc/init.d/runOnBoot
### BEGIN INIT INFO
# Provides: runOnBoot Required-Start: $local_fs $syslog $network
# Required-Stop: $local_fs $syslog $network Default-Start: 2 3 4 5
# Default-Stop: 0 1 6 Short-Description: runOnBoot startup Description:
# runOnBoot auto startup 1.0
### END INIT INFO

#------------------------------------------------------------------------------
swapon /opt/images/swap

mkdir /sys/kernel/config/usb_gadget/gg
cd /sys/kernel/config/usb_gadget/gg
echo "0x0502" > idVendor
echo "0x3235" > idProduct
mkdir functions/rndis.rn0
mkdir configs/c1.1
ln -s functions/rndis.rn0 configs/c1.1/
echo "musb-hdrc.1.auto" > UDC
ifconfig usb0 192.168.137.2
ifconfig usb0 up
route add default gw 192.168.137.1


# Demo to run a script ↓
# script_path=/home/start.sh
# if [ ! -r ${script_path} ]; then
#     echo ${script_path} not existing;
# fi
# . ${myStart_spaddr}

#------------------------------------------------------------------------------
```

给文件添加可执行权限：

```
chmod +x /etc/init.d/runOnBoot
```

最后要添加软链接：

```
ln -s /etc/init.d/runOnBoot /etc/rc2.d/S99runOnBoot
```

> `/etc/rc.d/rc0.d/`～`/etc/rc.d/rc6.d/`文件夹的含义不同，S开头代表是开启时处理的脚本，按照后面紧跟的数字进行按顺序启动，S99则是最后进行启动。

重启即可看到命令和脚本自动执行了。

### 解决root-fs分区开机后被挂载为*Read-Only*的问题

新配置的文件系统需要添加fstab进行对应分区的自动挂载，修改`/etc/fstab`文件：

```
# <file system>	<mount pt>	<type>	<options>	<dump>	<pass>
/dev/root	/		ext2	rw,noauto	0	1
proc		/proc		proc	defaults	0	0
devpts		/dev/pts	devpts	defaults,gid=5,mode=620,ptmxmode=0666	0	0
tmpfs		/dev/shm	tmpfs	mode=0777	0	0
tmpfs		/tmp		tmpfs	mode=1777	0	0
tmpfs		/run		tmpfs	mode=0755,nosuid,nodev	0	0
sysfs		/sys		sysfs	defaults	0	0
/opt/images/swap swap swap defaults 0 0
```

DNS记录在`/etc/resolv.conf`这个文件里，可以按`resolv.conf`文件的格式修改DNS。 

```
nano /etc/resolv.conf

修改为：
nameserver 8.8.8.8
```

### 启用swap

芯片的SiP内存只有64MB，大部分情况下都不够用，所以需要开启swap使用内存卡的一部分空间来作为交换内存。

通过free -m来查看下内存使用状况：

```
# free -m
              total        used        free      shared  buff/cache   available
Mem:             54          15           6           0          31          34
Swap:             0           0           0
```

创建一个自定义的目录`/opt/images/`：

```
mkdir /opt/images/
rm -rf /opt/images/swap
```

创建一个需要内存大小的文件，如512M：

```
# dd if=/dev/zero of=/opt/images/swap bs=1024 count=512000
2048000+0 records in
2048000+0 records out
2097152000 bytes (2.1 GB, 2.0 GiB) copied, 30.3635 s, 69.1 MB/s
```

把创建的文件变成SWAP分区并启用：

```
mkswap /opt/images/swap
swapon /opt/images/swap
```

`free -m`看看SWAP是否生效，ok的话设置开机自动挂载swap：

```
nano /etc/fstab

添加一行：
/opt/images/swap swap swap defaults 0 0
```

### 部署
直接插上SD卡拷贝所有文件（需要在VMware宿主机打开终端操作），在挂载的SD卡root-fs磁盘打开终端，输入：

```
cd /media/xxx/rootfs/
sudo cp -Rf path/to/rootfs-debian/* ./
```


## 使用buildroot构建文件系统




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
