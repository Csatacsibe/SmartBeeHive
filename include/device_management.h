/*
 * device_management.h
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

#ifndef DEVICE_MANAGEMENT_H_
#define DEVICE_MANAGEMENT_H_

#include "types.h"

typedef enum led_state
{
  OFF = 0,
  ON  = 1,
  TOGGLE = 2
}led_state_t;

void init_BSP(void);

void init_board(void);

/******************************************************************/

boolean_t waitFor(boolean_t* flag, uint32_t timeout);

void ext_LED(led_state_t state);

boolean_t r_push_button(void);

boolean_t get_switch_state(void);

void toggle_switch_state(void);

void measure_RTC_1min(void);

uint32_t get_RTC_min_val(void);

float round_to(float val, uint8_t frac);

#endif /* DEVICE_MANAGEMENT_H_ */
