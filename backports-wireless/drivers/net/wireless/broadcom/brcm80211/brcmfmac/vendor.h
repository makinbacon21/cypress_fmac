// SPDX-License-Identifier: ISC
/*
 * Copyright (c) 2014 Broadcom Corporation
 * Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _vendor_h_
#define _vendor_h_

#define BROADCOM_OUI	0x001018
#if defined(CPTCFG_BRCMFMAC_ANDROID)
#define GOOGLE_OUI	0x001A11
#endif /* defined(CPTCFG_BRCMFMAC_ANDROID) */

enum brcmf_vndr_cmds {
	BRCMF_VNDR_CMDS_UNSPEC,
	BRCMF_VNDR_CMDS_DCMD,
	BRCMF_VNDR_CMDS_LAST
};

enum brcmf_vndr_evts {
	BRCMF_VNDR_EVTS_PHY_TEMP,
	BRCMF_VNDR_EVTS_LAST
};

/**
 * enum brcmf_nlattrs - nl80211 message attributes
 *
 * @BRCMF_NLATTR_LEN: message body length
 * @BRCMF_NLATTR_DATA: message body
 */
enum brcmf_nlattrs {
	BRCMF_NLATTR_UNSPEC,

	BRCMF_NLATTR_LEN,
	BRCMF_NLATTR_DATA,
	BRCMF_NLATTR_VERS,
	BRCMF_NLATTR_PHY_TEMP,
	BRCMF_NLATTR_PHY_TEMPDELTA,

	__BRCMF_NLATTR_AFTER_LAST,
	BRCMF_NLATTR_MAX = __BRCMF_NLATTR_AFTER_LAST - 1
};

/* structure of event sent up by firmware: is this the right place for it? */
struct brcmf_phy_temp_evt {
	__le32 version;
	__le32 temp;
	__le32 tempdelta;
} __packed;

/**
 * struct brcmf_vndr_dcmd_hdr - message header for cfg80211 vendor command dcmd
 *				support
 *
 * @cmd: common dongle cmd definition
 * @len: length of expecting return buffer
 * @offset: offset of data buffer
 * @set: get or set request(optional)
 * @magic: magic number for verification
 */
struct brcmf_vndr_dcmd_hdr {
	uint cmd;
	int len;
	uint offset;
	uint set;
	uint magic;
};

enum brcmf_vendor_event {
	BRCM_VENDOR_EVENT_UNSPEC,
	BRCM_VENDOR_EVENT_PRIV_STR,
	GOOGLE_GSCAN_SIGNIFICANT_EVENT,
	GOOGLE_GSCAN_GEOFENCE_FOUND_EVENT,
	GOOGLE_GSCAN_BATCH_SCAN_EVENT,
	GOOGLE_SCAN_FULL_RESULTS_EVENT,
	GOOGLE_RTT_COMPLETE_EVENT,
	GOOGLE_SCAN_COMPLETE_EVENT,
	GOOGLE_GSCAN_GEOFENCE_LOST_EVENT,
	GOOGLE_SCAN_EPNO_EVENT,
	GOOGLE_DEBUG_RING_EVENT,
	GOOGLE_FW_DUMP_EVENT,
	GOOGLE_PNO_HOTSPOT_FOUND_EVENT,
	GOOGLE_RSSI_MONITOR_EVENT,
	GOOGLE_MKEEP_ALIVE_EVENT,
	BRCM_VENDOR_EVENT_IDSUP_STATUS,
	BRCM_VENDOR_EVENT_DRIVER_HANG,
	BRCMF_VNDR_EVENT_LAST
};

/**
 * struct brcmf_pfn_macaddr_cfg - message header for PFN MAC address config
 *
 * @ver: version of configuration structure
 * @flags: PFN settings
 * @macaddr: MAC address for PFN
 */
struct brcmf_pfn_macaddr_cfg {
	u8 ver;
	u8 flags;
	struct ether_addr macaddr;
};

#define BRCMF_PFN_MACADDR_CFG_VER			1
#define BRCMF_PFN_MAC_OUI_ONLY_MASK			0x1
#define BRCMF_PFN_SET_MAC_UNASSOC_MASK		0x2
#define BRCMF_PFN_RESTRICT_LA_MAC_MASK		0x4
#define BRCMF_PFN_MACADDR_FLAG_MASK			0x7

extern const struct wiphy_vendor_command brcmf_vendor_cmds[];
extern const struct nl80211_vendor_cmd_info brcmf_vendor_events[];
s32 brcmf_wiphy_phy_temp_evt_handler(struct brcmf_if *ifp,
				     const struct brcmf_event_msg *e,
				     void *data);
#if defined(CPTCFG_BRCMFMAC_ANDROID)
void brcmf_set_vndr_cmd(struct wiphy *wiphy);
int brcmf_cfg80211_vndr_send_async_event(struct wiphy *wiphy,
	struct net_device *dev, int event_id, const void  *data, int len);

enum {
	/* don't use 0 as a valid subcommand */
	VENDOR_NL80211_SUBCMD_UNSPECIFIED,

	/* define all vendor startup commands between 0x0 and 0x0FFF */
	VENDOR_NL80211_SUBCMD_RANGE_START = 0x0001,
	VENDOR_NL80211_SUBCMD_RANGE_END   = 0x0FFF,

	/* define all GScan related commands between 0x1000 and 0x10FF */
	ANDROID_NL80211_SUBCMD_GSCAN_RANGE_START = 0x1000,
	ANDROID_NL80211_SUBCMD_GSCAN_RANGE_END   = 0x10FF,

	/**
	 * define all NearbyDiscovery related commands
	 * between 0x1100 and 0x11FF
	 */
	ANDROID_NL80211_SUBCMD_NBD_RANGE_START = 0x1100,
	ANDROID_NL80211_SUBCMD_NBD_RANGE_END   = 0x11FF,

	/* define all RTT related commands between 0x1100 and 0x11FF */
	ANDROID_NL80211_SUBCMD_RTT_RANGE_START = 0x1100,
	ANDROID_NL80211_SUBCMD_RTT_RANGE_END   = 0x11FF,

	ANDROID_NL80211_SUBCMD_LSTATS_RANGE_START = 0x1200,
	ANDROID_NL80211_SUBCMD_LSTATS_RANGE_END   = 0x12FF,

	/* define all Logger related commands between 0x1400 and 0x14FF */
	ANDROID_NL80211_SUBCMD_DEBUG_RANGE_START = 0x1400,
	ANDROID_NL80211_SUBCMD_DEBUG_RANGE_END   = 0x14FF,

	/* define all wifi offload related commands between 0x1600 and 0x16FF */
	ANDROID_NL80211_SUBCMD_WIFI_OFFLOAD_RANGE_START = 0x1600,
	ANDROID_NL80211_SUBCMD_WIFI_OFFLOAD_RANGE_END   = 0x16FF,

	/* define all NAN related commands between 0x1700 and 0x17FF */
	ANDROID_NL80211_SUBCMD_NAN_RANGE_START = 0x1700,
	ANDROID_NL80211_SUBCMD_NAN_RANGE_END   = 0x17FF,

	/**
	 * define all Android Packet Filter related commands
	 * between 0x1800 and 0x18FF
	 */
	ANDROID_NL80211_SUBCMD_PKT_FILTER_RANGE_START = 0x1800,
	ANDROID_NL80211_SUBCMD_PKT_FILTER_RANGE_END   = 0x18FF,

	/* This is reserved for future usage */

};

enum andr_vendor_subcmd {
	GSCAN_SUBCMD_GET_CAPABILITIES =
		ANDROID_NL80211_SUBCMD_GSCAN_RANGE_START,
	GSCAN_SUBCMD_SET_CONFIG,
	GSCAN_SUBCMD_SET_SCAN_CONFIG,
	GSCAN_SUBCMD_ENABLE_GSCAN,
	GSCAN_SUBCMD_GET_SCAN_RESULTS,
	GSCAN_SUBCMD_SCAN_RESULTS,
	GSCAN_SUBCMD_SET_HOTLIST,
	GSCAN_SUBCMD_SET_SIGNIFICANT_CHANGE_CONFIG,
	GSCAN_SUBCMD_ENABLE_FULL_SCAN_RESULTS,
	GSCAN_SUBCMD_GET_CHANNEL_LIST,
	ANDR_WIFI_SUBCMD_GET_FEATURE_SET,
	ANDR_WIFI_SUBCMD_GET_FEATURE_SET_MATRIX,
	ANDR_WIFI_RANDOM_MAC_OUI,
	ANDR_WIFI_NODFS_CHANNELS,
	ANDR_WIFI_SET_COUNTRY,
	/* define all wifi calling related commands between 0x1600 and 0x16FF */
#ifdef CPTCFG_BRCMFMAC_NV_PRIV_CMD
	GSCAN_SUBCMD_SET_EPNO_SSID,
	WIFI_SUBCMD_SET_SSID_WHITELIST,
	WIFI_SUBCMD_SET_LAZY_ROAM_PARAMS,
	WIFI_SUBCMD_ENABLE_LAZY_ROAM,
	WIFI_SUBCMD_SET_BSSID_PREF,
	WIFI_SUBCMD_SET_BSSID_BLACKLIST,
	GSCAN_SUBCMD_ANQPO_CONFIG,
	WIFI_SUBCMD_SET_RSSI_MONITOR,
    WIFI_SUBCMD_SET_LATENCY_MODE = 0x101b,
	RTT_SUBCMD_SET_CONFIG = 0x1100,
	RTT_SUBCMD_CANCEL_CONFIG,
	RTT_SUBCMD_GETCAPABILITY,
	LSTATS_SUBCMD_GET_INFO = 0x1200,
	DEBUG_START_LOGGING = 0x1400,
	DEBUG_TRIGGER_MEM_DUMP,
	DEBUG_GET_MEM_DUMP,
	DEBUG_GET_VER,
	DEBUG_GET_RING_STATUS,
	DEBUG_GET_RING_DATA,
	DEBUG_GET_FEATURE,
	DEBUG_RESET_LOGGING,
	DEBUG_GET_WAKE_REASON_STATS = 0x140d,
	DEBUG_SET_HAL_PID = 0x1412,
#endif /* CPTCFG_BRCMFMAC_NV_PRIV_CMD */
	ANDR_OFFLOAD_SUBCMD_START_MKEEP_ALIVE = 0x1600,
	ANDR_OFFLOAD_SUBCMD_STOP_MKEEP_ALIVE,
	GSCAN_SUBCMD_SET_EPNO_SSID,
	ANDR_WIFI_SUBCMD_SET_SSID_WHITE_LIST,
	ANDR_WIFI_SUBCMD_SET_ROAM_PARAMS,
	ANDR_WIFI_SUBCMD_ENABLE_LAZY_ROAM,
	ANDR_WIFI_SUBCMD_SET_BSSID_PREF,
	ANDR_WIFI_SUBCMD_SET_BSSID_BLACKLIST,
	GSCAN_SUBCMD_ANQPO_CONFIG,
	ANDR_WIFI_SUBCMD_SET_RSSI_MONITOR,
	ANDR_WIFI_SUBCMD_CONFIG_ND_OFFLOAD,
    ANDR_WIFI_SUBCMD_TX_PWR_SCENARIO = 0x1900,

	DEBUG_START_LOGGING = ANDROID_NL80211_SUBCMD_DEBUG_RANGE_START,
	DEBUG_TRIGGER_MEM_DUMP,
	DEBUG_GET_MEM_DUMP,
	DEBUG_GET_VER,
	DEBUG_GET_RING_STATUS,
	DEBUG_GET_RING_DATA,
	DEBUG_GET_FEATURE,
	DEBUG_RESET_LOGGING,

	DEBUG_TRIGGER_DRIVER_MEM_DUMP,
	DEBUG_GET_DRIVER_MEM_DUMP,
	DEBUG_START_PKT_FATE_MONITORING,
	DEBUG_GET_TX_PKT_FATES,
	DEBUG_GET_RX_PKT_FATES,
	DEBUG_GET_WAKE_REASON_STATS,
	DEBUG_GET_FILE_DUMP_BUF,
	DEBUG_FILE_DUMP_DONE_IND,
	DEBUG_SET_HAL_START,
	DEBUG_SET_HAL_STOP,

	APF_SUBCMD_GET_CAPABILITIES =
		ANDROID_NL80211_SUBCMD_PKT_FILTER_RANGE_START,
	APF_SUBCMD_SET_FILTER,
	VENDOR_SUBCMD_MAX
};

enum gscan_attributes {
	GSCAN_ATTRIBUTE_NUM_BUCKETS = 10,
	GSCAN_ATTRIBUTE_BASE_PERIOD,
	GSCAN_ATTRIBUTE_BUCKETS_BAND,
	GSCAN_ATTRIBUTE_BUCKET_ID,
	GSCAN_ATTRIBUTE_BUCKET_PERIOD,
	GSCAN_ATTRIBUTE_BUCKET_NUM_CHANNELS,
	GSCAN_ATTRIBUTE_BUCKET_CHANNELS,
	GSCAN_ATTRIBUTE_NUM_AP_PER_SCAN,
	GSCAN_ATTRIBUTE_REPORT_THRESHOLD,
	GSCAN_ATTRIBUTE_NUM_SCANS_TO_CACHE,
	GSCAN_ATTRIBUTE_BAND = GSCAN_ATTRIBUTE_BUCKETS_BAND,

	GSCAN_ATTRIBUTE_ENABLE_FEATURE = 20,
	GSCAN_ATTRIBUTE_SCAN_RESULTS_COMPLETE,
	GSCAN_ATTRIBUTE_FLUSH_FEATURE,
	GSCAN_ATTRIBUTE_ENABLE_FULL_SCAN_RESULTS,
	GSCAN_ATTRIBUTE_REPORT_EVENTS,
	GSCAN_ATTRIBUTE_NUM_OF_RESULTS = 30,
	GSCAN_ATTRIBUTE_FLUSH_RESULTS,
	GSCAN_ATTRIBUTE_SCAN_RESULTS,
	GSCAN_ATTRIBUTE_SCAN_ID,
	GSCAN_ATTRIBUTE_SCAN_FLAGS,
	GSCAN_ATTRIBUTE_AP_FLAGS,
	GSCAN_ATTRIBUTE_NUM_CHANNELS,
	GSCAN_ATTRIBUTE_CHANNEL_LIST,
	GSCAN_ATTRIBUTE_CH_BUCKET_BITMASK
};

enum andr_wifi_attr {
	ANDR_WIFI_ATTRIBUTE_NUM_FEATURE_SET,
	ANDR_WIFI_ATTRIBUTE_FEATURE_SET,
	ANDR_WIFI_ATTRIBUTE_RANDOM_MAC_OUI,
	ANDR_WIFI_ATTRIBUTE_NODFS_SET,
	ANDR_WIFI_ATTRIBUTE_COUNTRY,
	ANDR_WIFI_ATTRIBUTE_ND_OFFLOAD_VALUE,
	ANDR_WIFI_ATTRIBUTE_TCPACK_SUP_VALUE
};

enum mkeep_alive_attributes {
	MKEEP_ALIVE_ATTRIBUTE_ID,
	MKEEP_ALIVE_ATTRIBUTE_IP_PKT,
	MKEEP_ALIVE_ATTRIBUTE_IP_PKT_LEN,
	MKEEP_ALIVE_ATTRIBUTE_SRC_MAC_ADDR,
	MKEEP_ALIVE_ATTRIBUTE_DST_MAC_ADDR,
	MKEEP_ALIVE_ATTRIBUTE_PERIOD_MSEC,
	MKEEP_ALIVE_ATTRIBUTE_ETHER_TYPE
};

enum debug_attributes {
	DEBUG_ATTRIBUTE_GET_DRIVER,
	DEBUG_ATTRIBUTE_GET_FW,
	DEBUG_ATTRIBUTE_RING_ID,
	DEBUG_ATTRIBUTE_RING_NAME,
	DEBUG_ATTRIBUTE_RING_FLAGS,
	DEBUG_ATTRIBUTE_LOG_LEVEL,
	DEBUG_ATTRIBUTE_LOG_TIME_INTVAL,
	DEBUG_ATTRIBUTE_LOG_MIN_DATA_SIZE,
	//DEBUG_ATTRIBUTE_DUMP_FILENAME,
	DEBUG_ATTRIBUTE_FW_DUMP_LEN,
	DEBUG_ATTRIBUTE_FW_DUMP_DATA,
	DEBUG_ATTRIBUTE_RING_DATA,
	DEBUG_ATTRIBUTE_RING_STATUS,
	DEBUG_ATTRIBUTE_RING_NUM,
	DEBUG_ATTRIBUTE_DRIVER_DUMP_LEN,
	DEBUG_ATTRIBUTE_DRIVER_DUMP_DATA,
	DEBUG_ATTRIBUTE_PKT_FATE_NUM,
	DEBUG_ATTRIBUTE_PKT_FATE_DATA
};

enum wake_stat_attributes {
	WAKE_STAT_ATTRIBUTE_TOTAL_CMD_EVENT,
	WAKE_STAT_ATTRIBUTE_CMD_EVENT_WAKE,
	WAKE_STAT_ATTRIBUTE_CMD_EVENT_COUNT,
	WAKE_STAT_ATTRIBUTE_CMD_EVENT_COUNT_USED,
	WAKE_STAT_ATTRIBUTE_TOTAL_DRIVER_FW,
	WAKE_STAT_ATTRIBUTE_DRIVER_FW_WAKE,
	WAKE_STAT_ATTRIBUTE_DRIVER_FW_COUNT,
	WAKE_STAT_ATTRIBUTE_DRIVER_FW_COUNT_USED,
	WAKE_STAT_ATTRIBUTE_TOTAL_RX_DATA_WAKE,
	WAKE_STAT_ATTRIBUTE_RX_UNICAST_COUNT,
	WAKE_STAT_ATTRIBUTE_RX_MULTICAST_COUNT,
	WAKE_STAT_ATTRIBUTE_RX_BROADCAST_COUNT,
	WAKE_STAT_ATTRIBUTE_RX_ICMP_PKT,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_PKT,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_RA,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_NA,
	WAKE_STAT_ATTRIBUTE_RX_ICMP6_NS,
	WAKE_STAT_ATTRIBUTE_IPV4_RX_MULTICAST_ADD_CNT,
	WAKE_STAT_ATTRIBUTE_IPV6_RX_MULTICAST_ADD_CNT,
	WAKE_STAT_ATTRIBUTE_OTHER_RX_MULTICAST_ADD_CNT,
	WAKE_STAT_ATTRIBUTE_LAST
};

enum apf_attributes {
	APF_ATTRIBUTE_VERSION,
	APF_ATTRIBUTE_MAX_LEN,
	APF_ATTRIBUTE_PROGRAM,
	APF_ATTRIBUTE_PROGRAM_LEN
};

enum {
	/* Feature set */
	/* Memory dump of FW */
	DBG_MEMORY_DUMP_SUPPORTED = (1 << (0)),
	/* PKT Status */
	DBG_PER_PACKET_TX_RX_STATUS_SUPPORTED = (1 << (1)),
	/* Connectivity Event */
	DBG_CONNECT_EVENT_SUPPORTED = (1 << (2)),
	/* POWER of Driver */
	DBG_POWER_EVENT_SUPOORTED = (1 << (3)),
	/* WAKE LOCK of Driver */
	DBG_WAKE_LOCK_SUPPORTED	= (1 << (4)),
	/* verbose log of FW */
	DBG_VERBOSE_LOG_SUPPORTED = (1 << (5)),
	/* monitor the health of FW */
	DBG_HEALTH_CHECK_SUPPORTED = (1 << (6)),
	/* dumps driver state */
	DBG_DRIVER_DUMP_SUPPORTED = (1 << (7)),
	/* tracks connection packets' fate */
	DBG_PACKET_FATE_SUPPORTED = (1 << (8)),
	/* NAN Events */
	DBG_NAN_EVENT_SUPPORTED = (1 << (9)),
};

#define GSCAN_BG_BAND_MASK	0x1
#define GSCAN_A_BAND_MASK	0x2
#define GSCAN_DFS_MASK		0x4
#define GSCAN_ABG_BAND_MASK	(GSCAN_A_BAND_MASK | GSCAN_BG_BAND_MASK)
#define GSCAN_BAND_MASK		(GSCAN_ABG_BAND_MASK | GSCAN_DFS_MASK)

/* Basic infrastructure mode */
#define WIFI_FEATURE_INFRA		0x0001
/* Support for 5 GHz Band */
#define WIFI_FEATURE_INFRA_5G		0x0002
/* Support for GAS/ANQP */
#define WIFI_FEATURE_HOTSPOT		0x0004
/* Wifi-Direct */
#define WIFI_FEATURE_P2P		0x0008
/* Soft AP */
#define WIFI_FEATURE_SOFT_AP		0x0010
/* Google-Scan APIs */
#define WIFI_FEATURE_GSCAN		0x0020
/* Neighbor Awareness Networking */
#define WIFI_FEATURE_NAN		0x0040
/* Device-to-device RTT */
#define WIFI_FEATURE_D2D_RTT		0x0080
/* Device-to-AP RTT */
#define WIFI_FEATURE_D2AP_RTT		0x0100
/* Batched Scan (legacy) */
#define WIFI_FEATURE_BATCH_SCAN		0x0200
/* Preferred network offload */
#define WIFI_FEATURE_PNO		0x0400
/* Support for two STAs */
#define WIFI_FEATURE_ADDITIONAL_STA	0x0800
/* Tunnel directed link setup */
#define WIFI_FEATURE_TDLS		0x1000
/* Support for TDLS off channel */
#define WIFI_FEATURE_TDLS_OFFCHANNEL	0x2000
/* Enhanced power reporting */
#define WIFI_FEATURE_EPR		0x4000
/* Support for AP STA Concurrency */
#define WIFI_FEATURE_AP_STA		0x8000
/* Support for Linkstats */
#define WIFI_FEATURE_LINKSTAT		0x10000
/* WiFi PNO enhanced */
#define WIFI_FEATURE_HAL_EPNO		0x40000
/* RSSI Monitor */
#define WIFI_FEATURE_RSSI_MONITOR	0x80000
/* ND offload configure */
#define WIFI_FEATURE_CONFIG_NDO		0x200000
/* Invalid Feature */
#define WIFI_FEATURE_INVALID		0xFFFFFFFF
#endif /* defined(CPTCFG_BRCMFMAC_ANDROID) */

#endif /* _vendor_h_ */
