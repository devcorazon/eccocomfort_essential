#include "fan.h"


static esp_err_t fan_pwm_init();
static bool fan_initialized = false;

static int fan_speed = 0;

static esp_err_t fan_pwm_init()
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer =
    {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_13_BIT,  // Use 13-bit resolution for more granularity
        .freq_hz = FAN_FREQUENCE, // Set frequency of pwm
    };
    ledc_timer_config(&ledc_timer);

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t pwm_config =
    {
        .gpio_num = FAN_SPEED_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = FAN_PWM_PULSE_SPEED_NONE_IN , // Set initial duty cycle to 0
    };
    ledc_channel_config(&pwm_config);

    esp_rom_gpio_pad_select_gpio(FAN_DIRECTION_PIN);
    gpio_set_direction(FAN_DIRECTION_PIN, GPIO_MODE_OUTPUT);

    fan_initialized = true;

    return ESP_OK;
}

esp_err_t fan_set(uint8_t direction,uint8_t speed)
{
	if(!fan_initialized)
	{
	   fan_pwm_init();
	}
    // setting direction
    gpio_set_level(FAN_DIRECTION_PIN, (int)direction);

	// Check speed level bounds
    if (speed < 0)
    {
    	speed = 0;
    }
    else if (speed > 5)
    {
    	speed = 5;
    }

    // Choose direction pulse width
    if (direction == FAN_IN)
    {
        fan_speed = fan_pwm_pulse_in[speed];
    }
    else
    {
        fan_speed = fan_pwm_pulse_out[speed];
    }

    // Set the duty cycle
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, fan_speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    // Set values to the runtime struct
    set_mode_state(direction);
    set_speed_state(fan_speed);

	return ESP_OK;
}

void fan_task(void *pvParameters)
{
    // init fan
    fan_pwm_init();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100)); // Sleep for 100 ms
    }
}
