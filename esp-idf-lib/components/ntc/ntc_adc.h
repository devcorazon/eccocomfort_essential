/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#ifndef __NTC_ADC_H__
#define __NTC_ADC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"


#ifdef __cplusplus
extern "C" {
#endif

#define ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED 1

/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
//ADC1 Channel 3
#define EXAMPLE_ADC1_CHAN3   ADC_CHANNEL_3
// Number of ADC sample in order to get an adc value
#define NO_OF_SAMPLE 64
#if 1
#define NTC_ADC_VIN 3300ul
#else
#define NTC_ADC_VIN 3000ul
#endif
// Update this value based on your hardware specification
#define NTC_ADC_LEG_RESISTANCE 10000ul

#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_CHANNEL ADC_CHANNEL_3

#define NTC_ADC_TEMPERATURE_SCALE 10 // Define the temperature scale here

typedef struct {
    uint32_t resistance;
    int16_t temperature;
} ntc_shape_t;

static const ntc_shape_t ntc_convert[] = {
#if 0
    {329500, -50 * NTC_ADC_TEMPERATURE_SCALE},
    {247700, -45 * NTC_ADC_TEMPERATURE_SCALE},
    {188500, -40 * NTC_ADC_TEMPERATURE_SCALE},
    {144100, -35 * NTC_ADC_TEMPERATURE_SCALE},
    {111300, -30 * NTC_ADC_TEMPERATURE_SCALE},
    {86430, -25 * NTC_ADC_TEMPERATURE_SCALE},
    {67770, -20 * NTC_ADC_TEMPERATURE_SCALE},
    {53410, -15 * NTC_ADC_TEMPERATURE_SCALE},
    {42470, -10 * NTC_ADC_TEMPERATURE_SCALE},
    {33900, -5  * NTC_ADC_TEMPERATURE_SCALE},
    {27280,  0},
    {22050,  5  * NTC_ADC_TEMPERATURE_SCALE},
    {17960,  10 * NTC_ADC_TEMPERATURE_SCALE},
    {14690,  15 * NTC_ADC_TEMPERATURE_SCALE},
    {12090,  20 * NTC_ADC_TEMPERATURE_SCALE},
    {10000,  25 * NTC_ADC_TEMPERATURE_SCALE},
    {8313,  30 * NTC_ADC_TEMPERATURE_SCALE},
    {6940,  35 * NTC_ADC_TEMPERATURE_SCALE},
    {5827,  40 * NTC_ADC_TEMPERATURE_SCALE},
    {4911,  45 * NTC_ADC_TEMPERATURE_SCALE},
    {4160,  50 * NTC_ADC_TEMPERATURE_SCALE}
#else
    {129900, -30 * NTC_ADC_TEMPERATURE_SCALE},
    {98180, -25 * NTC_ADC_TEMPERATURE_SCALE},
    {75020, -20 * NTC_ADC_TEMPERATURE_SCALE},
    {57930, -15 * NTC_ADC_TEMPERATURE_SCALE},
    {45170, -10 * NTC_ADC_TEMPERATURE_SCALE},
    {35550, -5  * NTC_ADC_TEMPERATURE_SCALE},
    {28200,  0},
    {22600,  5  * NTC_ADC_TEMPERATURE_SCALE},
    {18230,  10 * NTC_ADC_TEMPERATURE_SCALE},
    {14820,  15 * NTC_ADC_TEMPERATURE_SCALE},
    {12130,  20 * NTC_ADC_TEMPERATURE_SCALE},
    {10000,  25 * NTC_ADC_TEMPERATURE_SCALE},
    {8295,  30 * NTC_ADC_TEMPERATURE_SCALE},
    {6922,  35 * NTC_ADC_TEMPERATURE_SCALE},
    {5810,  40 * NTC_ADC_TEMPERATURE_SCALE},
    {4903,  45 * NTC_ADC_TEMPERATURE_SCALE},
    {4160,  50 * NTC_ADC_TEMPERATURE_SCALE}
#endif
};


/**
 * @brief Initialize device ADC for NTC
 *
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_init();

/**
 * @brief ADC read and temperature calculation
 * @param temperaturev value int16_t
 * @return `ESP_OK` on success
 */
esp_err_t ntc_adc_temperature(int16_t *temperature);


static bool adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
static void adc_calibration_deinit(adc_cali_handle_t handle);


#ifdef __cplusplus
}
#endif

/**@}*/

#endif  // __NTC_ADC_H__
