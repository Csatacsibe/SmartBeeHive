/*
 * SIM808_GPRS.h
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#ifndef GPRS_GSM_GPS_SIM808_GPRS_H_
#define GPRS_GSM_GPS_SIM808_GPRS_H_

#include <types.h>

void init_GPRS(void);
boolean_t check_sig_strength_SIM808(void);
boolean_t connect_GPRS(uint32_t timeout);
boolean_t download_data_GPRS(char* data, uint32_t timeout);
boolean_t disconnect_GPRS(uint32_t timeout);

#endif /* GPRS_GSM_GPS_SIM808_GPRS_H_ */
