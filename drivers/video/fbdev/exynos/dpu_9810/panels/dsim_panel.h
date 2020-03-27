/*
 * Copyright (c) Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __DSIM_PANEL__
#define __DSIM_PANEL__


extern unsigned int lcdtype;

extern struct dsim_lcd_driver *mipi_lcd_driver;

#if IS_ENABLED(CONFIG_EXYNOS_DECON_LCD_ANA6705_A51)
extern struct dsim_lcd_driver ana6705_mipi_lcd_driver;
#elif IS_ENABLED(CONFIG_EXYNOS_DECON_LCD_S6E3FA9_ANOTE)
extern struct dsim_lcd_driver s6e3fa9_mipi_lcd_driver;
#else
#error "Unknown CONFIG_EXYNOS_DECON_LCD config"
#endif

extern int replace_lcd_driver(struct dsim_lcd_driver *drv);

#endif

