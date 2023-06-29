/*
 * collaudo.h
 *
 *  Created on: 29 juin 2023
 *      Author: youcef.benakmoume
 */

#ifndef COLLAUDO_H
#define COLLAUDO_H

#include "esp_console.h"
#include <stdbool.h>
#include <i2cdev.h>
#include <esp_err.h>
#include <string.h>

#define PROMPT_STR CONFIG_IDF_TARGET

void collaudo_task(void *pvParameters);
static esp_err_t do_set_led_cmd(int argc, char **argv);
static esp_err_t do_get_sensor_cmd(int argc, char **argv);

#endif /* COLLAUDO_H */


