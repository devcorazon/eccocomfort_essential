/*
 * ir_receiver.h
 *
 *  Created on: 29 juin 2023
 *      Author: youcef.benakmoume
 */

#ifndef __IR_RECEIVER_H__
#define __IR_RECEIVER_H__

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>
#include "driver/rmt.h"


#ifdef __cplusplus
extern "C" {
#endif

#define RMT_RX_CHANNEL RMT_CHANNEL_2
#define RMT_RX_GPIO_NUM 2
#define RMT_TICK_10_US (10000) // RMT counter value for 10 us tick
#define RMT_RX_BUFFER_SIZE 100


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
 * @param item Pointer to an array of rmt_item32_t structures where the received IR items will be stored.
 * @param item_num Number of items in the array.
 * @return ESP_OK if the reading is successful, ESP_FAIL otherwise.
 */
esp_err_t ir_receiver_read(rmt_item32_t* item, int item_num);
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
