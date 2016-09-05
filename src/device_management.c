/*
 * device_management.c
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

#include "device_management.h"

static boolean_t switch_state = False;

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

void ext_LED(uint8_t state)
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
  uint8_t debounce[8];

  uint8_t i, check = 0;

  for (i = 0; i < 8; i++)
  {
    if (HAL_GPIO_ReadPin(TACT_BUTTON_GPIO_Port, TACT_BUTTON))
    {
      debounce[i] = 1;
    }
    else
    {
      debounce[i] = 0;
    }

    if (debounce[i] == 1)
    {
      check++;
    }

    HAL_Delay(20);
  }

  if (check == 8)
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
