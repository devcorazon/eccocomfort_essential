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
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_led));
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_all));
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_test_fan));
    // Suspend task after initializing the console.
    for(;;) {
       vTaskSuspend(NULL);
    }
}

static esp_err_t do_test_led_cmd(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Invalid arguments. Usage: test_led  <index>\n");
        return ESP_FAIL;
    }
    const char *index = argv[1];

    uint8_t mode;
    uint8_t color;

    if (strcmp(index, "0") == 0)
    {
   	color = RGB_LED_COLOR_NONE;
  	mode = RGB_LED_MODE_OFF;
    }
    else if (strcmp(index, "1") == 0)
    {
        color = RGB_LED_COLOR_RED;
    	mode = RGB_LED_MODE_ON;
    }
    else if (strcmp(index, "2") == 0)
    {
    	color = RGB_LED_COLOR_GREEN;
    	mode = RGB_LED_MODE_ON;
    }
    else if (strcmp(index, "3") == 0)
    {
    	color = RGB_LED_COLOR_BLUE;
    	mode = RGB_LED_MODE_ON;
    }
    else
    {
        printf("Invalid index. Supported colors: 0, 1, 2, 3\n");
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(rgb_led_set(color,mode));

    return ESP_OK;
}

static esp_err_t do_test_all_cmd(int argc, char **argv)
{
    printf("Temperature =  %d.%01d C\n", TEMP_RAW_TO_INT(get_temperature()), TEMP_RAW_TO_DEC(get_temperature()));
	printf("Relative humidity =  %u.%01u %%\n", RH_RAW_TO_INT(get_relative_humidity()), RH_RAW_TO_DEC(get_relative_humidity()));
	printf("VOC Index =  %ld \n", get_voc());
	printf("LUX =  %u.%01u %%\n", RH_RAW_TO_INT(get_lux()), RH_RAW_TO_DEC(get_lux()));
	printf("NTCT Temperature =  %d \n", get_ntc_temperature());

	return ESP_OK;
}

static esp_err_t do_test_fan_cmd(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Invalid arguments. Usage: test_fan <index>\n");
        return ESP_FAIL;
    }
    const char *index = argv[1];
    uint8_t speed;
    uint8_t direction;

    if (strcmp(index, "0") == 0)
    {
        speed = SPEED_NONE;
        direction = FAN_STOP;
    }
    else if (strcmp(index, "1") == 0)
    {
        speed = SPEED_NIGHT;
        direction = FAN_IN;
    }
    else if (strcmp(index, "2") == 0)
    {
        speed = SPEED_LOW;
        direction = FAN_IN;
    }
    else if (strcmp(index, "3") == 0)
    {
        speed = SPEED_MEDIUM;
        direction = FAN_IN;
    }
    else if (strcmp(index, "4") == 0)
    {
        speed = SPEED_HIGH;
        direction = FAN_IN;
    }
    else if (strcmp(index, "5") == 0)
    {
        speed = SPEED_NIGHT;
        direction = FAN_OUT;
    }
    else if (strcmp(index, "6") == 0)
    {
        speed = SPEED_LOW;
        direction = FAN_OUT;
    }
    else if (strcmp(index, "7") == 0)
    {
        speed = SPEED_MEDIUM;
        direction = FAN_OUT;
    }
    else if (strcmp(index, "8") == 0)
    {
        speed = SPEED_HIGH;
        direction = FAN_OUT;
    }
    else if (strcmp(index, "9") == 0)
    {
        speed = SPEED_BOOST;
        direction = FAN_OUT;
    }
    else
    {
        printf("Invalid index. Supported fan speeds: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9\n");
        return ESP_FAIL;
    }

    // Assume that fan_speed and fan_direction are set
    fan_set(direction,speed);

    return ESP_OK;
}
