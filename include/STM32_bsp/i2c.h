/*
 * i2c.h
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f0xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

void MX_I2C1_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */
