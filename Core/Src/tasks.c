/*
 * tinyUSB_task.c
 *
 *  Created on: Feb 23, 2023
 *      Author: jo5ta
 */
#include "main.h"

void tinyUSB_task(void *argument) {
	tud_init(BOARD_TUD_RHPORT);
	for (;;) {
		tud_task();
		tud_cdc_write_flush();
		taskYIELD();
	}
}
