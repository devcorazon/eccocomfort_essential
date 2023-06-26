/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#ifndef __LTR303_H__
#define __LTR303_H__

#include <stdint.h>
#include <stdbool.h>
#include <i2cdev.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LTR303_I2C_ADDR 0x29

// LTR303 register addresses
#define LTR303_CONTR         0x80
#define LTR303_MEAS_RATE     0x85
#define LTR303_PART_ID       0x86
#define LTR303_MANUFAC_ID    0x87
#define LTR303_DATA_CH1_0    0x88
#define LTR303_DATA_CH1_1    0x89
#define LTR303_DATA_CH0_0    0x8A
#define LTR303_DATA_CH0_1    0x8B
#define LTR303_STATUS		 0x8C
#define LTR303_INTERRUPT     0x8F
#define LTR303_THRES_UP_0    0x97
#define LTR303_THRES_UP_1	 0x98
#define LTR303_THRES_LOW_0   0x99
#define LTR303_THRES_LOW_1   0x9A
#define LTR303_INTR_PERS     0x9E

#define LTR303_ACTIVE					0x01
#define LTR303_RESET					0x02


#define CONFIG_LTR303_GAIN_96X
#define CONFIG_LTR303_MEASUREMENT_RATE_500MS
#define CONFIG_LTR303_INTEGRATION_TIME_250MS

#if defined CONFIG_LTR303_GAIN_1X
	#define LTR303_LUX_RANGE			f64000
	#define LTR303_GAIN					0x00
#elif defined CONFIG_LTR303_GAIN_2X
	#define LTR303_LUX_RANGE			32000
	#define LTR303_GAIN					0x01
#elif defined CONFIG_LTR303_GAIN_4X
	#define LTR303_LUX_RANGE			16000
	#define LTR303_GAIN					0x02
#elif defined CONFIG_LTR303_GAIN_8X
	#define LTR303_LUX_RANGE			8000
	#define LTR303_GAIN					0x03
#elif defined CONFIG_LTR303_GAIN_48X
	#define LTR303_LUX_RANGE			1300
	#define LTR303_GAIN					0x06
#elif defined CONFIG_LTR303_GAIN_96X
	#define LTR303_LUX_RANGE			600
	#define LTR303_GAIN					0x07
#endif

#if defined CONFIG_LTR303_MEASUREMENT_RATE_50MS
	#define LTR303_MEASUREMENT_RATE		0x00
#elif defined CONFIG_LTR303_MEASUREMENT_RATE_100MS
	#define LTR303_MEASUREMENT_RATE		0x01
#elif defined CONFIG_LTR303_MEASUREMENT_RATE_200MS
	#define LTR303_MEASUREMENT_RATE		0x02
#elif defined CONFIG_LTR303_MEASUREMENT_RATE_500MS
	#define LTR303_MEASUREMENT_RATE		0x03
#elif defined CONFIG_LTR303_MEASUREMENT_RATE_1000MS
	#define LTR303_MEASUREMENT_RATE		0x04
#elif defined CONFIG_LTR303_MEASUREMENT_RATE_2000MS
	#define LTR303_MEASUREMENT_RATE		0x05
#endif

#if defined CONFIG_LTR303_INTEGRATION_TIME_50MS
	#define LTR303_INTEGRATION_TIME		0x01
#elif defined CONFIG_LTR303_INTEGRATION_TIME_100MS
	#define LTR303_INTEGRATION_TIME		0x00
#elif defined CONFIG_LTR303_INTEGRATION_TIME_150MS
	#define LTR303_INTEGRATION_TIME		0x04
#elif defined CONFIG_LTR303_INTEGRATION_TIME_200MS
	#define LTR303_INTEGRATION_TIME		0x02
#elif defined CONFIG_LTR303_INTEGRATION_TIME_250MS
	#define LTR303_INTEGRATION_TIME		0x05
#elif defined CONFIG_LTR303_INTEGRATION_TIME_300MS
	#define LTR303_INTEGRATION_TIME		0x06
#elif defined CONFIG_LTR303_INTEGRATION_TIME_350MS
	#define LTR303_INTEGRATION_TIME		0x07
#elif defined CONFIG_LTR303_INTEGRATION_TIME_400MS
	#define LTR303_INTEGRATION_TIME		0x03
#endif

/**
 * Device descriptor
 */
typedef struct {
    i2c_dev_t i2c_dev;
} ltr303_t;

/**
 * @brief Initialize device descriptor
 *
 * @param dev Device descriptor
 * @param port I2C port
 * @param sda_gpio SDA GPIO pin
 * @param scl_gpio SCL GPIO pin
 * @return `ESP_OK` on success
 */
esp_err_t ltr303_init_desc(ltr303_t *dev,i2c_port_t port,gpio_num_t sda_gpio, gpio_num_t scl_gpio);

/**
 * @brief Free device descriptor
 *
 * @param dev Device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t ltr303_free_desc(ltr303_t *dev);

/**
 * @brief Initialize device
 *
 * @param dev Device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t ltr303_init(ltr303_t *dev);

/*
* Gets the 16-bit channel 0 and channel 1 data
* Default value of both channels is 0x00
* Returns true (1) if successful, false (0) if there was an I2C error
* (Also see getError() below)
*/
esp_err_t ltr303_sample_fetch(ltr303_t *dev,uint16_t *CH0_data_raw, uint16_t *CH1_data_raw);

/*
*  Convert raw data to lux
* gain: 0 (1X) or 7 (96X), see getControl()
* integrationTime: integration time in ms, from getMeasurementRate()
* CH0, CH1: results from getData()
* lux will be set to resulting lux calculation
* returns true (1) if calculation was successful
* returns false (0) AND lux = 0.0 IF EITHER SENSOR WAS SATURATED (0XFFFF)
*/
esp_err_t ltr303_get_lux(ltr303_t *dev,uint16_t CH0_data_raw, uint16_t CH1_data_raw,float *lux);


#ifdef __cplusplus
}
#endif

/**@}*/

#endif  // __TSL4531_H__
