/*
 * SIM808_GSM.h
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#ifndef GPRS_GSM_GPS_SIM808_GSM_H_
#define GPRS_GSM_GPS_SIM808_GSM_H_

#include "types.h"

/*
 *  Sends an SMS with the given text to the given number
 *
 *  @param text: text to be sent max 160 byte
 *  @param number: addressee's phone number
 *
 */
boolean_t send_SMS(char* text, char* number);

#endif /* GPRS_GSM_GPS_SIM808_GSM_H_ */
