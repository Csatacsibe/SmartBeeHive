/*
 * power_management.h
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#ifndef POWER_MANAGEMENT_H_
#define POWER_MANAGEMENT_H_

#define VREFINT_CAL_ADDR   ((uint16_t*) ((uint32_t) 0x1FFFF7BA))
#define TEMP30_CAL_ADDR    ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define TEMP110_CAL_ADDR   ((uint16_t*) ((uint32_t) 0x1FFFF7C2))

#include "types.h"

typedef enum power_saving_mode
{
  SLEEP  = 1,
  STOP = 2,
  STANDBY = 3
}power_saving_mode_t;

/*
 *  enable/disable ADP1613 DCDC converter
 *
 *  params:
 *  val: True  = enable
 *       False = disable
 */
void _4V2_converter_set(boolean_t val);

/*
 *  enable/disable MCP73832 LiPo charger
 *
 *  params:
 *  val: True  = enable
 *       False = disable
 */
void enable_bat_charger(boolean_t val);

/*
 *  returns status of MCP73832 LiPo charger
 *
 *  return: True:   Shutdown, No Battery Present, Charger Complete - Standby
 *          False: Preconditioning, Constant-Current Fast Charge, Constant Voltage
 */
boolean_t get_charger_stat(void);

/*
 *  set default pin values
 */
void power_mngt_init(void);

/*
 * Reads the internal voltage reference,
 * the internal reference voltage calibration
 * value and calculates the supply voltage in milivolts.
 */
uint16_t calculate_MCU_Vdd(void);

/*
 * Returns the battery voltage in milivolts.
 */
uint16_t r_battery_voltage(void);

/*
 * Returns the board supply current in mA.
 */
uint16_t r_supply_current(void);

/*
 * Reads the MCU internal temperature sensor.
 * Returns the temperature in °C.
 */
float r_MCU_temp(void);

/*
 * The MCU enters the power saving mode selected by @param mode.
 */
void enter_mode(power_saving_mode_t mode);

#endif /* POWER_MANAGEMENT_H_ */
