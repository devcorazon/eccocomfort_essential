/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
/**
 * Simple example with SHT4x sensor.
 *
 * It shows different user task implementation: either low level or high level
 * functions are used.
 *
 */
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <sht4x.h>
#include <sgp40.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>

#include "../esp-idf-lib/components/ltr303/ltr303.h"

/* float is used in printf(). you need non-default configuration in
 * sdkconfig for ESP8266, which is enabled by default for this
 * example. see sdkconfig.defaults.esp8266
 */
#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

#define I2C_MASTER_SDA_PIN 4
#define I2C_MASTER_SCL_PIN 5


static const char *TAG = "ecocomfort_essential";

static const char *voc_index_name(int32_t voc_index)
{
    if (voc_index <= 0) return "INVALID VOC INDEX";
    else if (voc_index <= 10) return "unbelievable clean";
    else if (voc_index <= 30) return "extremely clean";
    else if (voc_index <= 50) return "higly clean";
    else if (voc_index <= 70) return "very clean";
    else if (voc_index <= 90) return "clean";
    else if (voc_index <= 120) return "normal";
    else if (voc_index <= 150) return "moderately polluted";
    else if (voc_index <= 200) return "higly polluted";
    else if (voc_index <= 300) return "extremely polluted";

    return "RUN!";
}

void task_sht40_sgp30(void *pvParameters)
{

    float temperature;
    float humidity;
    sgp40_t sgp;
    sht4x_t sht;
    ltr303_t ltr;

    memset(&sht, 0, sizeof(sht4x_t));

    // setup SHT40
    ESP_ERROR_CHECK(sht4x_init_desc(&sht, 0, I2C_MASTER_SDA_PIN, I2C_MASTER_SCL_PIN));
    ESP_ERROR_CHECK(sht4x_init(&sht));

    // setup SGP40
    memset(&sgp, 0, sizeof(sgp));
    ESP_ERROR_CHECK(sgp40_init_desc(&sgp, 0, I2C_MASTER_SDA_PIN, I2C_MASTER_SCL_PIN));
    ESP_ERROR_CHECK(sgp40_init(&sgp));
    ESP_LOGI(TAG, "SGP40 initilalized. Serial: 0x%04x%04x%04x",
            sgp.serial[0], sgp.serial[1], sgp.serial[2]);

    // set LTR303
    memset(&ltr, 0, sizeof(ltr));
    ESP_ERROR_CHECK(ltr303_init_desc(&ltr, 0, I2C_MASTER_SDA_PIN, I2C_MASTER_SCL_PIN));
    ESP_ERROR_CHECK(ltr303_init(&ltr));

    // Wait until all set up
    vTaskDelay(pdMS_TO_TICKS(250));
    TickType_t last_wakeup = xTaskGetTickCount();

    while (1)
    {
        // perform one measurement and do something with the results
        ESP_ERROR_CHECK(sht4x_measure(&sht, &temperature, &humidity));

	    // Get temperature and humidity values and feed it to SGP40
		int32_t voc_index;
		ESP_ERROR_CHECK(sgp40_measure_voc(&sgp, humidity, temperature, &voc_index));

		// Get channel data for LTR303
		uint16_t channel1,channel2;
		ESP_ERROR_CHECK(ltr303_sample_fetch(&ltr, &channel1, &channel2));

		// Get the lux value
		uint32_t lux;
		ESP_ERROR_CHECK(ltr303_get_lux(&ltr, channel1, channel2 , &lux));

		ESP_LOGI(TAG, "Temperature: %.2f °C,Humidity: %.2f %%, VOC index: %" PRIi32 ", Lux: %" PRIu32 ", Air is [%s]",
		        temperature, humidity, voc_index, lux, voc_index_name(voc_index));

		// Wait 1 second for next sensor reading
		vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(1000));

    }
}


void app_main()
{
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreatePinnedToCore(task_sht40_sgp30, "sht4x_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);

}
