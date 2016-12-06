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

extern boolean_t diagnostic;

typedef enum power_saving_mode
{
  SLEEP  = 0,
  STOP = 1,
  STANDBY = 2
}power_saving_mode_t;

void enable_4V2_converter(boolean_t val);
void enable_bat_charger(boolean_t val);

/*
 *  returns status of MCP73832 LiPo charger
 *
 *  return: True:   Shutdown, No Battery Present, Charger Complete - Standby
 *          False: Preconditioning, Constant-Current Fast Charge, Constant Voltage
 */
boolean_t r_charger_stat(void);
void power_mngt_init(void);
uint16_t calculate_MCU_vcc(void);
uint16_t r_battery_voltage(uint16_t mcu_vcc);
uint16_t r_supply_current(uint16_t mcu_vcc);
float r_MCU_temp(uint16_t mcu_vcc);
void enter_mode(power_saving_mode_t mode);
void charge_control(uint16_t vbat);

#endif /* POWER_MANAGEMENT_H_ */
