/*
 * FXAS21002C_driver.h
 *
 *  Created on: 2016. júl. 1.
 *      Author: Bence
 */

#ifndef FXAS21002C_DRIVER_H_
#define FXAS21002C_DRIVER_H_

#include "types.h"

typedef enum FXAS21002C_registers
{
	STATUS = 0x00,

	OUT_X_MSB = 0x01,
	OUT_X_LSB = 0x02,
	OUT_Y_MSB = 0x03,
	OUT_Y_LSB = 0x04,
	OUT_Z_MSB = 0x05,
	OUT_Z_LSB = 0x06,

	DR_STATUS = 0x07,

	F_STATUS = 0x08,
	F_SETUP  = 0x09, // R/W
	F_EVENT  = 0x0A,

	INT_SRC_FLAG = 0x0B,

	WHO_AM_I = 0x0C,

	CTRL_REG0 = 0x0D, // R/W
	CTRL_REG1 = 0x13, // R/W
	CTRL_REG2 = 0x14, // R/W
	CTRL_REG3 = 0x15, // R/W

	RT_CFG   = 0x0E, // R/W
	RT_SRC   = 0x0F,
	RT_THS   = 0x10, // R/W
	RT_COUNT = 0x11, // R/W

	TEMP = 0x12

}FXAS21002C_registers_t;

typedef enum FXAS21002C_mode
{
  Active  = 2,
  Ready   = 1,
  Standby = 0,
}FXAS21002C_modes_t;

typedef enum FXAS21002C_ODR
{
  _800Hz = 8000,
  _400Hz = 4000,
  _200Hz = 2000,
  _100Hz = 1000,
  _50Hz  = 500,
  _25Hz  = 350,
  _12Hz5 = 125
}FXAS21002C_ODR_t;

typedef enum FXAS21002C_FSR
{
  _4000_dps = 4000,
  _2000_dps = 2000,
  _1000_dps = 1000,
  _500_dps  = 500,
  _250_dps  = 250
}FXAS21002C_FSR_t;

typedef enum FXAS21002C_axis
{
  Z_axis = 2,
  Y_axis = 1,
  X_axis = 0
}FXAS21002C_axis_t;

void FXAS21002C_init(void);

void enter_mode_FXAS21002C(FXAS21002C_modes_t mode);
FXAS21002C_modes_t r_mode_FXAS21002C(void);
boolean_t is_boot_ended_FXAS21002(void);

void s_ODR_FXAS21002C(FXAS21002C_ODR_t rate);
FXAS21002C_ODR_t r_ODR_FXAS21002C(void);

void s_FSR_FXAS21002C(FXAS21002C_FSR_t range);
FXAS21002C_FSR_t r_FSR_FXAS21002C(void);

void s_selftest_FXAS21002C(boolean_t enable);
void reset_hard_FXAS21002C(void);
void reset_soft_FXAS21002C(void);

uint8_t who_am_I_FXAS21002C(void);
int8_t r_temp_FXAS21002C(void);

void s_RTD_on_axis_FXAS21002C(FXAS21002C_axis_t axis, boolean_t enable);
void s_RT_DCnt_mode_FXAS21002C(boolean_t set);
uint8_t calculate_RT_DCnt_value(uint16_t milisec);
void s_RT_count_FXAS21002C(uint8_t value);
void s_RT_threshold_FXAS21002C(uint16_t dps);

#endif /* FXAS21002C_DRIVER_H_ */
