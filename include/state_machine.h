/*
 * state_machine.h
 *
 *  Created on: 2016. nov. 27.
 *      Author: Bence
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "types.h"

#define DEBUG_VERSION
#define LOG_PERIOD 10

typedef enum smart_beehive_states
{
  LOGGING            = 0, // sleep, log and upload, no problemo
  ALARM_RAISED       = 1, // INTR received from gyro decide what happened ...
  ALARM_THEFT        = 2, // ... theft happened
  ALARM_DISPLACEMENT = 3, // ... displacement happened due to weather or other reason
  DEBUG_MONITORING   = 4  // do not sleep, do continuous measurements to be monitored by STMStudio
}smart_beehive_states_t;

extern smart_beehive_states_t state_SBH;

void init_state_machine(void);
void inc_wake_up_counter(void);
uint8_t get_wake_up_cycle(void);
void reset_wake_up_counter(void);


#endif /* STATE_MACHINE_H_ */
