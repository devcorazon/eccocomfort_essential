/*
 * sensor.h
 *
 *  Created on: 26 juin 2023
 *      Author: youcef.benakmoume
 */

#ifndef MAIN_INCLUDE_SENSOR_H_
#define MAIN_INCLUDE_SENSOR_H_

#include <stdbool.h>
#include <i2cdev.h>
#include <esp_err.h>
#include <string.h>

#define I2C_MASTER_SDA_PIN 4
#define I2C_MASTER_SCL_PIN 5

esp_err_t sensor_init();
void task_sensor_measure(void *pvParameters);
static const char *voc_index_name(int32_t voc_index);

#endif /* MAIN_INCLUDE_SENSOR_H_ */
