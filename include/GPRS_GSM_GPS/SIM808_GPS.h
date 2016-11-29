/*
 * SIM808_GPS.h
 *
 *  Created on: 2016. szept. 8.
 *      Author: Bence
 */

#ifndef GPRS_GSM_GPS_SIM808_GPS_H_
#define GPRS_GSM_GPS_SIM808_GPS_H_

/*
 * AT+CGPSPWR    GPS Power Control
 * AT+CGPSRST    GPS Reset Mode (COLD/HOT/WARM)
 * AT+CGPSINF    Get Current GPS Location Info
 * AT+CGPSOUT    GPS NMEA Data Output Control
 * AT+CGPSSTATUS GPS Status
 *
 *            OR
 *
 * AT+CGNSPWR GNSS power control
 * AT+CGNSSEQ Define the last NMEA sentence that parsed
 * AT+CGNSINF GNSS navigation information parsed from NMEA sentences
 * AT+CGNSURC GNSS navigation, GEO-fences and speed alarm URC report
 * AT+CGNSCMD Send command to GNSS
 * AT+CGNSTST Send data received from GNSS to AT UART
 */

#include "types.h"

typedef struct GPS_data
{
  boolean_t valid;

  float longitude;
  float latitude;
  float speed_over_ground;
}GPS_data_t;

void enable_GPS(boolean_t enable);
GPS_data_t get_data_GPS(void);
boolean_t get_state_GPS(void);

#endif /* GPRS_GSM_GPS_SIM808_GPS_H_ */
