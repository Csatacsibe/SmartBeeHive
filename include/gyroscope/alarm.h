/*
 * alarm.h
 *
 *  Created on: 2016. nov. 26.
 *      Author: Bence
 */

#ifndef GYROSCOPE_ALARM_H_
#define GYROSCOPE_ALARM_H_

#include "types.h"

void configure_alarm(uint16_t threshold, uint16_t debounce_time);
void enable_alarm(boolean_t enable);
void identify_alarm_event(uint16_t sec);

boolean_t send_theft_alarm_msg(char* number);
boolean_t send_displacement_alarm_msg(char* number);

#endif /* GYROSCOPE_ALARM_H_ */
