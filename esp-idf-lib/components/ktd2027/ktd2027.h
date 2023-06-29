/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#ifndef __KTD2026_H__
#define __KTD2026_H__

#include <stdint.h>
#include <stdbool.h>
#include <i2cdev.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KTD2027_I2C_ADDR 0x30

/* KTD2027 register map */
#define KTD2027_REG_EN_RST		0x00
#define KTD2027_REG_FLASH_PERIOD	0x01
#define KTD2027_REG_PWM1_TIMER		0x02
#define KTD2027_REG_PWM2_TIMER		0x03
#define KTD2027_REG_LED_EN		    0x04
#define KTD2027_REG_RED_LED		    0x06
#define KTD2027_REG_GREEN_LED		0x07
#define KTD2027_REG_BLUE_LED		0x08
#define KTD2027_TIME_UNIT		    500

#define KTD2027_RED_LED_EN	        0x01
#define KTD2027_GREEN_LED_EN	    0x04
#define KTD2027_BLUE_LED_EN	     	0x10

#define LED_OFF			0x00

#define MAX_BRIGHTNESS_VALUE 0xFF
#define	MAX_NUM_LEDS		3

enum rgb_led_color {
    RGB_LED_COLOR_NONE          = 0,
    RGB_LED_COLOR_RED           = 1,
    RGB_LED_COLOR_GREEN         = 2,
    RGB_LED_COLOR_BLUE          = 3,
	RGB_LED_COLOR_ALL           = 4,
};

enum rgb_led_mode {
    RGB_LED_MODE_OFF            = 0,
    RGB_LED_MODE_ON             = 1,
    RGB_LED_MODE_BLINK          = 2,
};

enum rgb_led_duration {
    RGB_LED_DURATION_ONESHOT    = 5,
    RGB_LED_DURATION_500MS      = 500,
    RGB_LED_DURATION_1000MS     = 1000,
    RGB_LED_DURATION_1500MS     = 1500,
    RGB_LED_DURATION_2000MS     = 2000,
    RGB_LED_DURATION_FOREVER    = 0xFF,
};

/**
 * Device descriptor
 */
typedef struct {
    i2c_dev_t i2c_dev;
} ktd2027_t;

/**
 * @brief Initialize device descriptor
 *
 * @param dev Device descriptor
 * @param port I2C port
 * @param sda_gpio SDA GPIO pin
 * @param scl_gpio SCL GPIO pin
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_init_desc(ktd2027_t *dev,i2c_port_t port,gpio_num_t sda_gpio, gpio_num_t scl_gpio);

/**
 * @brief Free device descriptor
 *
 * @param dev Device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_free_desc(ktd2027_t *dev);

/**
 * @brief Initialize device
 *
 * @param dev Device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_init(ktd2027_t *dev);

/**
 * @brief Set RGB LED
 *
 * @param dev Device descriptor
 * @param uint8_t color
 * @param uint8_t mode
 * @param uint16_t duration
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_led_set(ktd2027_t *dev,uint8_t color, uint8_t mode, uint16_t duration);
#ifdef __cplusplus
}
#endif

/**@}*/

#endif  // __KTD2026_H__
