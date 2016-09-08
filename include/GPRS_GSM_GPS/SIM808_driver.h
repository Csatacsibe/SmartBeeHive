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
extern boolean_t check_in_isr;

/*
 *  Sets the default state of the RST and PWRKEY pins.
 */
void SIM808_init(void);

/*
 *  Power ON/OFF GPS antenna.
 *
 *  @param val:
 *       True  = power on
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
void put_c_SIM808(char c);

/*
 *  Sends a string to the module.
 */
void put_s_SIM808(char* string);

/*
 *  Sends default configuration if the module is powered.
 */
void configure_SIM808(void);

/*
 * Sends a message and check if response matches
 * the pattern.
 *
 * @param msg: string to be sent
 * @param pattern: text to be checked in the answer
 * @param length: expected response length
 * @param to: timeout value
 *
 * @retval:
 * False in case of mismatch or timeout
 * True in case of match
 */
boolean_t check_resp_SIM808(char* msg, char* pattern, uint8_t length, uint8_t to);

/*
 * Sends a message and waits until the response is received.
 *
 * @param msg: string to be sent
 * @param length: expected response length
 * @param to: timeout value
 *
 * @retval:
 * False in case timeout
 * True in case of rx complete
 */
boolean_t send_n_wait_for_resp(char* msg, uint8_t length, uint8_t to);

#endif /* SIM808_DRIVER_H_ */
