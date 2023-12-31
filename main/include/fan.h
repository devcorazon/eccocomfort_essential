/*
 * fan.h
 *
 *  Created on: 29 juin 2023
 *      Author: youcef.benakmoume
 */

#ifndef MAIN_INCLUDE_FAN_H_
#define MAIN_INCLUDE_FAN_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_rom_gpio.h"
#include "storage.h"

#define FAN_SPEED_PIN    19
#define FAN_DIRECTION_PIN 18

#define FAN_PWM_PERIOD    (USEC_PER_SEC / 10000UL)
#define FAN_PWM_MAX    (8191UL)

#define FAN_FREQUENCE 1000

#define FAN_PWM_PULSE_SPEED_NONE_IN    ((FAN_PWM_MAX * 0UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_SLEEP_IN   ((FAN_PWM_MAX * 20UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_LOW_IN     ((FAN_PWM_MAX * 40UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_MEDIUM_IN  ((FAN_PWM_MAX * 65UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_HIGH_IN    ((FAN_PWM_MAX * 90UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_BOOST_IN    ((FAN_PWM_MAX * 100UL) / FAN_PWM_PERIOD)

#define FAN_PWM_PULSE_SPEED_NONE_OUT    ((FAN_PWM_MAX * 0UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_SLEEP_OUT   ((FAN_PWM_MAX * 22UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_LOW_OUT     ((FAN_PWM_MAX * 39UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_MEDIUM_OUT  ((FAN_PWM_MAX * 62UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_HIGH_OUT    ((FAN_PWM_MAX * 85UL) / FAN_PWM_PERIOD)
#define FAN_PWM_PULSE_SPEED_BOOST_OUT   ((FAN_PWM_MAX * 100UL) / FAN_PWM_PERIOD)

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

enum
{
    FAN_STOP = 0,
    FAN_IN = 0,
    FAN_OUT = 1
};

esp_err_t fan_set(uint8_t direction,uint8_t speed);
void fan_task(void *pvParameters);


#endif /* MAIN_INCLUDE_FAN_H_ */
