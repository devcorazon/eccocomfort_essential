/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#include "ntc_adc.h"

static const char *TAG = "ntc_adc";

/**
 * @brief Initialize device ADC for NTC
 *
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_init()
{
	// Configure ADC
//	ESP_ERROR_CHECK(adc1_config_width(NTC_ADC_RESOLUTION));
//	ESP_ERROR_CHECK(adc1_config_channel_atten(NTC_ADC_CHANNEL, NTC_ADC_ATTEN));

    return ESP_OK;
}

/**
 * @brief ADC read and temperature calculation
 * @param temperaturev value int16_t
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_temperature(int16_t *temperature)
{
	uint32_t adc_reading = 0;
	int16_t temperature_C = 0;
	int16_t adc_raw=0;
	int16_t voltage=0;
	uint32_t tmp;
	size_t i;

    adc_oneshot_unit_handle_t adc_handle = NULL;
    adc_oneshot_unit_init_cfg_t adc_init_config = {
        .unit_id = ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    adc_oneshot_new_unit(&adc_init_config, &adc_handle);

    adc_oneshot_chan_cfg_t adc_chan_config = {
        .bitwidth = ADC_BITWIDTH,
        .atten = ADC_ATTEN,
    };
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &adc_chan_config);

    adc_cali_handle_t adc_cali_handle = NULL;

    adc_cali_curve_fitting_config_t adc_cali_config = {
        .unit_id = ADC_UNIT,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH,
    };
    adc_cali_create_scheme_curve_fitting(&adc_cali_config, &adc_cali_handle);

    adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_raw);
    adc_cali_raw_to_voltage(adc_cali_handle, adc_raw, &voltage);

     printf("Raw: %d - Voltage: %d\r\n", adc_raw, voltage);

	return ESP_OK;
}
