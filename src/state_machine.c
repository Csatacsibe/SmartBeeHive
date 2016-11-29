/*
 * state_machine.c
 *
 *  Created on: 2016. nov. 27.
 *      Author: Bence
 */

#include "state_machine.h"

smart_beehive_states_t state_SBH = DEBUG_MONITORING;

static uint8_t wake_up_cnt = 0;

void init_state_machine()
{
  wake_up_cnt = 0;
  state_SBH = DEBUG_MONITORING;//LOGGING;
}

void inc_wake_up_counter()
{
  wake_up_cnt++;
}

uint8_t get_wake_up_cycle()
{
  return wake_up_cnt;
}

void reset_wake_up_counter()
{
  if(wake_up_cnt == 10)
  {
    wake_up_cnt = 0;
  }
}


