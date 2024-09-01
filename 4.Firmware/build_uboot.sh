#!/bin/bash
set -e
STARTDIR=`pwd`
SELFDIR=`dirname \`realpath ${0}\``
UBOOT_DIR=${STARTDIR}/sunxi_uboot
CROSSCOMPELITE_DIR=${STARTDIR}/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin
TARGET_CROSS_HOST=arm-linux-gnueabi
TARGET_CROSS_COMPILE=${CROSSCOMPELITE_DIR}/${TARGET_CROSS_HOST}-
cd ${UBOOT_DIR}
make clean CROSS_COMPILE=${TARGET_CROSS_COMPILE}
cp ./configs/sunvi_f1c200s_defconfig ./.config
make CROSS_COMPILE=${TARGET_CROSS_COMPILE}

