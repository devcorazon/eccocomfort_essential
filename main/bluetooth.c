/*
 * bluetooth.c
 *
 *  Created on: 10 juil. 2023
 *      Author: youcef.benakmoume
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bluetooth.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "freertos/semphr.h"
#include <inttypes.h>  // Include for PRIx32
#include "storage.h"

#define LOG_TAG "ADV_BLE"

#define ADV_HANDLE      0
#define NUM_ADV         1

static bool is_bt_mem_released = false;

static esp_ble_adv_params_t adv_params = {
    .adv_int_min       = 0x20,
    .adv_int_max       = 0x40,
    .adv_type          = ADV_TYPE_NONCONN_IND,
    .own_addr_type     = BLE_ADDR_TYPE_PUBLIC,
    .channel_map       = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event)
    {
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        if(param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(LOG_TAG, "Advertising start failed");
        }
        else
        {
            ESP_LOGI(LOG_TAG, "Advertising start successfully");
        }
        break;
    default:
        break;
    }
}

void ble_advertising_start()
{
	esp_err_t ret;
	char serial_number_str[9];  // For the hexadecimal string. 8 characters for the hexadecimal representation and 1 for the null-terminator.
	uint32_t serial_number = get_serial_number();

	snprintf(serial_number_str, sizeof(serial_number_str), "%08lX", serial_number); // Convert to hex string

	// Create advertising data dynamically
	uint8_t adv_data[26] = {
	    0x02, 0x01, 0x06,
	    0x02, 0x0a, 0x0b,
	    0x12, 0x09, 'E', 'C', 'M', 'F', '-'
	};

	memcpy(&adv_data[13], serial_number_str, 8);

    if (!is_bt_mem_released)
    {
        ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
        is_bt_mem_released = true;
    }

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_IDLE)
    {
        ret = esp_bt_controller_init(&bt_cfg);
        if (ret)
        {
            ESP_LOGE(LOG_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
            return;
        }
    }

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_INITED) {
        ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
        if (ret)
        {
            ESP_LOGE(LOG_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
            return;
        }
    }

    ret = esp_bluedroid_init();
    if (ret)
    {
        ESP_LOGE(LOG_TAG, "%s init bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret)
    {
        ESP_LOGE(LOG_TAG, "%s enable bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret)
    {
        ESP_LOGE(LOG_TAG, "gap register error, error code = %x", ret);
        return;
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);

    // Replace the 'A', 'D', 'V', '_', 'E', 'C', 'O', '_', 'C', 'O', 'M', 'F', 'O', 'R', 'T', '_', '8', '0'
    // with 'E', 'C', 'M', 'F', '-', and the serial_number_str

    memcpy(&adv_data[13], serial_number_str, 8);  // Copy the serial_number_str into adv_data.

    ret = esp_ble_gap_config_adv_data_raw(adv_data, sizeof(adv_data));
    if (ret)
    {
        ESP_LOGE(LOG_TAG, "config adv data raw failed, error code = %x", ret);
    }

    // start advertising
    ret = esp_ble_gap_start_advertising(&adv_params);
    if (ret)
    {
        ESP_LOGE(LOG_TAG, "start advertising failed, error code = %x", ret);
    }
}

void ble_advertising_stop()
{
    esp_err_t ret;

    // stop advertising
    ret = esp_ble_gap_stop_advertising();
    if (ret) {
        ESP_LOGE(LOG_TAG, "stop advertising failed, error code = %x", ret);
    }

    // disable bluedroid
    ret = esp_bluedroid_disable();
    if (ret) {
        ESP_LOGE(LOG_TAG, "%s disable bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
    }

    // deinit bluedroid
    ret = esp_bluedroid_deinit();
    if (ret) {
        ESP_LOGE(LOG_TAG, "%s deinit bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
    }

    // disable controller
    ret = esp_bt_controller_disable();
    if (ret) {
        ESP_LOGE(LOG_TAG, "%s disable controller failed: %s\n", __func__, esp_err_to_name(ret));
    }
}
