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

extern ADC_HandleTypeDef hadc;

void MX_ADC_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
