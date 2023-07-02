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
	xTaskCreate(collaudo_task, "collaudo_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create FreeRTOS task for the task_sensor_measure
	xTaskCreate(sensor_measure_task, "sensor_measure_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

	// Create IR Receiver task to receiving IR commands
	xTaskCreate(ir_receive_task, "ir_receive_task", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);

}
