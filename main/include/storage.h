/*
 * storage.h
 *
 *  Created on: 1 dic 2022
 *      Author: Daniele Schirosi
 */

#ifndef SRC_INCLUDE_STORAGE_H_
#define SRC_INCLUDE_STORAGE_H_

#include <string.h>
#include "structs.h"
#include "storage_internal.h"

int storage_init(void);
int storage_set_default(void);

/// runtime data
void set_temperature(int16_t temperature);
int16_t get_temperature(void);

void set_relative_humidity(int16_t relative_humidity);
uint16_t get_relative_humidity(void);

void set_voc(int32_t voc);
int32_t get_voc(void);

void set_lux(int16_t lux);
int16_t get_lux(void);


/// configuration settings
uint8_t get_mode_set(void);
int set_mode_set(uint8_t mode_set);

#endif /* SRC_INCLUDE_STORAGE_H_ */
