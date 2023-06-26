/**
 * @file ltr303.c
 *
 * Copyright (c) Youcef BENAKMOUME
 *
 */

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_idf_lib_helpers.h>
#include "ltr303.h"
#include "math.h"

#define I2C_FREQ_HZ 1000000 // 1MHz

static const char *TAG = "ltr303";

static const float ltr303_gain[] = {
	1.0f,
	2.0f,
	4.0f,
	8.0f,
	0.0f,
	0.0f,
	48.0f,
	96.0f,
};
static float ltr303_integration_time[] = {
	1.0f,
	0.5f,
	2.0f,
	4.0f,
	1.5f,
	2.5f,
	3.0f,
	3.5f,
};

#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

inline static esp_err_t write_register(ltr303_t *dev, uint8_t reg, uint8_t val)
{
    return i2c_dev_write_reg(&dev->i2c_dev,reg, &val, 1);
}

inline static esp_err_t read_register(ltr303_t *dev, uint8_t reg, uint8_t *val)
{
    return i2c_dev_read_reg(&dev->i2c_dev, reg, val, 1);
}

inline static esp_err_t read_register_16(ltr303_t *dev, uint8_t reg, uint16_t *val)
{
    return i2c_dev_read_reg(&dev->i2c_dev, reg, val, 2);
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
esp_err_t ltr303_init_desc(ltr303_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    CHECK_ARG(dev);

    dev->i2c_dev.port = port;
    dev->i2c_dev.addr = LTR303_I2C_ADDR;
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
esp_err_t ltr3031_free_desc(ltr303_t *dev)
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
esp_err_t ltr303_init(ltr303_t *dev)
{
    CHECK_ARG(dev);
 //   I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
 //   I2C_DEV_CHECK(&dev->i2c_dev, enable(dev));

    /* software reset */
     I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, LTR303_CONTR, LTR303_RESET));

    /* active mode and set gain*/
    I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, LTR303_CONTR, (LTR303_GAIN << 2) | LTR303_ACTIVE));

    /* set measure rate and integration time*/
	I2C_DEV_CHECK(&dev->i2c_dev, write_register(dev, LTR303_MEAS_RATE, (LTR303_INTEGRATION_TIME << 3) | LTR303_MEASUREMENT_RATE));

    uint8_t id;
    I2C_DEV_CHECK(&dev->i2c_dev, read_register(dev, LTR303_PART_ID, &id));
    id >>= 4;

    if (id == 10){
        ESP_LOGI(TAG, "Detected right pard id for LTR303 sensor: %u", id);
        return ESP_OK;
    }
    else {
        ESP_LOGW(TAG, "Unknown part id for LTR303 sensor: %u", id);
        return ESP_FAIL;
    }

 //   I2C_DEV_CHECK(&dev->i2c_dev, disable(dev));
 //   I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);

}
/* Gets the 16-bit channel 0 and channel 1 data
* Default value of both channels is 0x00
* Returns true (1) if successful, false (0) if there was an I2C error
* (Also see getError() below)
*/
esp_err_t ltr303_sample_fetch(ltr303_t *dev,uint16_t *CH0_data_raw, uint16_t *CH1_data_raw)
{
	uint16_t channel0_data = 0;
	uint16_t channel1_data = 0;

	CHECK_ARG(dev && CH0_data_raw && CH1_data_raw );
//    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);
//    I2C_DEV_CHECK(&dev->i2c_dev, enable(dev));

    // Read channel 1 and channel 2 ADC values
    I2C_DEV_CHECK(&dev->i2c_dev, read_register(dev, LTR303_DATA_CH0_0, &channel0_data ));
    I2C_DEV_CHECK(&dev->i2c_dev, read_register(dev, LTR303_DATA_CH0_1, &channel1_data ));

    *CH0_data_raw = channel0_data;
	*CH1_data_raw = channel1_data;

//   I2C_DEV_CHECK(&dev->i2c_dev, disable(dev));
//   I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);

   return ESP_OK;
}
/*
*  Convert raw data to lux
* gain: 0 (1X) or 7 (96X), see getControl()
* integrationTime: integration time in ms, from getMeasurementRate()
* CH0, CH1: results from getData()
* lux will be set to resulting lux calculation
* returns true (1) if calculation was successful
* returns false (0) AND lux = 0.0 IF EITHER SENSOR WAS SATURATED (0XFFFF)
*/
esp_err_t ltr303_get_lux(ltr303_t *dev,uint16_t CH0_data_raw, uint16_t CH1_data_raw,float *lux)
{

	float ratio;

	ratio = (float)CH1_data_raw / ((float)CH0_data_raw + (float)CH1_data_raw);

	if (ratio < 0.45f)
		*lux = (1.7743f * (float)CH0_data_raw + 1.1059f * (float)CH1_data_raw) / ltr303_gain[LTR303_GAIN] / ltr303_integration_time[LTR303_INTEGRATION_TIME];
	else if (ratio < 0.64f)
		*lux = (4.2785f * (float)CH0_data_raw - 1.9548f * (float)CH1_data_raw) / ltr303_gain[LTR303_GAIN] / ltr303_integration_time[LTR303_INTEGRATION_TIME];
	else if (ratio < 0.85f)
		*lux = (0.5926f * (float)CH0_data_raw + 0.1185f * (float)CH1_data_raw) / ltr303_gain[LTR303_GAIN] / ltr303_integration_time[LTR303_INTEGRATION_TIME];
	else
		*lux = 0.f;

    return ESP_OK;
}
