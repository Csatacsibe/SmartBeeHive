/*
 * device_management.h
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

#ifndef DEVICE_MANAGEMENT_H_
#define DEVICE_MANAGEMENT_H_

#include "types.h"

typedef enum led_state
{
  OFF = 0,
  ON  = 1,
  TOGGLE = 2
}led_state_t;

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

  float MCU_temperature;
  float GYRO_temperature;
  float SIM808_temperature;

  boolean_t charger_status;
}device_t;

/*
 *  Waits for a flag to be set
 *
 *  Params:
 *  flag: flag to be checked
 *  timeout: timeout value in milis
 *
 *  retval: False if timeout occurred
 */
boolean_t waitFor(boolean_t* flag, uint32_t timeout);

/*
 * ON, OFF, TOGGLE external LED
 */
void ext_LED(led_state_t state);

/*
 * Read external button state
 */
boolean_t r_push_button(void);

/*
 * Returns the switch state private
 * variable
 */
boolean_t get_switch_state(void);

/*
 * Toggles the switch state private
 * variable
 */
void toggle_switch_state(void);

/*
 * Measures an average 1 minute of the RTC using SysTick
 * retval is available in rtc_min_val
 */
void measure_RTC_1min(void);

/*
 * Returns the value of rtc_min_val variable
 */
uint32_t get_RTC_min_val(void);

#endif /* DEVICE_MANAGEMENT_H_ */
