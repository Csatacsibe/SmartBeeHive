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
 */

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

#endif /* GPRS_GSM_GPS_SIM808_DEVICE_H_ */
