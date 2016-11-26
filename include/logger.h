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
  uint16_t MCU_vcc;
  uint16_t SIM808_vcc;

  int8_t MCU_temperature;
  int8_t GYRO_temperature;
  float SIM808_temperature;

}device_t;

extern device_t device;

void update_state_logger(void);
void logger_state_machine(void);
void refresh_device_data(void);

#endif /* LOGGER_H_ */
