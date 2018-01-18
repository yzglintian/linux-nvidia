/*
 * t19x-nvlink-endpt.h:
 * This header contains the structures and APIs needed by the Tegra NVLINK
 * endpoint driver.
 *
 * Copyright (c) 2017-2018, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef T19X_NVLINK_ENDPT_H
#define T19X_NVLINK_ENDPT_H

#include <linux/io.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/of_graph.h>
#include <linux/slab.h>
#include <linux/of_address.h>
#include <linux/module.h>
#include <linux/platform/tegra/mc.h>
#include <linux/platform/tegra/mc-regs-t19x.h>
#include <linux/tegra_prod.h>
#include <linux/platform/tegra/tegra-nvlink.h>

#define NVLINK_DRV_NAME				"t19x-nvlink-endpt"
#define NVLINK_IP_VERSION			2 /* NVLINK VERSION 2.0 */
#define DEFAULT_LOOP_SLEEP_US			100
#define DEFAULT_LOOP_TIMEOUT_US			1000000
#define DEFAULT_IS_NEA				0

struct tegra_nvlink_device {
	/* base address of SYNC2X */
	void __iomem *nvlw_sync2x_base;
	/* base address of MSSNVLINK */
	void __iomem *mssnvlink_0_base;

#ifdef CONFIG_DEBUG_FS
	/* This is the debugfs directory for the Tegra endpoint driver */
	struct dentry *tegra_debugfs;
	struct dentry *tegra_debugfs_file;
#endif /* CONFIG_DEBUG_FS  */
	/* clocks */
	struct clk *clk_nvhs_pll0_mgmt;
	struct clk *clk_nvlink_sys;
	struct clk *clk_pllnvhs;
	struct clk *clk_txclk_ctrl;
	/* resets */
	struct reset_control *rst_nvhs_uphy_pm;
	struct reset_control *rst_nvhs_uphy;
	struct reset_control *rst_nvhs_uphy_pll0;
	struct reset_control *rst_nvhs_uphy_l0;
	struct reset_control *rst_nvhs_uphy_l1;
	struct reset_control *rst_nvhs_uphy_l2;
	struct reset_control *rst_nvhs_uphy_l3;
	struct reset_control *rst_nvhs_uphy_l4;
	struct reset_control *rst_nvhs_uphy_l5;
	struct reset_control *rst_nvhs_uphy_l6;
	struct reset_control *rst_nvhs_uphy_l7;
	/* Powergate id */
	int pgid_nvl;
	struct tegra_prod *prod_list;
	bool is_nea;
};

extern const struct single_lane_params entry_100us_sl_params;
extern const struct file_operations t19x_nvlink_endpt_ops;

u32 nvlw_tioctrl_readl(struct nvlink_device *ndev, u32 reg);
void nvlw_tioctrl_writel(struct nvlink_device *ndev, u32 reg, u32 val);

u32 nvlw_nvlipt_readl(struct nvlink_device *ndev, u32 reg);
void nvlw_nvlipt_writel(struct nvlink_device *ndev, u32 reg, u32 val);

u32 nvlw_minion_readl(struct nvlink_device *ndev, u32 reg);
void nvlw_minion_writel(struct nvlink_device *ndev, u32 reg, u32 val);

u32 nvlw_nvl_readl(struct nvlink_device *ndev, u32 reg);
void nvlw_nvl_writel(struct nvlink_device *ndev, u32 reg, u32 val);

u32 nvlw_sync2x_readl(struct nvlink_device *ndev, u32 reg);
void nvlw_sync2x_writel(struct nvlink_device *ndev, u32 reg, u32 val);

u32 nvlw_nvltlc_readl(struct nvlink_device *ndev, u32 reg);
void nvlw_nvltlc_writel(struct nvlink_device *ndev, u32 reg, u32 val);

int wait_for_reg_cond_nvlink(
			struct nvlink_device *ndev,
			u32 reg,
			u32 bit,
			int bit_set,
			char *bit_name,
			u32 (*reg_readl)(struct nvlink_device *, u32),
			u32 *reg_val);

void minion_dump_pc_trace(struct nvlink_device *ndev);
void minion_dump_registers(struct nvlink_device *ndev);
int minion_boot(struct nvlink_device *ndev);
int init_nvhs_phy(struct nvlink_device *ndev);
int minion_send_cmd(struct nvlink_device *ndev,
				u32 cmd,
				u32 scratch0_val);
void nvlink_enable_AN0_packets(struct nvlink_device *ndev);
void nvlink_config_common_intr(struct nvlink_device *ndev);
void nvlink_enable_link_interrupts(struct nvlink_device *ndev);
void minion_service_falcon_intr(struct nvlink_device *ndev);
irqreturn_t t19x_nvlink_endpt_isr(int irq, void *dev_id);

void init_single_lane_params(struct nvlink_device *ndev);
u32 t19x_nvlink_get_link_state(struct nvlink_device *ndev);
u32 t19x_nvlink_get_link_mode(struct nvlink_device *ndev);
int t19x_nvlink_set_link_mode(struct nvlink_device *ndev, u32 mode);
void t19x_nvlink_get_tx_sublink_state(struct nvlink_device *ndev,
				u32 *tx_sublink_state);
void t19x_nvlink_get_rx_sublink_state(struct nvlink_device *ndev,
				u32 *rx_sublink_state);
u32 t19x_nvlink_get_sublink_mode(struct nvlink_device *ndev,
				bool is_rx_sublink);
int t19x_nvlink_set_sublink_mode(struct nvlink_device *ndev, bool is_rx_sublink,
				u32 mode);
int t19x_nvlink_set_link_mode(struct nvlink_device *ndev, u32 mode);
void nvlink_enable_AN0_packets(struct nvlink_device *ndev);
int nvlink_retrain_link(struct nvlink_device *ndev, bool from_off);

#ifdef CONFIG_DEBUG_FS
void t19x_nvlink_endpt_debugfs_init(struct nvlink_device *ndev);
void t19x_nvlink_endpt_debugfs_deinit(struct nvlink_device *ndev);
#else
static inline void t19x_nvlink_endpt_debugfs_init(struct nvlink_device *ndev) {}
static inline void t19x_nvlink_endpt_debugfs_deinit(
						struct nvlink_device *ndev) {}
#endif /* CONFIG_DEBUG_FS  */

#endif /* T19X_NVLINK_ENDPT_H */
