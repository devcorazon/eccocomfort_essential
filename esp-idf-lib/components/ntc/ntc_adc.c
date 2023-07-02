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
	ESP_ERROR_CHECK(adc1_config_width(NTC_ADC_RESOLUTION));
	ESP_ERROR_CHECK(adc1_config_channel_atten(NTC_ADC_CHANNEL, NTC_ADC_ATTEN));

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
	uint32_t tmp;
	size_t i;

	for (int i = 0; i < NO_OF_SAMPLE; i++)
	{
		adc_reading += adc1_get_raw(NTC_ADC_CHANNEL);
	}
	adc_reading /= NO_OF_SAMPLE;

	if (adc_reading == ((1U << NTC_ADC_RESOLUTION) - 1U))
	{
		ESP_LOGE("NTC", "adc_read - failed");
		return -1;
	}

	tmp = adc_reading * NTC_ADC_LEG_RESISTANCE;
	tmp /= (((1U << NTC_ADC_RESOLUTION) - 1U) - adc_reading);

	for (i = 0U; i < sizeof(ntc_convert) / sizeof(ntc_shape_t); i++)
	{
		if (ntc_convert[i].resistance <= tmp)
			break;
	}

	if (i == 0U)
	{
		*temperature = ntc_convert[i].temperature;
	} else if (i == sizeof(ntc_convert) / sizeof(ntc_shape_t))
	{
		*temperature =
				ntc_convert[sizeof(ntc_convert) / sizeof(ntc_shape_t) - 1].temperature;
	} else
	{
		*temperature = ntc_convert[i - 1].temperature
				+ (((tmp - ntc_convert[i - 1].resistance)
						* (ntc_convert[i - 1].temperature
								- ntc_convert[i].temperature))
						/ (ntc_convert[i - 1].resistance
								- ntc_convert[i].resistance));
	}
	return ESP_OK;
}
