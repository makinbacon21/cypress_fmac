/*
 * nv_android.c
 *
 * NVIDIA Tegra NvCap for brcmfmac driver
 *
 * Copyright (C) 2019 NVIDIA Corporation. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/mmc/card.h>
#include <linux/wakelock.h>
#include <defs.h>
#include <brcmu_utils.h>
#include <brcmu_wifi.h>
#include "core.h"
#include "android.h"
#include "cfg80211.h"
#include "debug.h"
#include "fwil.h"
#include "nv_common.h"
#include "nv_android.h"

#define CMD_RSSI		"RSSI"
#define CMD_LINKSPEED		"LINKSPEED"

#define CMD_SET_IM_MODE		"SETMIRACAST"
#define CMD_UPDATE_CHANNEL_LIST	"UPDATE_CHANNEL_LIST"
#define CMD_RESTRICT_BW_20	"RESTRICT_BW_20"
#define CMD_MAXLINKSPEED	"MAXLINKSPEED"
#define CMD_SETROAMMODE		"SETROAMMODE"
#define CMD_AUTOSLEEP		"AUTOSLEEP" /* only for SDIO based chip */
#define CMD_SET_WPS_P2PIE	"SET_AP_WPS_P2P_IE"
#define CMD_SETBTCPARAMS	"SETBTCPARAMS"
#define CMD_GETBTCPARAMS	"GETBTCPARAMS"
#define CMD_MKEEP_ALIVE		"MKEEP_ALIVE" /* TODO */
u32 restrict_bw_20;
bool builtin_roam_disabled;

static int wl_android_get_link_speed(struct brcmf_if *ifp, char *command, int total_len)
{
	int link_speed;
	int bytes_written;
	int err;

	err = brcmf_fil_cmd_int_get(ifp, BRCMF_C_GET_RATE, &link_speed);
	if (err < 0) {
		brcmf_err("BRCMF_C_GET_RATE error (%d)\n", err);
		return err;
	}

	/* Convert Kbps to Android Mbps */
	link_speed = link_speed / 1000;
	bytes_written = snprintf(command, total_len, "LinkSpeed %d", link_speed);
	brcmf_dbg(INFO, "%s: command result is %s\n", __func__, command);
	return bytes_written;
}

static int wl_android_get_rssi(struct brcmf_if *ifp, char *command, int total_len)
{
	struct brcmf_scb_val_le scbval;
	int bytes_written;
	int rssi;
	int err;

	memset(&scbval, 0, sizeof(scbval));
	err = brcmf_fil_cmd_data_get(ifp, BRCMF_C_GET_RSSI, &scbval,
				     sizeof(scbval));
	if (err) {
		brcmf_err("BRCMF_C_GET_RSSI error (%d)\n", err);
		return err;
	}
	rssi = le32_to_cpu(scbval.val);

	bytes_written = snprintf(command, total_len, "rssi %d", rssi);
	brcmf_dbg(INFO, "%s: command result is %s\n", __func__, command);
	return bytes_written;
}

int
nv_android_private_cmd(struct brcmf_pub *drvr, struct net_device *ndev,
	char *command, u32 cmd_len, int *bytes_written)
{
	int ret = 1;
	struct brcmf_if *ifp = netdev_priv(ndev);
	struct wireless_dev *wdev = ndev->ieee80211_ptr;
	struct wiphy *wiphy = NULL;
	int skip = 0;
	int val;

	if (!bytes_written)
		return -EINVAL;
	if (!wdev)
		return -ENODEV;
	wiphy = wdev->wiphy;
	brcmf_err("command = %s received\n", command);

	if (strncmp(command, CMD_SET_IM_MODE,
			strlen(CMD_SET_IM_MODE)) == 0) {
		*bytes_written =
			nv_brcmf_android_set_im_mode(drvr, ndev, command,
						cmd_len);
	} else if (strncmp(command, CMD_SET_WPS_P2PIE,
		   strlen(CMD_SET_WPS_P2PIE)) == 0) {
		skip = strlen(CMD_SET_WPS_P2PIE) + 3;
		*bytes_written =
			brcmf_cfg80211_set_ap_wps_p2p_ie(ifp->vif, command + skip,
						(cmd_len - skip), *(command + skip - 2) - '0');
	} else if (strncmp(command, CMD_UPDATE_CHANNEL_LIST,
			strlen(CMD_UPDATE_CHANNEL_LIST)) == 0) {
		brcmf_setup_wiphybands(wiphy);
	} else if (strncmp(command, CMD_RESTRICT_BW_20, strlen(CMD_RESTRICT_BW_20)) == 0) {
		*bytes_written = -1;
		val = *(command + strlen(CMD_RESTRICT_BW_20) + 1) - '0';
		if (val == 0 || val == 1) {
			restrict_bw_20 = val;
			*bytes_written = 0;
		}
	} else if (strncmp(command, CMD_MAXLINKSPEED, strlen(CMD_MAXLINKSPEED)) == 0) {
		*bytes_written = brcmf_get_max_linkspeed(ndev, command, cmd_len);
	} else if (!builtin_roam_disabled && strncmp(command, CMD_SETROAMMODE, strlen(CMD_SETROAMMODE)) == 0) {
		 *bytes_written = nv_set_roam_mode(ndev, command, cmd_len);
	} else if (strncmp(command, CMD_SETBTCPARAMS, strlen(CMD_SETBTCPARAMS)) == 0) {
		*bytes_written = nv_btcoex_set_btcparams(ndev, command, cmd_len);
	} else if (strncmp(command, CMD_GETBTCPARAMS, strlen(CMD_GETBTCPARAMS)) == 0) {
		*bytes_written = nv_btcoex_get_btcparams(ndev, command, cmd_len);
	} else if (strncmp(command, CMD_MKEEP_ALIVE,
		strlen(CMD_MKEEP_ALIVE)) == 0) {
		brcmf_err("CMD_MKEEP_ALIVE not supported\n");
	} else if (strncmp(command, CMD_LINKSPEED, strlen(CMD_LINKSPEED)) == 0) {
		*bytes_written = wl_android_get_link_speed(ifp, command, cmd_len);
	} else if (strncmp(command, CMD_RSSI, strlen(CMD_RSSI)) == 0) {
		*bytes_written = wl_android_get_rssi(ifp, command, cmd_len);
	} else {
		return -EINVAL;
	}

	return ret;
}
