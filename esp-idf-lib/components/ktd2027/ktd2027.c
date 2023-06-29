/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#include "ktd2027.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_idf_lib_helpers.h>
#include "math.h"

#define I2C_FREQ_HZ 1000000 // 1MHz

static const char *TAG = "ktd2027";

#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

inline static esp_err_t write_register(ktd2027_t *dev, uint8_t reg, uint8_t val)
{
    return i2c_dev_write_reg(&dev->i2c_dev,reg, &val, 1);
}

inline static esp_err_t read_register(ktd2027_t *dev, uint8_t reg, uint8_t *val)
{
    return i2c_dev_read_reg(&dev->i2c_dev, reg, val, 1);
}

/**
 * @brief Initialize device descriptor
 *
 * @param dev Device descriptor
 * @param port I2C port
 * @param sda_gpio SDA GPIO pin
 * @param scl_gpio SCL GPIO pin
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_init_desc(ktd2027_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    CHECK_ARG(dev);

    dev->i2c_dev.port = port;
    dev->i2c_dev.addr = KTD2027_I2C_ADDR;
    dev->i2c_dev.cfg.sda_io_num = sda_gpio;
    dev->i2c_dev.cfg.scl_io_num = scl_gpio;
#if HELPER_TARGET_IS_ESP32
    dev->i2c_dev.cfg.master.clk_speed = I2C_FREQ_HZ;
#endif

    return i2c_dev_create_mutex(&dev->i2c_dev);
}
/**
 * @brief Free device descriptor
 *
 * @param dev Device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_free_desc(ktd2027_t *dev)
{
    CHECK_ARG(dev);
    return i2c_dev_delete_mutex(&dev->i2c_dev);
}
/**
 * @brief Initialize device
 *
 * @param dev Device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_init(ktd2027_t *dev)
{
    CHECK_ARG(dev);
 //   I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
 //   I2C_DEV_CHECK(&dev->i2c_dev, enable(dev));

    /* software reset and enable */
     I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, KTD2027_REG_EN_RST, 2));

    /* Set all led brightness to max */
     I2C_DEV_CHECK(&dev->i2c_dev,write_register(dev, KTD2027_REG_RED_LED, MAX_BRIGHTNESS_VALUE));
     I2C_DEV_CHECK(&dev->i2c_dev,write_register(dev, KTD2027_REG_GREEN_LED, MAX_BRIGHTNESS_VALUE));
     I2C_DEV_CHECK(&dev->i2c_dev,write_register(dev, KTD2027_REG_BLUE_LED, MAX_BRIGHTNESS_VALUE));

     /* Reset all led at init */
     I2C_DEV_CHECK(&dev->i2c_dev,write_register(dev,KTD2027_REG_LED_EN , LED_OFF));

 //   I2C_DEV_CHECK(&dev->i2c_dev, disable(dev));
 //   I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);

     return ESP_OK;
}
/**
 * @brief Set RGB LED
 *
 * @param dev Device descriptor
 * @param uint8_t color
 * @param uint8_t mode
 * @param uint16_t duration
 * @return `ESP_OK` on success
 */
esp_err_t ktd2027_led_set(ktd2027_t *dev,uint8_t color, uint8_t mode,uint16_t duration)
{
	CHECK_ARG(dev && color && mode && duration);

	switch (mode)
	{
	case RGB_LED_MODE_OFF:

		I2C_DEV_CHECK(&dev->i2c_dev,write_register(dev, KTD2027_REG_LED_EN, LED_OFF));
		break;

	case RGB_LED_MODE_ON:

		if (color == RGB_LED_COLOR_RED)
		{
			I2C_DEV_CHECK(&dev->i2c_dev,write_register(&dev, KTD2027_REG_LED_EN, KTD2027_RED_LED_EN));
		}
		else if (color == RGB_LED_COLOR_GREEN)
		{
			I2C_DEV_CHECK(&dev->i2c_dev,write_register(dev, KTD2027_REG_LED_EN, KTD2027_GREEN_LED_EN));
		}
		else if (color == RGB_LED_COLOR_BLUE)
		{
			I2C_DEV_CHECK(&dev->i2c_dev,write_register(dev, KTD2027_REG_LED_EN, KTD2027_BLUE_LED_EN));
		}
		break;
	case RGB_LED_MODE_BLINK:
		// code for RGB_LED_MODE_BLINK
		break;
	default:
		// code for default case
		break;
	}
	return ESP_OK;
}
