/*
 * device_management.c
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

#include "device_management.h"


static volatile boolean_t switch_state = False;
static volatile uint32_t rtc_min_val = 0;

/*
 * Local variables without getter
 */
static volatile uint32_t prev_tick = 0;
static volatile uint32_t rtc_min_cntr = 0;

boolean_t waitFor(boolean_t* flag, uint32_t timeout)
{
  uint32_t tickstart = HAL_GetTick();

  while(*flag != True)
  {
    if((timeout == 0) || ((HAL_GetTick() - tickstart) > timeout))
    {
      return False;
    }
  }

  *flag = False;

  return True;
}

void ext_LED(led_state_t state)
{
  if(state == 0)
  {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  }
  else if(state == 1)
  {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }
  else if(state == 2)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
  }
}

boolean_t r_push_button()
{
  uint32_t pushed = 0, not_pushed = 0;

  uint32_t i;

  for (i = 0; i < 10000; i++)
  {
    if (HAL_GPIO_ReadPin(TACT_BUTTON_GPIO_Port, TACT_BUTTON))
    {
      pushed++;
    }
    else
    {
      not_pushed++;
    }
  }

  if (pushed > 8000)
  {
    return True;
  }

  return False;
}

boolean_t get_switch_state()
{
  return switch_state;
}

void toggle_switch_state()
{
  switch(switch_state)
  {
    case True:  switch_state = False;
      break;
    case False: switch_state = True;
      break;
    default:
      break;
  }
}

void measure_RTC_1min()
{
  if(rtc_min_cntr < 10)
  {
    uint32_t tick_now = HAL_GetTick();
    rtc_min_val += (tick_now - prev_tick);
    prev_tick = tick_now;
    rtc_min_cntr++;

    if(rtc_min_cntr == 10)
    {
       rtc_min_val = rtc_min_val/10;
    }
  }
}

uint32_t get_RTC_min_val()
{
  return rtc_min_val;
}

