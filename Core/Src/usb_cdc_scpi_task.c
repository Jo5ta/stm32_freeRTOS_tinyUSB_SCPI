/*
 * tinyUSB_task.c
 *
 *  Created on: Feb 23, 2023
 *      Author: jo5ta
 */

#include "tusb_config.h"
#include "tusb.h"
#include "scpi_commands.h"

#define SCPI_INPUT_BUFFER_LENGTH 256
#define SCPI_ERROR_QUEUE_SIZE 17
char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH] = {};
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE] = {};
scpi_t scpi_context = {};

size_t SCPI_Write(scpi_t * context, const char * data, size_t len){
	return tud_cdc_n_write(0, data, len);
}
int SCPI_Error(scpi_t * context, int_fast16_t err){
	return 0;
}
scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val){
	return SCPI_RES_OK;
}
scpi_result_t SCPI_Reset(scpi_t * context){
	tud_cdc_n_write_clear(0);
	tud_cdc_n_read_flush(0);
	return SCPI_RES_OK;
}
scpi_result_t SCPI_Flush(scpi_t * context){
	return tud_cdc_n_write_flush(0) == 0? SCPI_RES_OK:SCPI_RES_ERR;
}
scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};

void usb_cdc_scpi_task(void *argument) {
    SCPI_Init(&scpi_context,
            scpi_commands,
            &scpi_interface,
            scpi_units_def,
            SCPI_IDN1, SCPI_IDN2, SCPI_IDN3, SCPI_IDN4,
            scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
            scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);

	for (;;) {
		if (tud_cdc_n_available(0)) {
			char buf[64];
			const uint32_t count = tud_cdc_n_read(0, buf, sizeof(buf));
			SCPI_Input(&scpi_context, buf, count);
		}
		taskYIELD();
	}
}
