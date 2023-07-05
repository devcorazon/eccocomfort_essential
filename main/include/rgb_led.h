/*
 * rgb_led.h
 *
 *  Created on: 30 juin 2023
 *      Author: youcef.benakmoume
 */

#ifndef MAIN_INCLUDE_RGB_LED_H_
#define MAIN_INCLUDE_RGB_LED_H_

#include <i2cdev.h>
#include <esp_err.h>
#include <esp_log.h>
#include <string.h>
#include <ktd2027.h>

#define I2C_MASTER_SDA_PIN 4
#define I2C_MASTER_SCL_PIN 5

esp_err_t rgb_led_set(uint8_t led_color,uint8_t led_mode);
void rgb_led_task(void *pvParameters);

#endif /* MAIN_INCLUDE_RGB_LED_H_ */
