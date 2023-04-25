#!/bin/bash
set -e
STARTDIR=`pwd`
SELFDIR=`dirname \`realpath ${0}\``
MKIMAGE="mkimage"
IMAGE_ITS="kernel.its"

OUTPUT_DIR=output


if [ -e ${STARTDIR}/${OUTPUT_DIR} ]; then
        rm ${STARTDIR}/${OUTPUT_DIR} -rf;	
fi
mkdir ${STARTDIR}/${OUTPUT_DIR};

cp  ${STARTDIR}/${IMAGE_ITS}                     ${STARTDIR}/${OUTPUT_DIR}/
cp  ${STARTDIR}/${MKIMAGE}                       ${STARTDIR}/${OUTPUT_DIR}/
cp  ${STARTDIR}/image/zImage                     ${STARTDIR}/${OUTPUT_DIR}/
cp  ${STARTDIR}/image/suniv-f1c100s-generic.dtb  ${STARTDIR}/${OUTPUT_DIR}/devicetree.dtb
cp  ${STARTDIR}/image/u-boot-sunxi-with-spl.bin  ${STARTDIR}/${OUTPUT_DIR}/
cd  ${STARTDIR}/${OUTPUT_DIR}
${STARTDIR}/${OUTPUT_DIR}/${MKIMAGE} -f ${IMAGE_ITS} zImage.bin

${STARTDIR}/mknanduboot.sh ${STARTDIR}/${OUTPUT_DIR}/u-boot-sunxi-with-spl.bin ${STARTDIR}/${OUTPUT_DIR}/u-boot-sunxi-with-nand-spl.bin


mkdir  ${STARTDIR}/${OUTPUT_DIR}/rootfs
tar -xvf ${STARTDIR}/image/rootfs.tar -C ${STARTDIR}/${OUTPUT_DIR}/rootfs
cd ${STARTDIR}/${OUTPUT_DIR} 
mksquashfs ${STARTDIR}/${OUTPUT_DIR}/rootfs rootfs.bin -b 64k -comp xz
rm ${STARTDIR}/${OUTPUT_DIR}/rootfs -rf
mkdir  ${STARTDIR}/${OUTPUT_DIR}/sys
mksquashfs ${STARTDIR}/${OUTPUT_DIR}/sys sys.bin -b 64k -comp xz
rm ${STARTDIR}/${OUTPUT_DIR}/sys -rf

rm -rf "${GENIMAGE_TMP}"
#rm -rf "${ROOTPATH_TMP}"
echo "start genimage"
GENIMAGE_CFG=${STARTDIR}/genimage-nand.cfg
GENIMAGE_TMP=${STARTDIR}/genimage.tmp
GENIMAGE_OUTPUT=${STARTDIR}/outputimage
if [ -e ${GENIMAGE_OUTPUT} ]; then
        rm ${GENIMAGE_OUTPUT} -rf;	
fi
mkdir ${GENIMAGE_OUTPUT};

${STARTDIR}/genimage \
	--rootpath "${ROOTPATH_TMP}"     \
	--tmppath "${GENIMAGE_TMP}"    \
	--inputpath "${STARTDIR}/${OUTPUT_DIR}"  \
	--outputpath "${GENIMAGE_OUTPUT}" \
	--config "${GENIMAGE_CFG}"


rm -rf "${GENIMAGE_TMP}"
rm ${STARTDIR}/${OUTPUT_DIR} -rf