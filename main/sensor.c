/*
 * sensor.c
 *
 *  Created on: 26 juin 2023
 *      Author: youcef.benakmoume
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>*
#include "sensor.h"


static const char *TAG = "sensor measure";

sht4x_t sht;
sgp40_t sgp;
ltr303_t ltr;

esp_err_t sensor_init()
{
    // Init I2C bus
    ESP_ERROR_CHECK(i2cdev_init());

    // Initialize SHT40
    memset(&sht, 0, sizeof(sht));
    ESP_ERROR_CHECK(sht4x_init_desc(&sht, 0, I2C_MASTER_SDA_PIN, I2C_MASTER_SCL_PIN));
    ESP_ERROR_CHECK(sht4x_init(&sht));

    // Initialize SGP40
    memset(&sgp, 0, sizeof(sgp));
    ESP_ERROR_CHECK(sgp40_init_desc(&sgp, 0, I2C_MASTER_SDA_PIN, I2C_MASTER_SCL_PIN));
    ESP_ERROR_CHECK(sgp40_init(&sgp));
    ESP_LOGI(TAG, "SGP40 initialized. Serial: 0x%04x%04x%04x",
             sgp.serial[0], sgp.serial[1], sgp.serial[2]);

    // Initialize LTR303
    memset(&ltr, 0, sizeof(ltr));
    ESP_ERROR_CHECK(ltr303_init_desc(&ltr, 0, I2C_MASTER_SDA_PIN, I2C_MASTER_SCL_PIN));
    ESP_ERROR_CHECK(ltr303_init(&ltr));

    // Initialize NTC ( ADC )
    ESP_ERROR_CHECK(ntc_adc_init());

    return ESP_OK;
}

void task_sensor_measure(void *pvParameters)
{
    float temperature;
    float humidity;
    float lux;
    int32_t voc_index;
    int16_t ntc_temperature;

    // Initialize I2C and all sensors
    ESP_ERROR_CHECK(sensor_init());
    // Wait until all set up
    vTaskDelay(pdMS_TO_TICKS(250));
    TickType_t last_wakeup = xTaskGetTickCount();

    while (1)
    {
        // Perform measurements
        ESP_ERROR_CHECK(sht4x_measure(&sht, &temperature, &humidity));
        ESP_ERROR_CHECK(sgp40_measure_voc(&sgp, humidity, temperature, &voc_index));
        ESP_ERROR_CHECK(ltr303_measure_lux(&ltr, &lux));
        ESP_ERROR_CHECK(ntc_adc_temperature(&ntc_temperature));

        set_temperature(SET_VALUE_TO_TEMP_RAW(temperature));
        set_relative_humidity(SET_VALUE_TO_RH_RAW(humidity));
        set_voc(voc_index);
        set_lux(SET_VALUE_TO_RH_RAW(lux));
        set_ntc_temperature(ntc_temperature);

        // Process and log the sensor readings
 //       ESP_LOGI(TAG, "Lux: %.2f, Temperature: %.2f °C, Humidity: %.2f %%, VOC index: %" PRId32 ", Air is [%s]",
 //                lux, temperature, humidity, voc_index, voc_index_name(voc_index));

        // Wait 1 second for the next sensor reading
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(1000));
    }
}


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

