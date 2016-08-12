/*
 * SIM808_driver.h
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#ifndef SIM808_DRIVER_H_
#define SIM808_DRIVER_H_

#include "constants.h"
#include "gpio.h"
#include "types.h"

// set default pin values
void SIM808_init(void);

/*
 *  power on/off GPS antenna
 *
 *  params:
 *  val: True  = power on
 *       False = power off
 */
void GPS_ant_pwr(boolean_t val);

/*
 *  Drives PWRKEY low for 2 second
 *  to turn ON/OFF the module
 */
void switch_SIM808(void);

/*
 *  Drives RST low for 150 milisecond
 *  to turn reset the module
 */
void reset_SIM808(void);

#endif /* SIM808_DRIVER_H_ */
