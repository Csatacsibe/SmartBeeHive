/*
 * device_management.c
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

#include "device_management.h"


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
