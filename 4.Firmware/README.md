# 移植笔记
目前该项目的所有的代码，都是需要进行移植的，主要是为了考验自己对于uboot与linux的理解，同时会再建立buildroot以及yocto上做rootfs的开发，目前会使用debain作为自己的操作系统

# 软件设计

因为支持nand flash，那么在支持nand flash的前提下，我将修改分区表

具体如下：
1. boot0: uboot以及SPL
2. env: 用于存放环境变量
3. fdt:设备树，独立升级
4. kernel:内核分区
5. rootfs:根文件系统 
6. bsp:用于独立的内核驱动与media驱动
7. sys:linux的app功能

# uboot的移植笔记

## 基础了解

## 移植arch

## 移植SPL

## 移植uboot common

# kernel的移植