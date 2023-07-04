#ifndef MAIN_INCLUDE_FAN_H_
#define MAIN_INCLUDE_FAN_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_rom_gpio.h"
#include "storage.h"

#define FAN_SPEED_PIN    18
#define FAN_DIRECTION_PIN 19

#define FAN_PWM_PERIOD    (USEC_PER_SEC / 10000UL)
#define FAN_PWM_MAX    (100UL)

#define FAN_PWM_PULSE_SPEED_NONE_IN    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 0UL)
#define FAN_PWM_PULSE_SPEED_SLEEP_IN    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 20UL)
#define FAN_PWM_PULSE_SPEED_LOW_IN    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 40UL)
#define FAN_PWM_PULSE_SPEED_MEDIUM_IN    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 65UL)
#define FAN_PWM_PULSE_SPEED_HIGH_IN    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 90UL)
#define FAN_PWM_PULSE_SPEED_BOOST_IN    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 100UL)

#define FAN_PWM_PULSE_SPEED_NONE_OUT    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 0UL)
#define FAN_PWM_PULSE_SPEED_SLEEP_OUT    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 22UL)
#define FAN_PWM_PULSE_SPEED_LOW_OUT    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 39UL)
#define FAN_PWM_PULSE_SPEED_MEDIUM_OUT    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 62UL)
#define FAN_PWM_PULSE_SPEED_HIGH_OUT    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 85UL)
#define FAN_PWM_PULSE_SPEED_BOOST_OUT    ((FAN_PWM_PERIOD / FAN_PWM_MAX) * 100UL)

#define USEC_PER_SEC   (1000000UL)

static const uint32_t fan_pwm_pulse_in[] = {
    FAN_PWM_PULSE_SPEED_NONE_IN,
	FAN_PWM_PULSE_SPEED_SLEEP_IN,
    FAN_PWM_PULSE_SPEED_LOW_IN,
    FAN_PWM_PULSE_SPEED_MEDIUM_IN,
    FAN_PWM_PULSE_SPEED_HIGH_IN,
    FAN_PWM_PULSE_SPEED_BOOST_IN
};

static const uint32_t fan_pwm_pulse_out[] = {
    FAN_PWM_PULSE_SPEED_NONE_OUT,
	FAN_PWM_PULSE_SPEED_SLEEP_OUT,
    FAN_PWM_PULSE_SPEED_LOW_OUT,
    FAN_PWM_PULSE_SPEED_MEDIUM_OUT,
    FAN_PWM_PULSE_SPEED_HIGH_OUT,
    FAN_PWM_PULSE_SPEED_BOOST_OUT
};

typedef enum
{
    FAN_STOP = 0,
    FAN_IN,
    FAN_OUT
} FanDirection;

esp_err_t fan_pwm_init();
esp_err_t fan_set_speed(int speed);
esp_err_t fan_set_direction(FanDirection direction);
void fan_task(void *pvParameters);


#endif /* MAIN_INCLUDE_FAN_H_ */
