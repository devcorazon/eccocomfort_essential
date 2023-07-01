/*
 * sensor.h
 *
 *  Created on: 26 juin 2023
 *      Author: youcef.benakmoume
 */

#ifndef MAIN_INCLUDE_SENSOR_H_
#define MAIN_INCLUDE_SENSOR_H_

#include <i2cdev.h>
#include <esp_err.h>
#include <esp_log.h>
#include <string.h>
#include <sht4x.h>
#include <sgp40.h>
#include <ltr303.h>
#include <storage.h>

#define I2C_MASTER_SDA_PIN 4
#define I2C_MASTER_SCL_PIN 5

// ADD channel number
#define NTC_ADC_CHANNEL 6
// ADC channel resolution
#define NTC_ADC_RESOLUTION ADC_WIDTH_BIT_12
// NTC resistance at 25 degrees Celsius
#define NTC_R25 10000
// ADC voltage reference in millivolts
#define ADC_VREF 3300

esp_err_t sensor_init();
void task_sensor_measure(void *pvParameters);
static const char *voc_index_name(int32_t voc_index);

#endif /* MAIN_INCLUDE_SENSOR_H_ */
