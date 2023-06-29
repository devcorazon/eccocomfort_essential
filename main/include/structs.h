/*
 * structs.h
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#ifndef SRC_INCLUDE_STRUCTS_H_
#define SRC_INCLUDE_STRUCTS_H_

#include "types.h"

///
struct noinit_data_s {

};

struct runtime_data_s {
	int16_t		temperature;
	uint16_t	relative_humidity;
};

///
struct configuration_settings_s {
	uint8_t		mode_set;
	uint8_t		speed_set;

};

struct application_data_s {
	struct noinit_data_s				noinit_data;
	uint32_t							crc_noinit_data;
	struct runtime_data_s				runtime_data;
	struct configuration_settings_s		configuration_settings;

};

#endif /* SRC_INCLUDE_STRUCTS_H_ */
