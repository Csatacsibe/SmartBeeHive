/*
 * SIM808_driver.h
 *
 *  Created on: 2016. aug. 11.
 *      Author: Bence
 */

#ifndef SIM808_DRIVER_H_
#define SIM808_DRIVER_H_

#include "types.h"

typedef void (*SIM808_rx_callback_t)(char* data);

extern uint8_t rx_buffer_SIM808[200];
extern boolean_t rx_cmplt, rx_error;
extern uint8_t rx_cnt, cr_cnt, cr_limit;
extern SIM808_rx_callback_t rx_callback_SIM808;

uint16_t length(char* message);

/*
 *  Sets the default state of the RST and PWRKEY pins.
 */
void SIM808_init(void);

void enable_GPS_antenna(boolean_t val);

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
void get_s_SIM808(uint8_t cr_lmt, SIM808_rx_callback_t callback);

boolean_t is_powered_SIM808(void);

#endif /* SIM808_DRIVER_H_ */
