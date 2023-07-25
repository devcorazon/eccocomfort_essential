/*
 * types.h
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#ifndef SRC_INCLUDE_TYPES_H_
#define SRC_INCLUDE_TYPES_H_

#define TEMPERATURE_INVALID						INT16_MAX
#define RELATIVE_HUMIDITY_INVALID				UINT16_MAX
#define VOC_INVALID								UINT16_MAX
#define LUX_INVALID								UINT16_MAX

#define TEMPERATURE_SCALE						(10)
#define RELATIVE_HUMIDITY_SCALE					(10u)
#define VOC_SCALE								(1u)
#define LUMINOSITY_SCALE						(1000u)

#define SET_VALUE_TO_TEMP_RAW(val)				(int16_t) (val * TEMPERATURE_SCALE)
#define SET_VALUE_TO_RH_RAW(val)				(uint16_t) (val * RELATIVE_HUMIDITY_SCALE)

#define TEMP_RAW_TO_INT(t)						(int16_t) (t / TEMPERATURE_SCALE)
#define TEMP_RAW_TO_DEC(t)						(int16_t) (abs((int) t) % TEMPERATURE_SCALE)

#define RH_RAW_TO_INT(rh)						(uint16_t) (rh / RELATIVE_HUMIDITY_SCALE)
#define RH_RAW_TO_DEC(rh)						(uint16_t) (rh % RELATIVE_HUMIDITY_SCALE)

// Rh threshold setting
enum {
    RH_THRESHOLD_SETTING_NOT_CONFIGURED			= 0x00,
    RH_THRESHOLD_SETTING_LOW					= 0x01,
    RH_THRESHOLD_SETTING_MEDIUM					= 0x02,
    RH_THRESHOLD_SETTING_HIGH					= 0x03,
};

// Luminosity sensor setting
enum {
    LUMINOSITY_SENSOR_SETTING_NOT_CONFIGURED	= 0x00,
    LUMINOSITY_SENSOR_SETTING_LOW				= 0x01,
    LUMINOSITY_SENSOR_SETTING_MEDIUM			= 0x02,
    LUMINOSITY_SENSOR_SETTING_HIGH				= 0x03,
};

// VOC threshold setting
enum {
	VOC_THRESHOLD_SETTING_NOT_CONFIGURED		= 0x00,
	VOC_THRESHOLD_SETTING_LOW					= 0x01,
	VOC_THRESHOLD_SETTING_MEDIUM				= 0x02,
	VOC_THRESHOLD_SETTING_HIGH					= 0x03,
};

// Mode
enum {
    MODE_OFF              						= 0x00,
    MODE_IMMISSION        						= 0x01,
    MODE_EMISSION         						= 0x02,
    MODE_FIXED_CYCLE      						= 0x03,
    MODE_AUTOMATIC_CYCLE  						= 0x04,
	MODE_AUTOMATIC_CYCLE_EXTRA_CYCLE			= BIT(6),
	MODE_AUTOMATIC_CYCLE_CALCULATE_DURATION		= BIT(7),
};

// Speed
enum {
    SPEED_NONE									= 0x00,
    SPEED_NIGHT									= 0x01,
    SPEED_LOW									= 0x02,
    SPEED_MEDIUM								= 0x03,
    SPEED_HIGH									= 0x04,
    SPEED_BOOST									= 0x05,
	SPEED_AUTOMATIC_CYCLE_FORCE_BOOST			= BIT(6),
	SPEED_AUTOMATIC_CYCLE_FORCE_NIGHT			= BIT(7),
};

// Direction of rotation state
enum {
	DIRECTION_NONE								= 0x00,
	DIRECTION_OUT								= 0x01,
	DIRECTION_IN								= 0x02,
};

// Data Type
typedef enum {
    DATA_TYPE_UINT8,
    DATA_TYPE_INT8,
    DATA_TYPE_UINT16,
    DATA_TYPE_INT16,
    DATA_TYPE_UINT32,
    DATA_TYPE_INT32,
    DATA_TYPE_UINT64,
    DATA_TYPE_INT64,
    DATA_TYPE_STRING,
} data_type_e;

#endif /* SRC_INCLUDE_TYPES_H_ */
