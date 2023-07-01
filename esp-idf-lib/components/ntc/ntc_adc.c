/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#include "ntc_adc.h"

#define I2C_FREQ_HZ 1000000 // 1MHz

static const char *TAG = "ntc_adc";

/**
 * @brief Initialize device ADC for NTC
 *
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_init()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_11);

    return ESP_OK;
}

/**
 * @brief ADC read and temperature calculation
 * @param temperaturev value int16_t
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_temperature(int16_t *temperature)
{
    int raw = adc1_get_raw(ADC1_CHANNEL_2);

    if (raw == NTC_ADC_RESOLUTION - 1)
    {
        ESP_LOGE(TAG, "Failed to read NTC value");
        return ESP_FAIL;
    }

    uint32_t tmp = raw * NTC_ADC_LEG_RESISTANCE;
    tmp /= (NTC_ADC_RESOLUTION - 1) - raw;

    size_t i;
    for (i = 0; i < sizeof(ntc_convert)/sizeof(ntc_convert[0]); i++)
    {
        if (ntc_convert[i].resistance <= tmp)
            break;
    }

    if (i == 0)
    {
        *temperature = ntc_convert[i].temperature;
    }
    else if (i == sizeof(ntc_convert)/sizeof(ntc_convert[0]))
    {
        *temperature = ntc_convert[sizeof(ntc_convert)/sizeof(ntc_convert[0]) - 1].temperature;
    }
    else
    {
        *temperature = ntc_convert[i - 1].temperature + (((tmp - ntc_convert[i - 1].resistance) *
                  (ntc_convert[i - 1].temperature - ntc_convert[i].temperature)) /
                  (ntc_convert[i - 1].resistance  - ntc_convert[i].resistance));
    }

    return ESP_OK;
}
