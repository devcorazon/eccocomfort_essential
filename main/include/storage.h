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
#include "nvs_flash.h"

esp_err_t storage_init(void);
int storage_set_default(void);

/// runtime data
void set_serial_number(uint32_t serial_number);
uint32_t get_serial_number(void);

uint16_t get_fw_version(void);

void set_mode_state(uint8_t mode_state);
uint8_t get_mode_state(void);

void set_speed_state(uint8_t speed_state);
uint8_t get_speed_state(void);

void set_direction_state(uint8_t direction_state);
uint8_t get_direction_state(void);

uint8_t get_device_state(void);
void set_device_state(uint8_t device_state);

void set_temperature(int16_t temperature);
int16_t get_temperature(void);

void set_relative_humidity(uint16_t relative_humidity);
uint16_t get_relative_humidity(void);

void set_voc(int32_t voc);
int32_t get_voc(void);

void set_lux(int16_t lux);
int16_t get_lux(void);

int16_t get_ntc_temperature(void);
void set_ntc_temperature(int16_t ntc_temperature);

/// configuration settings
uint8_t get_relative_humidity_set(void);
esp_err_t set_relative_humidity_set(uint8_t relative_humidity_set);

uint8_t get_lux_set(void);
esp_err_t set_lux_set(uint8_t lux_set);

uint8_t get_voc_set(void);
esp_err_t set_voc_set(uint8_t voc_set);

uint16_t get_temperature_offset(void);
esp_err_t set_temperature_offset(uint16_t temperature_offset);

uint16_t get_relative_humidity_offset(void);
esp_err_t set_relative_humidity_offset(uint16_t relative_humidity_offset);

esp_err_t set_mode_set(uint8_t mode_set);
uint8_t get_mode_set(void);

esp_err_t set_speed_set(uint8_t speed_set);
uint8_t get_speed_set(void);

#endif /* SRC_INCLUDE_STORAGE_H_ */
