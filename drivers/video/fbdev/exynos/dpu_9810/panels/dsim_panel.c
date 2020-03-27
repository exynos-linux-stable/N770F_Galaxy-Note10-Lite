/*
 * Copyright (c) Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/lcd.h>
#include "../dsim.h"

#include "dsim_panel.h"

#if IS_ENABLED(CONFIG_EXYNOS_DECON_LCD_ANA6705_A51)
struct dsim_lcd_driver *mipi_lcd_driver = &ana6705_mipi_lcd_driver;
#elif IS_ENABLED(CONFIG_EXYNOS_DECON_LCD_S6E3FA9_ANOTE)
struct dsim_lcd_driver *mipi_lcd_driver = &s6e3fa9_mipi_lcd_driver;
#else
#error "Unknown CONFIG_EXYNOS_DECON_LCD config"
#endif

int replace_lcd_driver(struct dsim_lcd_driver *drv)
{
	struct device_node *node;
	int count = 0;
	char *dts_name = "lcd_info";

	node = of_find_node_with_property(NULL, dts_name);
	if (!node) {
		dsim_info("%s: of_find_node_with_property\n", __func__);
		goto exit;
	}

	count = of_count_phandle_with_args(node, dts_name, NULL);
	if (!count) {
		dsim_info("%s: of_count_phandle_with_args\n", __func__);
		goto exit;
	}

	node = of_parse_phandle(node, dts_name, 0);
	if (!node) {
		dsim_info("%s: of_parse_phandle\n", __func__);
		goto exit;
	}

	if (count != 1) {
		dsim_info("%s: we need only one phandle in lcd_info\n", __func__);
		goto exit;
	}

	if (IS_ERR_OR_NULL(drv) || IS_ERR_OR_NULL(drv->name)) {
		dsim_info("%s: we need lcd_drv name to compare with device tree name(%s)\n", __func__, node->name);
		goto exit;
	}

	if (strstarts(node->name, drv->name)) {
		mipi_lcd_driver = drv;
		dsim_info("%s: %s is registered\n", __func__, mipi_lcd_driver->name);
	} else
		dsim_info("%s: %s is not with prefix: %s\n", __func__, node->name, drv->name);

exit:
	return 0;
}


unsigned int lcdtype;
EXPORT_SYMBOL(lcdtype);

static int __init get_lcd_type(char *arg)
{
	get_option(&arg, &lcdtype);

	dsim_info("%s: lcdtype: %6X\n", __func__, lcdtype);

	return 0;
}
early_param("lcdtype", get_lcd_type);

static int boot_panel_id = 0;

int get_lcd_info(char *arg)
{
	if (!arg) {
		pr_err("%s invalid arg\n", __func__);
		return -EINVAL;
	}

	if (!strncmp(arg, "connected", 9))
		return (boot_panel_id < 0) ? 0 : 1;
	else if (!strncmp(arg, "id", 2))
		return (boot_panel_id < 0) ? 0 : boot_panel_id;
	else if (!strncmp(arg, "window_color", 12))
		return (boot_panel_id < 0) ? 0 : ((boot_panel_id & 0x0F0000) >> 16);
	else
		return -EINVAL;
}

EXPORT_SYMBOL(get_lcd_info);

static int __init get_boot_panel_id(char *arg)
{
	get_option(&arg, &boot_panel_id);
	pr_info("PANEL:INFO:%s:boot_panel_id : 0x%x\n",
			__func__, boot_panel_id);

	return 0;
}

early_param("lcdtype", get_boot_panel_id);

