/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#include "ir_receiver.h"

static const char *TAG = "ir_receiver";
static RingbufHandle_t rb = NULL;

/**
 * @brief Initialize the IR receiver
 *
 * This function initializes the GPIO and RMT required for the IR receiver to function.
 *
 * @return ESP_OK if initialization is successful, error code otherwise.
 */
esp_err_t ir_receiver_init()
{
    rmt_config_t rmt_rx;
    rmt_rx.channel = RMT_RX_CHANNEL;
    rmt_rx.gpio_num = RMT_RX_GPIO_NUM;
    rmt_rx.clk_div = 80;
    rmt_rx.mem_block_num = 1;
    rmt_rx.rmt_mode = RMT_MODE_RX;
    rmt_rx.rx_config.filter_en = true;
    rmt_rx.rx_config.filter_ticks_thresh = 100;
    rmt_rx.rx_config.idle_threshold = 12000;

    esp_err_t err = rmt_config(&rmt_rx);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize RMT receiver: %s", esp_err_to_name(err));
        return ESP_FAIL;
    }
    ESP_ERROR_CHECK(rmt_driver_install(rmt_rx.channel, 1000, 0));

    ESP_ERROR_CHECK(rmt_get_ringbuf_handle(rmt_rx.channel, &rb));
    ESP_ERROR_CHECK(rmt_rx_start(rmt_rx.channel, 1));

    return ESP_OK;
}

/**
 * @brief Read IR data
 *
 * This function reads the duration and level of the IR signal from the IR receiver.
 *
 * @param duration Pointer to a uint32_t where the duration of the signal will be stored.
 * @param level Pointer to a bool where the level of the signal will be stored.
 * @return ESP_OK if the reading is successful, ESP_FAIL otherwise.
 */
esp_err_t ir_receiver_read(uint32_t *duration, bool *level)
{
    size_t rx_size = 0;
    rmt_item32_t* item = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 1000);

    if(item)
    {
        *duration = item->duration0 * 10 + item->duration1 * 10;
        *level = item->level0;

        // after parsing the data, return spaces to ringbuffer.
        vRingbufferReturnItem(rb, (void*) item);

        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}

/**
 * @brief The IR receiver task
 *
 * This task reads from the IR receiver continuously and logs the received signals.
 * It should be started with xTaskCreate().
 *
 * @param pvParameters Pointer to the task's parameters, which is not used in this task.
 */
void ir_receive_task(void *pvParameters)
{
    uint32_t duration;
    bool level;

    // Initialize IR Receiver
    ESP_ERROR_CHECK(ir_receiver_init());

    // Wait until all set up
    vTaskDelay(pdMS_TO_TICKS(250));
    TickType_t last_wakeup = xTaskGetTickCount();

    while (1)
    {
        esp_err_t err = ir_receiver_read(&duration, &level);

        if (err == ESP_OK)
        {
        	ESP_LOGI(TAG, "Received IR pulse: level=%d, duration=%u", level, (unsigned int)duration);
        }
        else
        {
            ESP_LOGI(TAG, "No IR signal received");
        }

        // Delay for a short while to not flood the log
        vTaskDelayUntil(&last_wakeup, pdMS_TO_TICKS(200));
    }
}
