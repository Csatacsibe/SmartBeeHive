/*
 * SIM808_driver.h
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#ifndef SIM808_DRIVER_H_
#define SIM808_DRIVER_H_

#include "types.h"

extern uint8_t rx_buffer_SIM808[160];
extern boolean_t rx_cmplt;

/*
 *  Sets the default state of the RST and PWRKEY pins.
 */
void SIM808_init(void);

/*
 *  Power ON/OFF GPS antenna.
 *
 *  params:
 *  val: True  = power on
 *       False = power off
 */
void GPS_ant_pwr(boolean_t val);

/*
 *  Drives PWRKEY low for 1.1 second to turn ON/OFF the module.
 */
void power_SIM808(void);

/*
 *  Drives RST low for 150 miliseconds to reset the module.
 */
void reset_SIM808(void);

/*
 *  Sends a single byte to the module.
 */
void put_c_SIM808(uint8_t c);

/*
 *  Sends a string to the module.
 */
void put_s_SIM808(uint8_t* string);

/*
 *  Sends default configuration if the module is powered.
 */
void configure_SIM808(void);

#endif /* SIM808_DRIVER_H_ */
