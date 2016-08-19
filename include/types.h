/*
 * types.h
 *
 *  Created on: 2016. jún. 18.
 *      Author: Bence
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "stm32f0xx.h"

typedef enum boolean
{
	True  = 1,
	False = 0
}boolean_t;

typedef enum io_operation
{
	set_bit,
	reset_bit
}io_operation_t;

#endif /* TYPES_H_ */
