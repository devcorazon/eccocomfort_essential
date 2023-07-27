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
#include <ntc_adc.h>
#include <storage.h>
#include <stdint.h>

#define I2C_MASTER_SDA_PIN 4
#define I2C_MASTER_SCL_PIN 5

esp_err_t sensor_init();
void sensor_measure_task(void *pvParameters);

#endif /* MAIN_INCLUDE_SENSOR_H_ */
