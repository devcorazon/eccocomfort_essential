/*
 * collaudo.c
 *
 *  Created on: 29 juin 2023
 *      Author: youcef.benakmoume
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdbool.h>
#include "collaudo.h"
#include <stdlib.h>

static const char *TAG = "collaudo";


void collaudo_task(void *pvParameters)
{
	esp_console_repl_t *repl = NULL;
	esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
	/* Prompt to be printed before each line.
	* This can be customized, made dynamic, etc.
	*/
	repl_config.prompt = PROMPT_STR ">";
	repl_config.max_cmdline_length = 40;

	/* Register commands */
	 esp_console_register_help_command();

	esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));
	ESP_ERROR_CHECK(esp_console_start_repl(repl));

	const esp_console_cmd_t cmd_test_led = {
	   .command = "test_led",
	   .help = "Test LED {index}",
	   .hint = NULL,
	   .func = do_test_led_cmd,
	 };
	    const esp_console_cmd_t cmd_test_all = {
	   .command = "test_all",
	   .help = "Test Sensor",
	   .hint = NULL,
	    .func = do_test_all_cmd,
	 };
	 const esp_console_cmd_t cmd_test_fan = {
	   .command = "test_fan",
	   .help = "Test Fan {index}",
	   .hint = NULL,
	   .func = do_test_fan_cmd,
	 };
	 const esp_console_cmd_t cmd_test_start = {
	   .command = "test_start",
	   .help = "Test start",
	   .hint = NULL,
	   .func = do_test_start_cmd,
	 };
	 const esp_console_cmd_t cmd_test_stop = {
	   .command = "test_stop",
	   .help = "Test stop",
	   .hint = NULL,
	   .func = do_test_stop_cmd,
	 };

	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_led));
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_all));
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_fan));
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_start));
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_stop));

    // Suspend task after initializing the console.
    for(;;) {
       vTaskSuspend(NULL);
    }
}

static esp_err_t do_test_led_cmd(int argc, char **argv)
{
    uint8_t mode;
    uint8_t color;

    if(argc < 2)
    {
        printf("Invalid arguments. Usage: test_led  <index>\n");
        return ESP_FAIL;
    }

    char *endptr;
    long index = strtol(argv[1], &endptr, 10); // Base 10 is used.

    // Check for conversion errors
    if (endptr == argv[1] || *endptr != '\0' || index < 0 || index > 3)
    {
        printf("Invalid index. Supported colors: 0, 1, 2, 3\n");
        return ESP_FAIL;
    }

    switch (index)
    {
        case 0:
            color = RGB_LED_COLOR_NONE;
            mode = RGB_LED_MODE_OFF;
            break;
        case 1:
            color = RGB_LED_COLOR_RED;
            mode = RGB_LED_MODE_ON;
            break;
        case 2:
            color = RGB_LED_COLOR_GREEN;
            mode = RGB_LED_MODE_ON;
            break;
        case 3:
            color = RGB_LED_COLOR_BLUE;
            mode = RGB_LED_MODE_ON;
            break;
        default: // This should never happen
            printf("Invalid index. Supported colors: 0, 1, 2, 3\n");
            return ESP_FAIL;
    }

    ESP_ERROR_CHECK(rgb_led_set(color,mode));

    return ESP_OK;
}

static esp_err_t do_test_all_cmd(int argc, char **argv)
{
	uint32_t serial_number = get_serial_number();
    printf("Serial Number: %08lx\n", serial_number);

	uint16_t fw_version = get_fw_version();
    printf("Firmware version : %u\n",fw_version);

    uint16_t temp = get_temperature();
    if (temp == UINT16_MAX)
    {
        printf("Temperature reading error\n");
    }
    else
    {
        printf("Temperature =  %d.%01d C\n", TEMP_RAW_TO_INT(temp), TEMP_RAW_TO_DEC(temp));
    }

    uint16_t rh = get_relative_humidity();
    if (rh == UINT16_MAX)
    {
        printf("Relative humidity reading error\n");
    }
    else
    {
        printf("Relative humidity =  %u.%01u %%\n", RH_RAW_TO_INT(rh), RH_RAW_TO_DEC(rh));
    }

    int32_t voc = get_voc();
    if (voc == UINT16_MAX)
    {
        printf("VOC Index reading error\n");
    }
    else
    {
        printf("VOC Index =  %ld \n", voc);
    }

    uint16_t lux = get_lux();
    if (lux == UINT16_MAX)
    {
        printf("LUX reading error\n");
    }
    else
    {
        printf("LUX =  %u.%01u %%\n", RH_RAW_TO_INT(lux), RH_RAW_TO_DEC(lux));
    }

    uint16_t ntc_temp = get_ntc_temperature();
    if (ntc_temp == UINT16_MAX)
    {
        printf("NTC Temperature reading error\n");
    }
    else
    {
        printf("NTC Temperature =  %d.%01d C\n", TEMP_RAW_TO_INT(ntc_temp), TEMP_RAW_TO_DEC(ntc_temp));
    }

    return ESP_OK;

}

static esp_err_t do_test_fan_cmd(int argc, char **argv)
{
    uint8_t speed;
    uint8_t direction;

    if(argc < 2)
    {
        printf("Invalid arguments. Usage: test_fan <index>\n");
        return ESP_FAIL;
    }

    char *endptr;
    long index = strtol(argv[1], &endptr, 10); // Base 10 is used.

    // Check for conversion errors
    if (endptr == argv[1] || *endptr != '\0' || index < 0 || index > 9) {
        printf("Invalid index. Supported fan speeds: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9\n");
        return ESP_FAIL;
    }

    switch (index)
    {
        case 0:
            speed = SPEED_NONE;
            direction = FAN_STOP;
            break;
        case 1:
            speed = SPEED_NIGHT;
            direction = FAN_IN;
            break;
        case 2:
            speed = SPEED_LOW;
            direction = FAN_IN;
            break;
        case 3:
            speed = SPEED_MEDIUM;
            direction = FAN_IN;
            break;
        case 4:
            speed = SPEED_HIGH;
            direction = FAN_IN;
            break;
        case 5:
            speed = SPEED_NIGHT;
            direction = FAN_OUT;
            break;
        case 6:
            speed = SPEED_LOW;
            direction = FAN_OUT;
            break;
        case 7:
            speed = SPEED_MEDIUM;
            direction = FAN_OUT;
            break;
        case 8:
            speed = SPEED_HIGH;
            direction = FAN_OUT;
            break;
        case 9:
            speed = SPEED_BOOST;
            direction = FAN_OUT;
            break;
        default: // This should never happen
            printf("Invalid index. Supported fan speeds: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9\n");
            return ESP_FAIL;
    }

    fan_set(direction, speed);

    return ESP_OK;
}

static esp_err_t do_test_start_cmd(int argc, char** argv)
{
	ble_advertising_start();
	wifi_ap_start();
	return ESP_OK;
}

static esp_err_t do_test_stop_cmd(int argc, char** argv)
{
	ble_advertising_stop();
	wifi_ap_stop();
	return ESP_OK;
}
