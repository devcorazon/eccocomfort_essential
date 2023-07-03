/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */
#include "ntc_adc.h"

static const char *TAG = "ntc_adc";

static int adc_raw[2][10];
static int voltage[2][10];
static bool do_calibration1;

adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc1_cali_handle = NULL;

/**
 * @brief Initialize device ADC for NTC
 *
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_init()
{
	 adc_oneshot_unit_init_cfg_t init_config1 =
	 {
	     .unit_id = ADC_UNIT_1,
	 };
	 ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

	 //-------------ADC1 Config---------------//
	 adc_oneshot_chan_cfg_t config =
	 {
	    .bitwidth = ADC_BITWIDTH_DEFAULT,
	     .atten = ADC_ATTEN_DB_11,
	 };

	 ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN3, &config));

	 //-------------ADC1 Calibration Init---------------//

	 do_calibration1 = adc_calibration_init(ADC_UNIT_1, ADC_ATTEN_DB_11, &adc1_cali_handle);

   return ESP_OK;
}

/**
 * @brief ADC read and temperature calculation
 * @param temperaturev value int16_t
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_temperature(int16_t *temperature)
{
	 ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN3, &adc_raw[0][1]));
	 ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN3, adc_raw[0][1]);
	 if (do_calibration1)
	 {
	 ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw[0][1], &voltage[0][1]));
	 ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN3, voltage[0][1]);
	 }

	 uint32_t tmp = adc_raw[0][1] * NTC_ADC_LEG_RESISTANCE;
	     tmp /= (ADC_BITWIDTH - 1) - adc_raw[0][1];
	 	if (adc_raw[0][1] == ((1U << ADC_BITWIDTH) - 1U))
	 	{
	 		ESP_LOGE("NTC", "adc_read - failed");
	 		return -1;
	 	}

	     size_t i;
	     for (i = 0; i < sizeof(ntc_convert)/sizeof(ntc_convert[0]); i++)
	     {
	         if (ntc_convert[i].resistance <= tmp)
	             break;
	     }
	 	tmp = adc_raw[0][1] * NTC_ADC_LEG_RESISTANCE;
	 	tmp /= (((1U << ADC_BITWIDTH) - 1U) - adc_raw[0][1]);

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
	 	for (i = 0U; i < sizeof(ntc_convert) / sizeof(ntc_shape_t); i++)
	 	{
	 		if (ntc_convert[i].resistance <= tmp)
	 			break;
	 	}
	 	if (i == 0U)
	 	{
	 		*temperature = ntc_convert[i].temperature;
	 	}
	 	else if (i == sizeof(ntc_convert) / sizeof(ntc_shape_t))
	 	{
	 		*temperature =
	 				ntc_convert[sizeof(ntc_convert) / sizeof(ntc_shape_t) - 1].temperature;
	 	}
	 	else
	 	{
	 		*temperature = ntc_convert[i - 1].temperature
	 				+ (((tmp - ntc_convert[i - 1].resistance)
	 						* (ntc_convert[i - 1].temperature
	 								- ntc_convert[i].temperature))
	 						/ (ntc_convert[i - 1].resistance
	 								- ntc_convert[i].resistance));
	 	}

	 	int16_t temperature_ntc =  *temperature ;
	 	ESP_LOGI(TAG, "TEMPERATURE %d 1",temperature_ntc);

	return ESP_OK;
}

/*---------------------------------------------------------------
        ADC Calibration
---------------------------------------------------------------*/
static bool adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config =
        {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK)
        {
            calibrated = true;
        }
    }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated)
    {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else
    {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

static void adc_calibration_deinit(adc_cali_handle_t handle)
{
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    ESP_LOGI(TAG, "deregister %s calibration scheme", "Curve Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(handle));

#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    ESP_LOGI(TAG, "deregister %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(handle));
#endif
}

