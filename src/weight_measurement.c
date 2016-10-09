/*
 * weight_measurement.c
 *
 *  Created on: 2016. szept. 26.
 *      Author: Bence
 */

#include <weight_measurement.h>
#include <power_management.h>
#include <STM32_bsp/adc.h>
#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>

static uint16_t unloaded = 0;

uint16_t r_wheatstone_bridge()
{
  float output;
  uint16_t mcu_vdd = calculate_MCU_Vdd();
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(STRAIN_GAUGE);
  output = (mcu_vdd/4095.0) * digital_val;

  return output;
}

uint16_t process_bridge_output(uint16_t value)
{
  int16_t temp;
  temp = value - unloaded;

  if(temp < 0)
  {
    temp = 0;
  }

  return temp;
}

void calibrate_scale()
{
  unloaded = r_wheatstone_bridge();
}


