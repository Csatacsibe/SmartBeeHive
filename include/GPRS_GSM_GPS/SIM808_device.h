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

/*
 * AT+CNETLIGHT Close the Net Light or Open It to Shining
 *
 * @param value:
 *  0 Close the net light
 *  1 Open the net light to shining
 */
void set_netlight(uint8_t value);

float r_temperature_SIM808(void);
void cmd_tmp_SIM808(void);
uint16_t r_vcc_SIM808(void);
void cmd_vcc_SIM808(void);

boolean_t is_SIM_registered_SIM808(void);

boolean_t check_msg_header_SIM808(char* data, char* header);

#endif /* GPRS_GSM_GPS_SIM808_DEVICE_H_ */
