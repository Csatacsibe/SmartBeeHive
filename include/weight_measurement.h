/*
 * weight_measurement.h
 *
 *  Created on: 2016. szept. 26.
 *      Author: Bence
 */

#ifndef WEIGHT_MEASUREMENT_H_
#define WEIGHT_MEASUREMENT_H_

#include "types.h"

/*
 *  Return the output votlage of the strain gauge Wheatstone bridge in milivolts.
 */
uint16_t r_wheatstone_bridge(void);

uint16_t process_bridge_output(uint16_t value);

void calibrate_scale(void);

#endif /* WEIGHT_MEASUREMENT_H_ */
