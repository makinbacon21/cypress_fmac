# SPDX-License-Identifier: ISC
#
# Makefile fragment for Broadcom 802.11 Networking Device Driver
#
# Copyright (c) 2010 Broadcom Corporation
# Copyright (C) 2018 NVIDIA Corporation. All rights reserved.
#

ccflags-y += \
	-I$(backport_srctree)/drivers/net/wireless/broadcom/brcm80211/brcmfmac \
	-I$(backport_srctree)/drivers/net/wireless/broadcom/brcm80211/include/ \
	-DCPTCFG_BRCMFMAC_NV_GPIO \
	-DCPTCFG_BRCMFMAC_NV_CUSTOM_MAC \
	-DCPTCFG_BRCMFMAC_NV_CUSTOM_FILES \
	-DCPTCFG_BRCMFMAC_NV_COUNTRY_CODE \
	-DCPTCFG_BRCMFMAC_NV_PRIV_CMD

obj-$(CPTCFG_BRCMFMAC) += brcmfmac.o
brcmfmac-objs += \
		cfg80211.o \
		chip.o \
		fwil.o \
		fweh.o \
		p2p.o \
		proto.o \
		common.o \
		core.o \
		firmware.o \
		feature.o \
		btcoex.o \
		vendor.o \
		pno.o
brcmfmac-$(CPTCFG_BRCMFMAC_PROTO_BCDC) += \
		bcdc.o \
		fwsignal.o
brcmfmac-$(CPTCFG_BRCMFMAC_PROTO_MSGBUF) += \
		commonring.o \
		flowring.o \
		msgbuf.o
brcmfmac-$(CPTCFG_BRCMFMAC_SDIO) += \
		sdio.o \
		bcmsdh.o
brcmfmac-$(CPTCFG_BRCMFMAC_USB) += \
		usb.o
brcmfmac-$(CPTCFG_BRCMFMAC_PCIE) += \
		pcie.o
brcmfmac-$(CPTCFG_BRCMDBG) += \
		debug.o
brcmfmac-$(CPTCFG_BRCM_TRACING) += \
		tracepoint.o
brcmfmac-$(CONFIG_OF) += \
		of.o
brcmfmac-$(CONFIG_DMI) += \
		dmi.o
brcmfmac-$(CPTCFG_BRCMFMAC_ANDROID) += \
		android.o \
		nv_common.o

ifneq ($(CPTCFG_NV_CUSTOM_SYSFS_TEGRA),)
brcmfmac-$(CPTCFG_NV_CUSTOM_SYSFS_TEGRA) += \
	nv/nv_custom_sysfs_tegra.o
ccflags-y += \
	-I$(backport_srctree)/drivers/net/wireless/broadcom/brcm80211/brcmfmac/nv \
	-DCPTCFG_NV_CUSTOM_SYSFS_TEGRA
endif

ifneq ($(CPTCFG_NV_CUSTOM_CAP),)
brcmfmac-$(CPTCFG_NV_CUSTOM_CAP) += \
	nv/nv_ping.o \
	nv/nv_rssi.o \
	nv/nv_tcpdump.o
ccflags-y += \
	-DCPTCFG_NV_CUSTOM_CAP
endif
