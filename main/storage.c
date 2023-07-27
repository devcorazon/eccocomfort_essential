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
    	ret = nvs_read("mode_set", &application_data.configuration_settings.mode_set, DATA_TYPE_UINT8);

       // read speed_set from NVS
    	ret = nvs_read("speed_set", &application_data.configuration_settings.speed_set, DATA_TYPE_UINT8);
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
	return application_data.runtime_data.fw_version_v_ctrl = (FW_VERSION_MAJOR) << 12 | (FW_VERSION_MINOR) << 6 | (FW_VERSION_PATCH);
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

uint8_t get_device_state(void)
{
	return application_data.runtime_data.device_state;
}
void set_device_state(uint8_t device_state)
{
	application_data.runtime_data.device_state= device_state;
}

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

void set_relative_humidity(uint16_t relative_humidity)
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

//int16_t get_ntc_temperature(void)
//{
//	return application_data.runtime_data.ntc_temperature;
//}
//void set_ntc_temperature(int16_t ntc_temperature)
//{
//	application_data.runtime_data.ntc_temperature = ntc_temperature;
//}

/// configuration settings

uint8_t get_relative_humidity_set(void)
{
	return application_data.configuration_settings.relative_humidity_set;
}

esp_err_t set_relative_humidity_set(uint8_t relative_humidity_set)
{
	application_data.configuration_settings.relative_humidity_set = relative_humidity_set;

	// Call the save function with key as "relative_humidity_set"
	esp_err_t err = nvs_save("relative_humidity_set", &relative_humidity_set, DATA_TYPE_UINT8);

    return err;
}

uint8_t get_lux_set(void)
{
	return application_data.configuration_settings.lux_set;
}

esp_err_t set_lux_set(uint8_t lux_set)
{
	application_data.configuration_settings.lux_set = lux_set;

	// Call the save function with key as "lux_set"
	esp_err_t err = nvs_save("lux_set", &lux_set, DATA_TYPE_UINT8);

    return err;
}

uint8_t get_voc_set(void)
{
	return application_data.configuration_settings.voc_set;
}

esp_err_t set_voc_set(uint8_t voc_set)
{
	application_data.configuration_settings.voc_set = voc_set;

	// Call the save function with key as "voc_set"
	esp_err_t err = nvs_save("voc_set", &voc_set, DATA_TYPE_UINT8);

    return err;
}

uint8_t get_temperature_offset(void)
{
	return application_data.configuration_settings.temperature_offset;
}

esp_err_t set_temperature_offset(uint16_t temperature_offset)
{
	application_data.configuration_settings.temperature_offset = temperature_offset;

	// Call the save function with key as "temperature_offset"
	esp_err_t err = nvs_save("temperature_offset", &temperature_offset, DATA_TYPE_INT16);

    return err;
}

uint8_t get_relative_humidity_offset(void)
{
	return application_data.configuration_settings.relative_humidity_offset;
}

esp_err_t set_relative_humidity_offset(uint16_t relative_humidity_offset)
{
	application_data.configuration_settings.relative_humidity_offset = relative_humidity_offset;

	// Call the save function with key as "relative_humidity_offset"
	esp_err_t err = nvs_save("relative_humidity_offset", &relative_humidity_offset, DATA_TYPE_UINT16);

    return err;
}

uint8_t get_mode_set(void)
{
	return application_data.configuration_settings.mode_set;
}

esp_err_t set_mode_set(uint8_t mode_set)
{
	application_data.configuration_settings.mode_set = mode_set;

	// Call the save function with key as "mode_set"
	esp_err_t err = nvs_save("mode_set", &mode_set, DATA_TYPE_UINT8);

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
	esp_err_t err = nvs_save("speed_set", &speed_set, DATA_TYPE_UINT8);

    return err;
}

esp_err_t nvs_save(char *key, void *data, data_type_e type)
{
    esp_err_t err;

    switch(type) {
        case DATA_TYPE_UINT8:
            err = nvs_set_u8(my_handle, key, *(uint8_t*)data);
            break;
        case DATA_TYPE_INT8:
            err = nvs_set_i8(my_handle, key, *(int8_t*)data);
            break;
        case DATA_TYPE_UINT16:
            err = nvs_set_u16(my_handle, key, *(uint16_t*)data);
            break;
        case DATA_TYPE_INT16:
            err = nvs_set_i16(my_handle, key, *(int16_t*)data);
            break;
        case DATA_TYPE_UINT32:
            err = nvs_set_u32(my_handle, key, *(uint32_t*)data);
            break;
        case DATA_TYPE_INT32:
            err = nvs_set_i32(my_handle, key, *(int32_t*)data);
            break;
        case DATA_TYPE_UINT64:
            err = nvs_set_u64(my_handle, key, *(uint64_t*)data);
            break;
        case DATA_TYPE_INT64:
            err = nvs_set_i64(my_handle, key, *(int64_t*)data);
            break;
        case DATA_TYPE_STRING:
            err = nvs_set_str(my_handle, key, (char*)data);
            break;
        default:
            return ESP_FAIL;
    }

    if (err != ESP_OK)
    {
        ESP_LOGE("NVS", "Error (%s) Writing!", esp_err_to_name(err));
        return err;
    }

    err = nvs_commit(my_handle);
    return err;
}

esp_err_t nvs_read(char *key, void *data, data_type_e type)
{
    esp_err_t err;

    switch(type) {
        case DATA_TYPE_UINT8:
            err = nvs_get_u8(my_handle, key, (uint8_t*)data);
            break;
        case DATA_TYPE_INT8:
            err = nvs_get_i8(my_handle, key, (int8_t*)data);
            break;
        case DATA_TYPE_UINT16:
            err = nvs_get_u16(my_handle, key, (uint16_t*)data);
            break;
        case DATA_TYPE_INT16:
            err = nvs_get_i16(my_handle, key, (int16_t*)data);
            break;
        case DATA_TYPE_UINT32:
            err = nvs_get_u32(my_handle, key, (uint32_t*)data);
            break;
        case DATA_TYPE_INT32:
            err = nvs_get_i32(my_handle, key, (int32_t*)data);
            break;
        case DATA_TYPE_UINT64:
            err = nvs_get_u64(my_handle, key, (uint64_t*)data);
            break;
        case DATA_TYPE_INT64:
            err = nvs_get_i64(my_handle, key, (int64_t*)data);
            break;
        case DATA_TYPE_STRING:
            err = nvs_get_str(my_handle, key, (char*)data, NULL);
            break;
        default:
            return ESP_FAIL;
    }

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
