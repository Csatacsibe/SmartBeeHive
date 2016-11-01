/*
 * SIM808_device.h
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#ifndef GPRS_GSM_GPS_SIM808_DEVICE_H_
#define GPRS_GSM_GPS_SIM808_DEVICE_H_

#include "types.h"

/* List of possibly useful commands:
 *
 * AT+CANT Antenna Detecting
 * AT+CMTE Set Critical Temperature Operating Mode or Query Temperature
 * AT+SLEDS Set the Timer Period of Net Light
 * AT+CSGS Netlight Indication of GPRS Status
 * AT+CNETLIGHT Close the Net Light or Open It to Shining
 * AT+CWHITELIST Set the White List
 * AT+CSMINS SIM Inserted Status Reporting
 * AT+CIPGSMLOC GSM Location and Time
 */

void process_response_SIM808(void);

/*
 * AT+CNETLIGHT Close the Net Light or Open It to Shining
 *
 * @param value:
 *  0 Close the net light
 *  1 Open the net light to shining
 */
void set_netlight(uint8_t value);

/*
 *  Returns the Net Light status
 *
 *  @retval:
 *  0 Net light closed
 *  1 Net light opened to shining
 */
uint8_t get_netlight(void);

/*
 * Returns temperature value �C
 */
float r_temperature_SIM808(void);

/*
 * Queries temperature by sending the appropriate AT command
 */
void cmd_tmp_SIM808(void);

/*
 * Processes response of tmp_cmd
 */
void process_tmp_reading_SIM808(char* data);

/*
 * Returns vcc value in milivolts
 */
uint16_t r_vcc_SIM808(void);

/*
 * Queries vcc by sending the appropriate AT command
 */
void cmd_vcc_SIM808(void);

/*
 * Processes response of tmp_cmd
 */
void process_vcc_reading_SIM808(char* data);

#endif /* GPRS_GSM_GPS_SIM808_DEVICE_H_ */
