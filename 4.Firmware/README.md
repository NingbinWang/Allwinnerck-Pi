# 移植笔记
目前该项目的所有的代码，都是需要进行移植的，主要是为了考验自己对于uboot与linux的理解。

固件路径如下：
* [bootloader](https://github.com/NingbinWang/sunxi_uboot)
* [kernel](https://github.com/NingbinWang/sunxi_kernel)
* [yocto](https://github.com/NingbinWang/sunxi_yocto) 暂时不研究yocto



# uboot的移植笔记
在移植过程中，最重要的是要明白在menuconfig中各个参数的一些含义，早期不是很懂的情况下，更改过很多次的deconfig才能正常启动起来。

## 基础了解
uboot的相关代码架构如下：
```
.
├── api             //封装一些平台无关的操作，如字符串打印，显示，网络，内存
├── arch            //以平台架构区分
│   ├──arm
│   │   └──cpu
│   │   │   └──arm926ejs
│   │   │   │   └──sunxi   //cpu相关的一些操作，如定时器读取
│   │   │   │   │   └──u-boot-spl.lds  //spl的放置方法
│   │   └──dts
│   │   │   └──suniv-f1c100s-generic.dts   // f1c100s芯片的一些配置
│   │   │   └──suniv-f1c100s.dtsi
│   │   │   └──suniv.dtsi
│   │   └──lib      //一些库文件
│   │   └──mach-sunxi
│   │   │   └──board.c          //board_init_f
│   │   │   └──dram_sun4i.c     //ddr的操作，复位，时钟，延时，odt，etc.
│   │   │   └──dram_helpers.c   //ddr的设置及读写测试
├── board
│   ├──sunxi
│   │   └──board.c              //sunxi_board_init 入口
│   │   └──dram_suniv.c        //DRAM的一些默认参数
├── boot            //Uboot命令启动kernel使用的命令
├── cmd             //Uboot命令行的一些命令
├── common          //含spl
├── configs         //menuconfig里的默认配置,比如各类驱动适配
│   ├── sunvi_f1c200s_defconfig
├── disk            //硬盘分区的驱动
├── doc
├── drivers         //外设驱动
├── dts
├── examples
├── env
├── fs              //多种文件系统
├── include
│   ├──configs
│   │   └──sunxi_common.h   //预配置的参数，如串口号等
│   │   └──suniv.h
├── lib             //加密压缩等算法
├── net             //nfs,tftp等网络协议
├── post
├── scripts 
├── tools
```

# kernel的移植
内核的移植，主要关注相关的patch即可