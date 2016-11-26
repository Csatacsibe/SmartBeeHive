/*
 * device_management.c
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

/* STM32 peripherals */
#include <STM32_bsp/adc.h>
#include <STM32_bsp/gpio.h>
#include <STM32_bsp/i2c.h>
#include <STM32_bsp/usart.h>
#include <STM32_bsp/rtc.h>

#include <gyroscope/FXAS21002C_driver.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <power_management.h>
#include <weight_measurement.h>

#include "device_management.h"
#include "math.h"
#include "logger.h"

static volatile boolean_t switch_state = False;
static volatile uint32_t rtc_min_val = 0;

/*
 * Local variables without getter
 */
static volatile uint32_t prev_tick = 0;
static volatile uint32_t rtc_min_cntr = 0;

/***************************************************************************/
void init_BSP()
{
  MX_GPIO_Init();
  //MX_DMA_Init();
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
}

void init_board()
{
  power_mngt_init();
  SIM808_init();
  FXAS21002C_init();
  scale_init();

  refresh_device_data();
}

/***************************************************************************/

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
  switch_state = ((switch_state == True) ? False : True);
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

float round_to(float val, uint8_t frac)
{
    uint16_t divider = 0;

    switch(frac)
    {
        case 0:
            divider = 1;
            break;
        case 1:
            divider = 10;
            break;
        case 2:
            divider = 100;
            break;
        case 3:
            divider = 1000;
            break;
        default:
            divider = 10;
            break;
    }

    return roundf(val * (divider)) / (divider);
}

