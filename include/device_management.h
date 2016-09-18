/*
 * device_management.h
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

#ifndef DEVICE_MANAGEMENT_H_
#define DEVICE_MANAGEMENT_H_

#include "types.h"

/*
 *  Waits for a flag to be set
 *
 *  Params:
 *  flag: flag to be checked
 *  timeout: timeout value in milis
 *
 *  retval: False if timeout occurred
 */
boolean_t waitFor(boolean_t* flag, uint32_t timeout);

/*
 * SET, RESET, TOGGLE external LED
 *
 */
void ext_LED(uint8_t state);

/*
 * Read external button state
 */
boolean_t r_push_button(void);

/*
 * Returns the switch state private
 * variable
 */
boolean_t get_switch_state(void);

/*
 * Toggles the switch state private
 * variable
 */
void toggle_switch_state(void);

/*
 * Measures an average 1 minute of the RTC using SysTick
 * retval is available in rtc_min_val
 */
void measure_RTC_1min(void);

/*
 * Returns the value of rtc_min_val variable
 */
uint32_t get_rtc_min_val(void);

#endif /* DEVICE_MANAGEMENT_H_ */
