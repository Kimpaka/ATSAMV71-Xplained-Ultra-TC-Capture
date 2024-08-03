/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_usart_base.h>

struct usart_sync_descriptor TARGET_IO;

void TIMER_0_PORT_init(void)
{

	gpio_set_pin_function(LED1, MUX_PC9B_TC2_TIOB7);
}

void TIMER_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_TC2_CHANNEL0);
	_pmc_enable_periph_clock(ID_TC2_CHANNEL1);
	_pmc_enable_periph_clock(ID_TC2_CHANNEL2);
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(PA21, MUX_PA21A_USART1_RXD1);

	gpio_set_pin_function(PB4, MUX_PB4D_USART1_TXD1);
}

void TARGET_IO_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_USART1);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	TARGET_IO_PORT_init();
	usart_sync_init(&TARGET_IO, USART1, _usart_get_usart_sync());
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOA);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PA23 */

	gpio_set_pin_level(LED0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED0, GPIO_PIN_FUNCTION_OFF);

	TIMER_0_CLOCK_init();
	TIMER_0_PORT_init();
	TIMER_0_init();

	delay_driver_init();

	TARGET_IO_init();
}
