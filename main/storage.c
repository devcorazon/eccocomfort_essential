/*
 * storage.c
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#include <freertos/FreeRTOS.h>
#include "storage.h"

// Data on ram.
//static __attribute__((section(".noinit"))) struct application_data_s application_data;
static struct application_data_s application_data;


static void storage_init_noinit_data(void)
{
	memset(&application_data.runtime_data, 0, sizeof(application_data.runtime_data));

	application_data.crc_noinit_data = crc((const uint8_t *) &application_data.noinit_data, sizeof(application_data.noinit_data));
}

static void storage_init_runtime_data(void)
{
	memset(&application_data.runtime_data, 0, sizeof(application_data.runtime_data));

	//application_data.runtime_data.temperature = TEMPERATURE_INVALID;
	//application_data.runtime_data.relative_humidity = RELATIVE_HUMIDITY_INVALID;
}

int storage_init(void)
{
	if (application_data.crc_noinit_data != crc((const uint8_t *) &application_data.noinit_data, sizeof(application_data.noinit_data))) {
		storage_init_noinit_data();
	}

	storage_init_runtime_data();

	/*TODO*/
	// load by NVS

	return 0;
}

int storage_set_default(void)
{
	memset(&application_data, 0, sizeof(application_data));

	storage_init_noinit_data();
	storage_init_runtime_data();

	return 0;
}

/// runtime data

int16_t get_temperature(void)
{
	return application_data.runtime_data.temperature;
}

void set_temperature(int16_t temperature)
{
	application_data.runtime_data.temperature = temperature;
}

uint16_t get_relative_humidity(void)
{
	return application_data.runtime_data.relative_humidity;
}

void set_relative_humidity(int16_t relative_humidity)
{
	application_data.runtime_data.relative_humidity = relative_humidity;
}

int32_t get_voc(void)
{
	return application_data.runtime_data.voc;
}

void set_voc(int32_t voc)
{
	application_data.runtime_data.voc = voc;
}

int16_t get_lux(void)
{
	return application_data.runtime_data.lux;
}

void set_lux(int16_t lux)
{
	application_data.runtime_data.lux = lux;
}

int16_t get_ntc_temperature(void)
{
	return application_data.runtime_data.ntc_temperature;
}

void set_ntc_temperature(int16_t ntc_temperature)
{
	application_data.runtime_data.ntc_temperature = ntc_temperature;
}

uint8_t get_mode_state(void)
{
	return application_data.runtime_data.mode_state;
}

void set_mode_state(uint8_t mode_state)
{
	application_data.runtime_data.mode_state = mode_state;
}

uint8_t get_speed_state(void)
{
	return application_data.runtime_data.speed_state;
}

uint32_t get_serial_number(void)
{
	return application_data.runtime_data.serial_number;
}

void set_serial_number(uint8_t serial_number_byte[4])
{
	application_data.runtime_data.serial_number = ((uint32_t)serial_number_byte[0]) << 24 | ((uint32_t)serial_number_byte[1]) << 16 | ((uint32_t)serial_number_byte[2]) << 8 | ((uint32_t)serial_number_byte[3]);
}

uint16_t get_fw_version(void)
{
	return application_data.runtime_data.fw_version_v_ctrl;
}

void set_fw_version(uint8_t fw_version_byte[2])
{
	application_data.runtime_data.fw_version_v_ctrl = ((uint16_t)fw_version_byte[0]) << 12 | ((uint16_t)fw_version_byte[1]) << 6 | ((uint16_t)fw_version_byte[2]);
}


void set_speed_state(uint8_t speed_state)
{
	application_data.runtime_data.speed_state = speed_state;
}

/// configuration settings
uint8_t get_mode_set(void)
{
	return application_data.configuration_settings.mode_set;
}

void set_mode_set(uint8_t mode_set)
{
	application_data.configuration_settings.mode_set = mode_set;

	/*TODO*/
	// save on NVS
}

uint8_t get_speed_set(void)
{
	return application_data.configuration_settings.speed_set;
}

void set_speed_set(uint8_t speed_set)
{
	application_data.configuration_settings.speed_set = speed_set;

	/*TODO*/
	// save on NVS
}
