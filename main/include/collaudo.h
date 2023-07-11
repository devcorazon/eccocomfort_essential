/*
 * collaudo.h
 *
 *  Created on: 29 juin 2023
 *      Author: youcef.benakmoume
 */

#ifndef COLLAUDO_H
#define COLLAUDO_H

#include <esp_err.h>
#include <esp_log.h>
#include "esp_console.h"
#include <string.h>
#include "rgb_led.h"
#include "sensor.h"
#include "fan.h"
#include "wifi.h"
#include "bluetooth.h"


#define PROMPT_STR CONFIG_IDF_TARGET

void collaudo_task(void *pvParameters);
static esp_err_t do_test_led_cmd(int argc, char **argv);
static esp_err_t do_test_all_cmd(int argc, char **argv);
static esp_err_t do_test_fan_cmd(int argc, char **argv);
static esp_err_t do_test_start_cmd(int argc, char **argv);
static esp_err_t do_test_stop_cmd(int argc, char** argv);

#endif /* COLLAUDO_H */


