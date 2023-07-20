/*
 * wifi.c
 *
 *  Created on: 7 juil. 2023
 *      Author: youcef.benakmoume
 */
#include <wifi.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "storage.h"

static const char *TAG = "wifi softAP";

esp_netif_t* ap_netif;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
	/* Creating the new SSID */
	uint32_t serial_number = get_serial_number();
	char serial_number_str[9]; // 8 characters for the hexadecimal number and 1 for the null terminator
	sprintf(serial_number_str, "%08" PRIx32, serial_number); // Convert to hex string

	char ssid_prefix[] = "ECMF-";
	char ssid[32]; // SSID length must be between 8-32 characters

	snprintf(ssid, sizeof(ssid), "%s%s", ssid_prefix, serial_number_str); // Concatenate the prefix and the serial number

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ap_netif = esp_netif_create_default_wifi_ap();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
	                                                    ESP_EVENT_ANY_ID,
	                                                    &wifi_event_handler,
	                                                    NULL,
	                                                    NULL));

	wifi_config_t wifi_config = {
	    .ap = {
	        .channel = EXAMPLE_ESP_WIFI_CHANNEL,
	        .password = EXAMPLE_ESP_WIFI_PASS,
	        .max_connection = EXAMPLE_MAX_STA_CONN,
	        .authmode = WIFI_AUTH_WPA_WPA2_PSK,
	        .pmf_cfg = {
	                .required = false,
	        },
	    },
	};

	// Copy the SSID to the wifi_config
	memcpy(wifi_config.ap.ssid, ssid, sizeof(ssid));
	wifi_config.ap.ssid_len = strlen(ssid);

	if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
	    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void wifi_ap_stop()
{
    ESP_LOGI(TAG, "Stopping WiFi access point...");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());

    //Unregister event handler before deleting default event loop
     ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler));

     //Delete default event loop here
     ESP_ERROR_CHECK(esp_event_loop_delete_default());

     // Destroy the network interface
     esp_netif_destroy(ap_netif);
}

void wifi_ap_start()
{
	// enable wifi in AP MODE
	ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
	wifi_init_softap();
}


