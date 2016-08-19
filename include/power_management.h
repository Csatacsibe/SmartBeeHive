/*
 * power_management.h
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#ifndef POWER_MANAGEMENT_H_
#define POWER_MANAGEMENT_H_

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

// set default pin values
void power_mngt_init(void);

#endif /* POWER_MANAGEMENT_H_ */
