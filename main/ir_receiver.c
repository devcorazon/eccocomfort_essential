
#include "esp_log.h"
#include "ir_receiver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static RingbufHandle_t rb = NULL;  // Make rb accessible in all functions

/**
 * @brief Initialize the IR receiver
 *
 * This function initializes the GPIO and RMT required for the IR receiver to function.
 *
 * @return ESP_OK if initialization is successful, error code otherwise.
 */
esp_err_t ir_receiver_init()
{
    rmt_config_t rmt_rx =
    {
        .rmt_mode = RMT_MODE_RX,
        .channel = RMT_RX_CHANNEL,
        .clk_div = 80,  // 1 MHz clock
        .gpio_num = RMT_RX_GPIO_NUM,
        .mem_block_num = 1,
        .rx_config = {
            .filter_en = false,
            .idle_threshold = RMT_TICK_10_US,
        }
    };

    ESP_ERROR_CHECK(rmt_config(&rmt_rx));
    ESP_ERROR_CHECK(rmt_driver_install(rmt_rx.channel, RMT_RX_BUFFER_SIZE, 0));

    ESP_ERROR_CHECK(rmt_get_ringbuf_handle(rmt_rx.channel, &rb));
    ESP_ERROR_CHECK(rmt_rx_start(rmt_rx.channel, 1));

    return ESP_OK;
}
/**
 * @brief Read IR data
 *
 * This function reads the duration and level of the IR signal from the IR receiver.
 *
 * @param item Pointer to an array of rmt_item32_t structures where the received IR items will be stored.
 * @param item_num Number of items in the array.
 * @return ESP_OK if the reading is successful, ESP_FAIL otherwise.
 */
esp_err_t ir_receiver_read(rmt_item32_t* item, int item_num)
{
    for (int i = 0; i < item_num; i++)
    {
        int duration0 = item[i].duration0;
        int level0 = item[i].level0;
        int duration1 = item[i].duration1;
        int level1 = item[i].level1;

        printf("Item%d: Level%d Duration%d, Level%d Duration%d\n", i, level0, duration0, level1, duration1);
    }
    return ESP_OK;
}

void ir_receive_task(void* param)
{
	// Init IR Receiver
	ESP_ERROR_CHECK(ir_receiver_init());

    while (rb)
    {
        size_t rx_size = 0;
        rmt_item32_t* item = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 1000);
        if (item)
        {
            int item_num = rx_size / sizeof(rmt_item32_t);
            ESP_ERROR_CHECK(ir_receiver_read(item, item_num));
            vRingbufferReturnItem(rb, (void*) item);
        }
        // delay of 100ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete(NULL);
}
