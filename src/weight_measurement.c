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

uint16_t r_wheatstone_bridge()
{
  float output;
  uint16_t mcu_vdd = calculate_MCU_Vdd();
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(STRAIN_GAUGE);
  output = (mcu_vdd/4095.0) * digital_val;

  return output;
}

