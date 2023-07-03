/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#ifndef __NTC_ADC_H__
#define __NTC_ADC_H__

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>

#include "../esp_adc/include/esp_adc/adc_oneshot.h"
#include "../esp_adc/include/esp_adc/adc_cali.h"
#include "../esp_adc/include/esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"


#ifdef __cplusplus
extern "C" {
#endif

// Number of ADC sample in order to get an adc value
#define NO_OF_SAMPLE 64
// Update this value based on your hardware specification
#define NTC_ADC_LEG_RESISTANCE 10000

#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_11
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_CHANNEL ADC_CHANNEL_3

#define TEMPERATURE_SCALE 100 // Define the temperature scale here

typedef struct {
    uint32_t resistance;
    int16_t temperature;
} ntc_shape_t;

static const ntc_shape_t ntc_convert[] = {
    {329500, -50 * TEMPERATURE_SCALE},
    {247700, -45 * TEMPERATURE_SCALE},
    {188500, -40 * TEMPERATURE_SCALE},
    {144100, -35 * TEMPERATURE_SCALE},
    {111300, -30 * TEMPERATURE_SCALE},
    {86430, -25 * TEMPERATURE_SCALE},
    {67770, -20 * TEMPERATURE_SCALE},
    {53410, -15 * TEMPERATURE_SCALE},
    {42470, -10 * TEMPERATURE_SCALE},
    {33900, -5  * TEMPERATURE_SCALE},
    {27280,  0},
    {22050,  5  * TEMPERATURE_SCALE},
    {17960,  10 * TEMPERATURE_SCALE},
    {14690,  15 * TEMPERATURE_SCALE},
    {12090,  20 * TEMPERATURE_SCALE},
    {10000,  25 * TEMPERATURE_SCALE},
    {8313,  30 * TEMPERATURE_SCALE},
    {6940,  35 * TEMPERATURE_SCALE},
    {5827,  40 * TEMPERATURE_SCALE},
    {4911,  45 * TEMPERATURE_SCALE},
    {4160,  50 * TEMPERATURE_SCALE}
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



#ifdef __cplusplus
}
#endif

/**@}*/

#endif  // __NTC_ADC_H__
