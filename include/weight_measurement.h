/*
 * weight_measurement.h
 *
 *  Created on: 2016. szept. 26.
 *      Author: Bence
 */

#ifndef WEIGHT_MEASUREMENT_H_
#define WEIGHT_MEASUREMENT_H_

#include "types.h"

typedef void (*scale_callback_t)(void);

extern scale_callback_t scale_callback;
extern boolean_t scale_sampling_done;

/*
 * Set the null point and initialize the averaging filter's buffer
 */
void scale_init();

/*
 * Returns the mass in g, input is a processed value in mV
 */
uint32_t measure_mass(uint16_t mcu_vcc);
uint32_t r_hive_mass(void);
void start_scale_sampling(void);
void process_scale_samples(void);

#endif /* WEIGHT_MEASUREMENT_H_ */
