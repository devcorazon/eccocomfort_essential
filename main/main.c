/*
 * main.c
 *
 *  Created on: 26 juin 2023
 *      Author: youcef.benakmoume
 */

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>
#include "main.h"
#include "sensor.h"

static const char *TAG = "ecocomfort-essential";

void app_main()
{
    xTaskCreatePinnedToCore(task_sensor_measure, "sht4x_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);

}
