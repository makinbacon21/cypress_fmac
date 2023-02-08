/* SPDX-License-Identifier: ISC */
/* Copyright 2017, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All rights reserved.
 * This software, including source code, documentation and related
 * materials ("Software"), is owned by Cypress Semiconductor
 * Corporation or one of its subsidiaries ("Cypress") and is protected by
 * and subject to worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA"). If no EULA applies, Cypress hereby grants
 * you a personal, nonexclusive, non-transferable license to copy, modify,
 * and compile the Software source code solely for use in connection with
 * Cypress's integrated circuit products. Any reproduction, modification,
 * translation, compilation, or representation of this Software except as
 * specified above is prohibited without the express written permission of
 * Cypress.
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

extern struct brcmf_pub *g_drvr;

int brcmf_android_wifi_off(struct brcmf_pub *drvr, struct net_device *dev);
int brcmf_android_wifi_on(struct brcmf_pub *drvr, struct net_device *dev);
int brcmf_android_attach(struct device *dev, struct brcmf_mp_device *settings);
int brcmf_android_detach(struct brcmf_pub *drvr);
bool brcmf_android_is_attached(struct brcmf_pub *drvr);
bool brcmf_android_is_inited(struct brcmf_pub *drvr);
bool brcmf_android_wifi_is_on(struct brcmf_pub *drvr);
bool brcmf_android_in_reset(struct brcmf_pub *drvr);
void brcmf_android_set_reset(struct brcmf_pub *drvr, bool is_reset);
int brcmf_android_wake_lock(struct brcmf_pub *drvr);
int brcmf_android_wake_unlock(struct brcmf_pub *drvr);
int brcmf_android_wake_unlock_timeout(struct brcmf_pub *drvr);
void brcmf_android_wake_lock_waive(struct brcmf_pub *drvr, bool is_waive);
bool brcmf_android_wake_lock_is_waive(struct brcmf_pub *drvr);
int brcmf_handle_private_cmd(struct brcmf_pub *drvr, struct net_device *net,
			     char *command, u32 cmd_len);
int brcmf_android_reset_country(struct brcmf_pub *drvr);
int brcmf_android_set_extra_wiphy(struct wiphy *wiphy, struct brcmf_if *ifp);
void brcmf_android_restore_pktfilter(struct brcmf_pub *drvr);
void brcmf_android_set_wake_flag(struct brcmf_pub *drvr, int set);
int brcmf_android_get_wake_flag(struct brcmf_pub *drvr);
void brcmf_android_add_wake_event_cnts(struct brcmf_pub *drvr,
				       enum brcmf_fweh_event_code code);
void brcmf_android_add_wake_pkt_cnts(struct brcmf_pub *drvr,
				     struct sk_buff *skb);

struct brcmf_android_wifi_priv_cmd {
	char *buf;
	int used_len;
	int total_len;
};

#define DOT11_OUI_LEN		3	/* d11 OUI length */

#define RS_FAILED	(-1)
#define RS_OK		(0)
#define RS_INIT		(1)

struct brcmf_andr_wake_counts {
	u32 rcwake;
	u32 rc_event[BRCMF_E_LAST];
	u32 rxwake;
	u32 rx_ucast;
	u32 rx_mcast;
	u32 rx_bcast;
	u32 rx_arp;
	u32 rx_icmpv6;
	u32 rx_multi_ipv6;
	u32 rx_multi_ipv4;
	u32 rx_multi_other;
	u32 rx_icmpv6_ra;
	u32 rx_icmpv6_na;
	u32 rx_icmpv6_ns;
};

struct brcmf_andr_wake_bus_info {
	atomic_t pkt_wake;
};

struct brcmf_android {
	struct brcmf_pub *drvr;
	struct wiphy *wiphy;
	int reset_status;
	bool wifi_reset;
	bool wifi_on;
	bool init_done;
	bool deinit;
	char country[3];
	bool wakelock_waive;
	struct wakeup_source wakelock;
	struct wakeup_source rx_wakelock;
	spinlock_t wakelock_spinlock;	/* must be held before using wakelock */
	unsigned int wakelock_counter;
	unsigned int wakelock_waive_counter;
	u16 pkt_filter_list;
	void *fw_mem_dump;
	size_t fw_mem_dump_len;
	u8 enabled_ndev_num;
	struct brcmf_andr_wake_counts *wake_cnts;
	struct brcmf_andr_wake_bus_info *wake_info;
};

int brcmu_ether_atoe(const char *p,  char *ea);
