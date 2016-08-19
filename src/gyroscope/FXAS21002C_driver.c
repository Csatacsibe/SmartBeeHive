/*
 * FXAS21002C.c
 *
 *  Created on: 2016. júl. 1.
 *      Author: Bence
 */

#include "FXAS21002C_driver.h"

#include "i2c.h"
#include "gpio.h"

#include "constants.h"

static const uint16_t i2c_addr = (0x20<<1); // FXAS21002 I2C address

void r_regs_FXAS21002C(FXAS21002C_registers_t reg, uint16_t reg_nbr, uint8_t buffer[])
{
	uint8_t cmd = reg;

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, &cmd, 1, 10000);
	HAL_I2C_Master_Receive(&hi2c1, i2c_addr, buffer, reg_nbr, 10000);
}


void w_regs_FXAS21002C(FXAS21002C_registers_t reg, uint16_t reg_nbr, uint8_t buffer[])
{
	uint8_t i;
	uint8_t cmd[reg_nbr + 1];

  cmd[0] = reg;

  for(i = 1; i < reg_nbr + 1; i++)
  {
      cmd[i] = buffer[i-1];
  }

  HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, cmd, reg_nbr + 1, 10000);
}

void reset_FXAS21002C()
{
  HAL_GPIO_WritePin(GY_RST_GPIO_Port, GY_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(150);
  HAL_GPIO_WritePin(GY_RST_GPIO_Port, GY_RST_Pin, GPIO_PIN_SET);
}
