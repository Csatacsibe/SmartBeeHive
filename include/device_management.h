/*
 * device_management.h
 *
 *  Created on: 2016. szept. 5.
 *      Author: Bence
 */

#ifndef DEVICE_MANAGEMENT_H_
#define DEVICE_MANAGEMENT_H_

#include "types.h"

/*
 *  Waits for a flag to be set
 *
 *  Params:
 *  flag: flag to be checked
 *  timeout: timeout value in milis
 *
 *  retval: False if timeout occurred
 */
boolean_t waitFor(boolean_t* flag, uint32_t timeout);



#endif /* DEVICE_MANAGEMENT_H_ */
