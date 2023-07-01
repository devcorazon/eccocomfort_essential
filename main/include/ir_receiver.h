/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#ifndef __IR_RECEIVER_H__
#define __IR_RECEIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "freertos/ringbuf.h"



#ifdef __cplusplus
extern "C" {
#endif

#define RMT_RX_CHANNEL RMT_CHANNEL_0
#define RMT_RX_GPIO_NUM 2

/**
 * @brief Initialize the IR receiver
 *
 * This function initializes the GPIO and RMT required for the IR receiver to function.
 *
 * @return ESP_OK if initialization is successful, error code otherwise.
 */
esp_err_t ir_receiver_init();

/**
 * @brief Read IR data
 *
 * This function reads the duration and level of the IR signal from the IR receiver.
 *
 * @param duration Pointer to a uint32_t where the duration of the signal will be stored.
 * @param level Pointer to a bool where the level of the signal will be stored.
 * @return ESP_OK if the reading is successful, ESP_FAIL otherwise.
 */
esp_err_t ir_receiver_read(uint32_t *duration, bool *level);


/**
 * @brief The IR receiver task
 *
 * This task reads from the IR receiver continuously and logs the received signals.
 * It should be started with xTaskCreate().
 *
 * @param pvParameters Pointer to the task's parameters, which is not used in this task.
 */
void ir_receive_task(void *pvParameters);


#endif  // __IR_RECEIVER_H__
