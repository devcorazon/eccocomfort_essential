/*
 * main.c
 *
 *  Created on: 26 juin 2023
 *      Author: youcef.benakmoume
 */

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "main.h"

static const char *TAG = "ecocomfort-essential";

void app_main()
{
    // Create FreeRTOS task for the collaudo_task
    xTaskCreatePinnedToCore(collaudo_task, "collaudo_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);

    // Create FreeRTOS task for the task_sensor_measure
    xTaskCreatePinnedToCore(task_sensor_measure, "task_sensor_measure", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}
