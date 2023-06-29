/*
 * collaudo.c
 *
 *  Created on: 29 juin 2023
 *      Author: youcef.benakmoume
 */
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "collaudo.h"
#include "ktd2027.h"
#include "sensor.h"

extern ktd2027_t ktd;

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

	const esp_console_cmd_t cmd_set_led = {
	   .command = "set_led",
	   .help = "Set LED {color} {on/off}",
	   .hint = NULL,
	   .func = do_set_led_cmd,
	 };
	    const esp_console_cmd_t cmd_get_sensor = {
	   .command = "get_sensor",
	   .help = "Get Sensor (type)",
	   .hint = NULL,
	    .func = do_get_sensor_cmd,
	 };
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_set_led));
	 ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_get_sensor));
    // Suspend task after initializing the console.
    for(;;) {
       vTaskSuspend(NULL);
    }
}


static esp_err_t do_set_led_cmd(int argc, char **argv)
{
    if(argc < 3) {
        printf("Invalid arguments. Usage: set_led <color> <on/off>\n");
        return ESP_FAIL;
    }
    const char *color = argv[1];
    const char *state = argv[2];

    uint8_t led_mode;
    uint8_t led_color;
    uint16_t duration = 1000;

    if (strcmp(color, "red") == 0)
    {
    	led_color = RGB_LED_COLOR_RED;
    }
    else if (strcmp(color, "green") == 0)
    {
    	led_color = RGB_LED_COLOR_GREEN;
    }
    else if (strcmp(color, "blue") == 0)
    {
    	led_color = RGB_LED_COLOR_BLUE;
    }
    else if (strcmp(color, "all") == 0)
    {
        // Handle 'all' case separately
    	led_color = RGB_LED_COLOR_ALL;
    }
    else
    {
        printf("Invalid color. Supported colors: red, green, blue, all\n");
        return ESP_FAIL;
    }

    if (strcmp(state, "on") == 0)
    {
    	led_mode = RGB_LED_MODE_ON;
    }
    else if (strcmp(state, "off") == 0)
    {
    	led_mode = RGB_LED_MODE_OFF;
    }
    else if (strcmp(state, "blink") == 0)
    {
    	led_mode = RGB_LED_MODE_BLINK;
    }
    else
    {
        printf("Invalid state. Supported states: on, off, blink\n");
        return ESP_FAIL;
    }

    if (led_color == RGB_LED_COLOR_ALL && led_mode == RGB_LED_MODE_ON )
    {
        // Handle 'all' case by iterating over all LEDs
         printf("all led on");
    }
    else if (led_color == RGB_LED_COLOR_ALL && led_mode == RGB_LED_MODE_OFF )
    {
        // Handle 'all' case by iterating over all LEDs
         printf("all led off");
    }
    else
    {
        // Handle " one led "
    	printf("ok im in led %d",led_color);
   // 	ktd2027_led_set(&ktd, led_color, led_mode, duration);
    }
    return ESP_OK;
}

static esp_err_t do_get_sensor_cmd(int argc, char **argv)
{
	if(argc < 2)
	{
	   printf("Invalid arguments. Usage: get_sensor <type>\n");
	   return ESP_FAIL;
	}
	const char *type = argv[1];
	const char *state = argv[2];

	if (strcmp(type, "temperature") == 0)
	{
		printf("Temperature =  %d C\n",get_temperature());
	}
	else if (strcmp(type, "humidity") == 0)
	{
		printf("Temperature =  %d C\n",get_relative_humidity());
	}
	else
	{
		printf("Invalid sensor. Supported sensors: temperature, humidity\n");
		return ESP_FAIL;
	}
	return ESP_OK;
}
