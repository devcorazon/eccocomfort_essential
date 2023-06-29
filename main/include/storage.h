/*
 * storage.h
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#ifndef SRC_INCLUDE_STORAGE_H_
#define SRC_INCLUDE_STORAGE_H_

#include "structs.h"

int storage_init(void);
int storage_set_default(void);

/// runtime data
int16_t get_temperature(void);
int set_temperature(int16_t temperature);

uint16_t get_relative_humidity(void);
int set_relative_humidity(uint16_t relative_humidity);

/// configuration settings
uint8_t get_mode_set(void);
int set_mode_set(uint8_t mode_set);

#endif /* SRC_INCLUDE_STORAGE_H_ */
