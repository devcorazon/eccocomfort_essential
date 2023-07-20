/*
 * rgb_led.c
 *
 *  Created on: 30 juin 2023
 *      Author: youcef.benakmoume
 */

#include <freertos/FreeRTOS.h>
#include "rgb_led.h"

ktd2027_t ktd;
static esp_err_t rgb_led_init();

esp_err_t rgb_led_init()
{
    // Initialize KTD2027 I2C and descriptor
    memset(&ktd, 0, sizeof(ktd));
    ESP_ERROR_CHECK(ktd2027_init_desc(&ktd, 0, I2C_MASTER_SDA_PIN, I2C_MASTER_SCL_PIN));
    ESP_ERROR_CHECK(ktd2027_init(&ktd));

    // Initialize KTD2027 registers
    ESP_ERROR_CHECK(ktd2027_init(&ktd));

    return ESP_OK;
}

esp_err_t rgb_led_set(uint8_t led_color,uint8_t led_mode)
{
	ESP_ERROR_CHECK(ktd2027_led_set(&ktd,led_color,led_mode));
	return ESP_OK;
}

esp_err_t rgb_led_blink(uint8_t led_color, uint32_t blink_duration, uint32_t blink_period)
{
    uint8_t led_mode = 1;  // Assuming led_mode 1 represents blinking

    uint32_t blink_cycles = blink_duration / blink_period;  // Calculate the number of blink cycles

    for (uint32_t i = 0; i < blink_cycles; i++)
    {
        // Turn on the LED
        ESP_ERROR_CHECK(ktd2027_led_set(&ktd, led_color, led_mode));
        vTaskDelay(blink_period / portTICK_PERIOD_MS);

        // Turn off the LED
        ESP_ERROR_CHECK(ktd2027_led_set(&ktd, led_color, 0));
        vTaskDelay(blink_period / portTICK_PERIOD_MS);
    }

    return ESP_OK;
}

void rgb_led_task(void *pvParameters)
{
    // init led
	rgb_led_init();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100)); // Sleep for 100 ms
    }
}
