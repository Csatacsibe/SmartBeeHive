/*
 * SIM808_driver.h
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#ifndef SIM808_DRIVER_H_
#define SIM808_DRIVER_H_

#include "types.h"

typedef void (*callback_t)(void);

extern uint8_t rx_buffer_SIM808[160];
extern boolean_t rx_cmplt, rx_error;
extern uint8_t rx_cnt, cr_cnt, cr_limit;
extern callback_t *rx_callback;

uint16_t length(char* message);

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
 *  Drives RST low for 150 milliseconds to reset the module.
 */
void reset_SIM808(void);

/*
 *  Sends a single byte to the module.
 */
void put_c_SIM808(uint8_t c);

/*
 * Receives one character in blocking mode.
 */
uint8_t get_c_SIM808(void);

/*
 *  Sends a string to the module, returns sent byte number.
 */
uint16_t put_s_SIM808(char* string);

/*
 * Calls UART_receive_IT
 *
 * Params:
 * @cr_lmt: number of '\r' to be received before rx complete
 * @callback: function to be called to process
 */
void get_s_SIM808(uint8_t cr_lmt, callback_t *callback);

/*
 *  Sends default configuration if the module is powered.
 */
void configure_SIM808(void);

boolean_t is_powered_SIM808(void);

#endif /* SIM808_DRIVER_H_ */
