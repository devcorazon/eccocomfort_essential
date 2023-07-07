/*
 * wifi.h
 *
 *  Created on: 7 juil. 2023
 *      Author: youcef.benakmoume
 */

#ifndef MAIN_INCLUDE_WIFI_H_
#define MAIN_INCLUDE_WIFI_H_

#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

/* The examples use WiFi configuration that you can set via project configuration menu.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      "ecocomfort_essential"
#define EXAMPLE_ESP_WIFI_PASS      "mypassword"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       4


static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
void wifi_init_softap(void);
void wifi_ap_start();
void wifi_ap_stop();

#endif /* MAIN_INCLUDE_WIFI_H_ */
