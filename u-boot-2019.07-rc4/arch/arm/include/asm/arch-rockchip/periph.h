/* SPDX-License-Identifier: GPL-2.0 */
/*
 * (C) Copyright 2015 Google, Inc
 */

#ifndef _ASM_ARCH_PERIPH_H
#define _ASM_ARCH_PERIPH_H

/*
 * The peripherals supported by the hardware. This is used to specify clocks
 * and pinctrl settings. Some SoCs will not support all of these, but it
 * provides a common reference for common drivers to use.
 */
enum periph_id {
	PERIPH_ID_PWM0,
	PERIPH_ID_PWM1,
	PERIPH_ID_PWM2,
	PERIPH_ID_PWM3,
	PERIPH_ID_PWM4,
	PERIPH_ID_I2C0,
	PERIPH_ID_I2C1,
	PERIPH_ID_I2C2,
	PERIPH_ID_I2C3,
	PERIPH_ID_I2C4,
	PERIPH_ID_I2C5,
	PERIPH_ID_I2C6,
	PERIPH_ID_I2C7,
	PERIPH_ID_I2C8,
	PERIPH_ID_SPI0,
	PERIPH_ID_SPI1,
	PERIPH_ID_SPI2,
	PERIPH_ID_SPI3,
	PERIPH_ID_SPI4,
	PERIPH_ID_SPI5,
	PERIPH_ID_UART0,
	PERIPH_ID_UART1,
	PERIPH_ID_UART2,
	PERIPH_ID_UART3,
	PERIPH_ID_UART4,
	PERIPH_ID_LCDC0,
	PERIPH_ID_LCDC1,
	PERIPH_ID_SDMMC0,
	PERIPH_ID_SDMMC1,
	PERIPH_ID_SDMMC2,
	PERIPH_ID_HDMI,
	PERIPH_ID_GMAC,
	PERIPH_ID_SFC,
	PERIPH_ID_I2S,

	PERIPH_ID_COUNT,

	/* Some aliases */
	PERIPH_ID_EMMC = PERIPH_ID_SDMMC0,
	PERIPH_ID_SDCARD = PERIPH_ID_SDMMC1,
	PERIPH_ID_UART_BT = PERIPH_ID_UART0,
	PERIPH_ID_UART_BB = PERIPH_ID_UART1,
	PERIPH_ID_UART_DBG = PERIPH_ID_UART2,
	PERIPH_ID_UART_GPS = PERIPH_ID_UART3,
	PERIPH_ID_UART_EXP = PERIPH_ID_UART4,
};

#endif
