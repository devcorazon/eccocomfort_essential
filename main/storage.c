/*
 * storage.c
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#include <freertos/FreeRTOS.h>
#include "storage.h"
#include "nvs_flash.h"
#include <esp_log.h>

nvs_handle_t my_handle;

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

esp_err_t storage_init(void)
{
    uint8_t mode_set;
    uint8_t speed_set;

    if (application_data.crc_noinit_data != crc((const uint8_t *) &application_data.noinit_data, sizeof(application_data.noinit_data)))
    {
        storage_init_noinit_data();
    }

    storage_init_runtime_data();
    // read mode_set from NVS
    esp_err_t ret = nvs_read("mode_set", &mode_set);
    if (ret != ESP_OK)
    {
        return ret; // handle error
    }
    // read speed_set from NVS
    ret = nvs_read("speed_set", &speed_set);
    if (ret != ESP_OK)
    {
        return ret;  // handle error
    }

    set_mode_set(mode_set);
    set_speed_set(speed_set);

    return ESP_OK;
}

esp_err_t storage_set_default(void)
{
	memset(&application_data, 0, sizeof(application_data));

	storage_init_noinit_data();
	storage_init_runtime_data();

	return ESP_OK;
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

void set_speed_state(uint8_t speed_state)
{
	application_data.runtime_data.speed_state = speed_state;
}

uint8_t get_direction_state(void)
{
	return application_data.runtime_data.direction_state;
}
void set_direction_state(uint8_t direction_state)
{
	application_data.runtime_data.direction_state= direction_state;
}

uint32_t get_serial_number(void)
{
	return application_data.runtime_data.serial_number;
}

void set_serial_number(uint32_t serial_number)
{
	application_data.runtime_data.serial_number = serial_number;
}

uint16_t get_fw_version(void)
{
	return application_data.runtime_data.fw_version_v_ctrl;
}

void set_fw_version(uint16_t fw_version)
{
	application_data.runtime_data.fw_version_v_ctrl = fw_version;
}


/// configuration settings
uint8_t get_mode_set(void)
{
	return application_data.configuration_settings.mode_set;
}

esp_err_t set_mode_set(uint8_t mode_set)
{
	application_data.configuration_settings.mode_set = mode_set;

	// Call the save function with key as "speed_set"
	esp_err_t err = nvs_save("mode_set", mode_set);

    return err;
}

uint8_t get_speed_set(void)
{
	return application_data.configuration_settings.speed_set;
}

esp_err_t set_speed_set(uint8_t speed_set)
{
	application_data.configuration_settings.speed_set = speed_set;

	// Call the save function with key as "speed_set"
	esp_err_t err = nvs_save("speed_set", speed_set);

    return err;
}

esp_err_t nvs_read(char *key, uint8_t *data)
{
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return err;
    }
    else
    {
        err = nvs_get_u8(my_handle, key, data);
        if (err != ESP_OK)
        {
            if (err == ESP_ERR_NVS_NOT_FOUND)
            {
                ESP_LOGI("NVS", "The value associated with the key %s has not been set yet!", key);
            }
            else
            {
                ESP_LOGE("NVS", "Error (%s) reading!", esp_err_to_name(err));
            }
            nvs_close(my_handle);
            return err;
        }
    }
    nvs_close(my_handle);
    return ESP_OK;
}

esp_err_t nvs_save(char *key, uint8_t data)
{
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
    	return err;  // handle error
    }
    err = nvs_set_u8(my_handle, key, data);
    if (err != ESP_OK)
    {
    	nvs_close(my_handle);
    	return err;  // handle error
    }
    err = nvs_commit(my_handle);
    nvs_close(my_handle);

    return err;
}

esp_err_t nvs_erase(void)
{
    // Erase all NVS
    esp_err_t err = nvs_flash_erase();

    return err;
}
