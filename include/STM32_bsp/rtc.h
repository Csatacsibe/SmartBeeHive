/*
 * rtc.h
 *
 *  Created on: 2016. sept. 14.
 *      Author: Bence
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rtc_H
#define __rtc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

extern RTC_HandleTypeDef hrtc;

void MX_RTC_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ rtc_H */
