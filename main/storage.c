/*
 * storage.c
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#include <freertos/FreeRTOS.h>
#include "storage.h"
#include <esp_log.h>
#include "main.h"

nvs_handle_t my_handle;
static bool handle_open = false; // Add this flag to check if handle is open


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

static void storage_init_configuration_settings(void)
{
	memset(&application_data.configuration_settings, 0, sizeof(application_data.configuration_settings));
}

esp_err_t storage_init(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Set Firmware version
    set_fw_version();

    // Getting serial number from eFuse BLK3
    ESP_ERROR_CHECK(efuse_init());

    if (application_data.crc_noinit_data != crc((const uint8_t *) &application_data.noinit_data, sizeof(application_data.noinit_data)))
    {
        storage_init_noinit_data();
    }

    storage_init_runtime_data();

    // Open NVS handle
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (ret == ESP_OK)
    {
       // read mode_set from NVS
       ret = nvs_read("mode_set", &application_data.configuration_settings.mode_set);

       // read speed_set from NVS
       ret = nvs_read("speed_set", &application_data.configuration_settings.speed_set);
    }
    else
    {
       ESP_LOGE("NVS", "Error (%s) opening NVS handle!", esp_err_to_name(ret));
       if ( ret == ESP_ERR_NVS_NOT_FOUND )
       {
    	//Set configuration settings to default value
         storage_init_configuration_settings();
       }
    }
    return ret;
}

esp_err_t storage_set_default(void)
{
	memset(&application_data, 0, sizeof(application_data));

	storage_init_noinit_data();
	storage_init_runtime_data();
	storage_init_configuration_settings();

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

void set_fw_version()
{
	uint8_t fw_version_byte[3];
    // Setting Firmware version
    fw_version_byte[0] = FW_VERSION_MAJOR;
    fw_version_byte[1] = FW_VERSION_MINOR;
    fw_version_byte[2] = FW_VERSION_PATCH;

	application_data.runtime_data.fw_version_v_ctrl = ((uint16_t)fw_version_byte[0]) << 12 | ((uint16_t)fw_version_byte[1]) << 6 | ((uint16_t)fw_version_byte[2]);;
}


/// configuration settings
uint8_t get_mode_set(void)
{
	return application_data.configuration_settings.mode_set;
}

esp_err_t set_mode_set(uint8_t mode_set)
{
	application_data.configuration_settings.mode_set = mode_set;

	// Call the save function with key as "mode_set"
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

esp_err_t nvs_save(char *key, uint8_t data)
{
    esp_err_t err = nvs_set_u8(my_handle, key, data);
    if (err != ESP_OK)
    {
        ESP_LOGE("NVS", "Error (%s) Writing!", esp_err_to_name(err));
        return err;
    }
    err = nvs_commit(my_handle);
    // Don't close the handle here
    return err;
}

esp_err_t nvs_read(char *key, uint8_t *data)
{
    esp_err_t err = nvs_get_u8(my_handle, key, data);

    if (err != ESP_OK)
    {
        if (err == ESP_ERR_NVS_NOT_FOUND)
        {
            ESP_LOGI("NVS","The value associated with the key %s has not been set yet!", key);
        }
        else
        {
            ESP_LOGE("NVS", "Error (%s) reading!", esp_err_to_name(err));
        }
    }
    return err;
}

esp_err_t nvs_erase(void)
{
    // Erase all NVS
    esp_err_t err = nvs_flash_erase();
    return err;
}

esp_err_t efuse_init()
{
    uint8_t serial_number_byte[4];
    uint32_t serial_number;
    size_t num_bits = 4 * 8;
    size_t start_bit = 28 * 8;

    if (esp_efuse_read_block(EFUSE_BLK3, &serial_number_byte, start_bit, num_bits) == ESP_OK)
    {
        // verify after burn efuse if BE or LE
        printf("Serial Number[byte]: %02x%02x%02x%02x\n", serial_number_byte[0], serial_number_byte[1], serial_number_byte[2], serial_number_byte[3]);
        serial_number =((uint32_t)serial_number_byte[0]) << 24 | ((uint32_t)serial_number_byte[1]) << 16 | ((uint32_t)serial_number_byte[2]) << 8 | ((uint32_t)serial_number_byte[3]);
        set_serial_number(serial_number);
        return ESP_OK;
    }
    else
    {
    	return ESP_FAIL;
    }
}
