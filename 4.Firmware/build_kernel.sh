#!/bin/bash
set -e
STARTDIR=`pwd`
SELFDIR=`dirname \`realpath ${0}\``
KERNEL_DIR=${STARTDIR}/sunxi_kernel
MODDIR=${STARTDIR}/outputkernel
CROSSCOMPELITE_DIR=${STARTDIR}/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/bin
TARGET_CROSS_HOST=arm-linux-gnueabi
TARGET_CROSS_COMPILE=${CROSSCOMPELITE_DIR}/${TARGET_CROSS_HOST}-
if [ ! -e ${MODDIR} ]; then \
		mkdir ${MODDIR}; \
fi
cd ${KERNEL_DIR}
#cp ./arch/arm/configs/allwinnerck_pi_defconfig ./.config
make $1 CROSS_COMPILE=${TARGET_CROSS_COMPILE}

if [ "$1" == "modules"  ]; then \
    make modules_install CROSS_COMPILE=${TARGET_CROSS_COMPILE} INSTALL_MOD_PATH=${MODDIR}; \
fi

if [ "$1" == "all"  ]; then \
    cp ${KERNEL_DIR}/arch/arm/boot/zImage ${MODDIR}/; \
    cp ${KERNEL_DIR}/arch/arm/boot/dts/suniv-f1c100s-generic.dtb ${MODDIR}/; \
fi