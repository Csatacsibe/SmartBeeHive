/*
 * adc.h
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include "types.h"

#define STRAIN_GAUGE    ADC_CHANNEL_4
#define SUPPLY_CURRENT  ADC_CHANNEL_5
#define BATTERY_VOLTAGE ADC_CHANNEL_6

extern ADC_HandleTypeDef hadc;

void MX_ADC_Init(void);

void config_ext_channel_ADC(uint32_t channel, boolean_t val);
uint32_t r_single_ext_channel_ADC(uint32_t channel);

void config_int_channel_ADC(uint32_t channel, boolean_t val);
uint32_t r_single_int_channel_ADC(uint32_t channel);

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
