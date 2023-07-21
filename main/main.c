/*
 * main.c
 *
 *  Created on: 26 juin 2023
 *      Author: youcef.benakmoume
 */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "main.h"

static const char *TAG = "ecocomfort-essential";

void app_main()
{
    uint8_t fw_version_byte[3];
    uint16_t fw_version;

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // init storage
    storage_init();

    // Setting Firmware version
    fw_version_byte[0] = FW_VERSION_MAJOR;
    fw_version_byte[1] = FW_VERSION_MINOR;
    fw_version_byte[2] = FW_VERSION_PATCH;

    fw_version = ((uint16_t)fw_version_byte[0]) << 12 | ((uint16_t)fw_version_byte[1]) << 6 | ((uint16_t)fw_version_byte[2]);
    set_fw_version(fw_version);

    // Getting serial number from eFuse BLK3
    ESP_ERROR_CHECK(efuse_init());

	// Create FreeRTOS task for the collaudo_task
	xTaskCreate(collaudo_task, "collaudo_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create FreeRTOS task for the task_sensor_measure
	xTaskCreate(sensor_measure_task, "sensor_measure_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create IR Receiver task to receiving IR commands
	xTaskCreate(ir_receive_task, "ir_receive_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create Fan task
	xTaskCreate(fan_task, "fan task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create RGB Led task
	xTaskCreate(rgb_led_task, "rgb_led_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create WIFI AP task
//	xTaskCreate(wifi_task, "wifi_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create BLUETOOTH AP task
//	xTaskCreate(ble_advertising_start_task, "bluetooth_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);
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
