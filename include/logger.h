/*
 * logger.h
 *
 *  Created on: 2016. nov. 25.
 *      Author: Bence
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "types.h"

typedef struct hive_data
{
  float    humidity;
  float    temperature;
  uint32_t mass;
}hive_data_t;

typedef struct device
{
  uint16_t vbat;
  uint16_t current;

  boolean_t charger;

  uint16_t MCU_vcc;
  int8_t MCU_temperature;

  uint16_t SIM808_vcc;
  float SIM808_temperature;

  int8_t GYRO_temperature;
}device_t;

extern device_t device;

void update_state_logger(void);
void refresh_device_data(void);
void log_hive_data(uint8_t log_cycle);
void create_packet(char* packet);
void send_records(void);

#endif /* LOGGER_H_ */
