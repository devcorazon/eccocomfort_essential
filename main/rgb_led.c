/*
 * rgb_led.c
 *
 *  Created on: 30 juin 2023
 *      Author: youcef.benakmoume
 */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "rgb_led.h"

typedef struct {
    uint8_t led_color;
    uint32_t blink_duration;
    uint32_t blink_period;
} blink_params_t;

ktd2027_t ktd;

static esp_err_t rgb_led_init();

static esp_err_t rgb_led_init()
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

void rgb_led_blink_task(void *pvParameters)
{
    blink_params_t *params = (blink_params_t *)pvParameters;

    uint8_t led_mode = 1;  // Assuming led_mode 1 represents blinking
    uint32_t blink_cycles = params->blink_duration / params->blink_period;  // Calculate the number of blink cycles

    for (uint32_t i = 0; i < blink_cycles; i++)
    {
        // Turn on the LED
        ESP_ERROR_CHECK(ktd2027_led_set(&ktd, params->led_color, led_mode));
        vTaskDelay(params->blink_period / portTICK_PERIOD_MS);

        // Turn off the LED
        ESP_ERROR_CHECK(ktd2027_led_set(&ktd, params->led_color, 0));
        vTaskDelay(params->blink_period / portTICK_PERIOD_MS);
    }

    // Free the memory allocated for parameters
    free(params);

    vTaskDelete(NULL);
}

esp_err_t rgb_led_blink(uint8_t led_color, uint32_t blink_duration, uint32_t blink_period)
{
    blink_params_t *params = (blink_params_t *)malloc(sizeof(blink_params_t));
    if (params == NULL)
    {
        return ESP_FAIL;
    }

    params->led_color = led_color;
    params->blink_duration = blink_duration;
    params->blink_period = blink_period;

    BaseType_t task_created = xTaskCreate(rgb_led_blink_task, "Blink task", 2048, params, 5, NULL);

    return task_created == pdPASS ? ESP_OK : ESP_FAIL;
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
