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

	TEMP = 0x12,

}FXAS21002C_registers_t;

/*
 *  read registers of device
 *
 *  params:
 *  reg:     First register's address (auto increment by device afterward)
 *  reg_nbr: Number of registers to be read
 *  buffer:  Buffer to store register content(s)
 */
void r_regs_FXAS21002C(FXAS21002C_registers_t reg, uint16_t reg_nbr, uint8_t buffer[]);

/*
 *  write registers of device
 *
 *  params:
 *  reg:     First register's address (auto increment by device afterward)
 *  reg_nbr: Number of registers to be written
 *  buffer:  Buffer of value(s) to be written
 */
void w_regs_FXAS21002C(FXAS21002C_registers_t reg, uint16_t reg_nbr, uint8_t buffer[]);

/*
 *  resets FXAS21002C by pulling its RST pin low
 */
void reset_hard_FXAS21002C(void);

/*
 *  resets FXAS21002C by setting the RST bit
 *  in the CTRL_REG1 register
 */
void reset_soft_FXAS21002C(void);

/*
 *  Initializes the RST pin
 */
void FXAS21002C_init(void);

#endif /* FXAS21002C_DRIVER_H_ */
