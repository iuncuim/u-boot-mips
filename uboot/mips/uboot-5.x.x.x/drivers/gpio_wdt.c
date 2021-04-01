/*
 * watchdog.c - gpio controlled watchdog toggle
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <watchdog.h>
#include <gpio.h>

#ifdef CONFIG_GPIO_WDT
static int watchdog_toggle=1;
static int count_off=0;

void hw_watchdog_reset(void)
{
	if (watchdog_toggle) {
		
		mtk_gpio_toggle(WDGRES_PIN);
		count_off =0;
	}else{
		if (count_off<10)
			count_off++;
		if(count_off == 5)
			mtk_gpio_toggle(WDGRES_PIN);
		if(count_off == 10)
			mtk_set_gpio_pin(WDGRES_PIN, 0);
	}
}

void hw_watchdog_init(void)
{
	hw_watchdog_reset();
}

int do_wdog_toggle(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2)
		goto usage;

	if (strncmp(argv[1], "on", 2) == 0)
		watchdog_toggle = 1;
	else if (strncmp(argv[1], "off", 3) == 0)
		watchdog_toggle = 0;
	else
		goto usage;

	return 0;
usage:
	printf("Usage: wdogtoggle %s\n", cmdtp->usage);
	return 1;
}

U_BOOT_CMD(
	wdogtoggle, 2, 2, do_wdog_toggle,
	"toggle GPIO pin to service watchdog",
	"[on/off] - Switch watchdog toggling via GPIO pin on/off"
);

#endif
