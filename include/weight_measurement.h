/*
 * weight_measurement.h
 *
 *  Created on: 2016. szept. 26.
 *      Author: Bence
 */

#ifndef WEIGHT_MEASUREMENT_H_
#define WEIGHT_MEASUREMENT_H_

#include "types.h"

extern boolean_t scale_sampling_done = False;

typedef void (*scale_callback_t)(void);

/*
 * Set the null point and initialize the averaging filter's buffer
 */
void scale_init();

/*
 * Returns the mass in g, input is a processed value in mV
 */
uint32_t measure_mass(uint16_t mcu_vcc);
uint32_t r_hive_mass(void);
uint16_t start_scale_sampling(uint16_t mcu_vcc);

#endif /* WEIGHT_MEASUREMENT_H_ */
