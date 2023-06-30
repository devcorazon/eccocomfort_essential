/*
 * storage.c
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#include <freertos/FreeRTOS.h>

#include <string.h>

#include "storage.h"
#include "storage_internal.h"

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

/// configuration settings
uint8_t get_mode_set(void)
{
	return application_data.configuration_settings.mode_set;
}

int set_mode_set(uint8_t mode_set)
{
	application_data.configuration_settings.mode_set = mode_set;

	/*TODO*/
	// save on NVS

	return 0;
}
