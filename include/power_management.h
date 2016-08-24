/*
 * power_management.h
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#ifndef POWER_MANAGEMENT_H_
#define POWER_MANAGEMENT_H_

#define VREFINT_CAL_ADDR   0x1FFFF7BA

#include "types.h"

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
void batt_charger_set(boolean_t val);

/*
 *  returns status of MCP73832 LiPo charger
 *
 *  return: GPIO_PIN_SET:   Shutdown, No Battery Present, Charger Complete - Standby
 *          GPIO_PIN_RESET: Preconditioning, Constant-Current Fast Charge, Constant Voltage
 */
GPIO_PinState get_charger_stat(void);

/*
 *  set default pin values
 */
void power_mngt_init(void);

/*
 * Reads the internal voltage reference,
 * the internal reference voltage calibration
 * value and calculates the supply voltage in Volts.
 */
float calculate_MCU_Vdd(void);

/*
 * Returns the battery voltage in Volts.
 */
float r_battery_voltage(void);

/*
 * Returns the board supply current in mA.
 */
float r_supply_current(void);

#endif /* POWER_MANAGEMENT_H_ */
