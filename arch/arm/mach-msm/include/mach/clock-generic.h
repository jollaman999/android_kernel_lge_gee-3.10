/*
 * Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __MACH_CLOCK_GENERIC_H
#define __MACH_CLOCK_GENERIC_H

#include <mach/clk-provider.h>

/* ==================== Mux clock ==================== */

struct clk_src {
	struct clk *src;
	int sel;
};

struct mux_clk;

struct clk_mux_ops {
	int (*set_mux_sel)(struct mux_clk *clk, int sel);
	int (*get_mux_sel)(struct mux_clk *clk);

	/* Optional */
	bool (*is_enabled)(struct mux_clk *clk);
	int (*enable)(struct mux_clk *clk);
	void (*disable)(struct mux_clk *clk);
};

#define MUX_SRC_LIST(...) \
	.parents = (struct clk_src[]){__VA_ARGS__}, \
	.num_parents = ARRAY_SIZE(((struct clk_src[]){__VA_ARGS__}))

struct mux_clk {
	/* Parents in decreasing order of preference for obtaining rates. */
	struct clk_src	*parents;
	int		num_parents;
	struct clk	*safe_parent;
	int		safe_sel;
	struct clk_mux_ops *ops;

	/* Fields not used by helper function. */
	void *const __iomem *base;
	u32		offset;
	u32		mask;
	u32		shift;
	u32		en_mask;
	void		*priv;

	struct clk	c;
};

static inline struct mux_clk *to_mux_clk(struct clk *c)
{
	return container_of(c, struct mux_clk, c);
}

extern struct clk_ops clk_ops_gen_mux;

#endif