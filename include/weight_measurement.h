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
uint16_t r_wheatstone_bridge(uint16_t mcu_vcc);

uint16_t process_bridge_output(uint16_t value);

/*
 * Set the null point and initialize the averaging filter's buffer
 */
void scale_init();

float averaging_filter(uint16_t input);

/*
 * Returns the mass in g, input is a processed value in mV
 */
uint32_t mass_in_g(float input);

#endif /* WEIGHT_MEASUREMENT_H_ */
