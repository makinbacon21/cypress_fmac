#!/bin/bash

# The build shell script is for the integration of i.MX8 Android build */
CY_WIFI_BACKPORT_SRC_PATH=$1
KERNEL_OUT=$2
TARGET_ARCH=$3
CCOMPILE=$4
CCLANG_TRIPLE=$5

export PATH=`pwd`/prebuilts/clang/host/linux-x86/clang-r353983d/bin:$PATH
export PATH=`pwd`/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin:$PATH

echo $PATH

make -C $CY_WIFI_BACKPORT_SRC_PATH KLIB=$KERNEL_OUT KLIB_BUILD=$KERNEL_OUT ARCH=arm64 CC=clang CLANG_TRIPLE=$CCLANG_TRIPLE CROSS_COMPILE=$CCOMPILE defconfig-brcmfmac
make -C $CY_WIFI_BACKPORT_SRC_PATH KLIB=$KERNEL_OUT KLIB_BUILD=$KERNEL_OUT ARCH=arm64 CC=clang CLANG_TRIPLE=$CCLANG_TRIPLE CROSS_COMPILE=$CCOMPILE modules
