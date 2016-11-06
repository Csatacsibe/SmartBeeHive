/*
 * Si7021.c
 *
 *  Created on: 2016. jún. 17.
 *      Author: Bence
 */

#include <hum_temp_sensor/Si7021_driver.h>
#include <device_management.h>
#include <STM32_bsp/i2c.h>

static const uint16_t i2c_addr = (0x40<<1); // Si7021 I2C address

static const uint8_t UR_bitmask = 0x3A;     // Reset Settings = 0011_1010, (reserved bits)
static const uint8_t RES0 = 0;
static const uint8_t RES1 = 7;
static const uint8_t VDDS = 6;    		    	// read only
static const uint8_t HTRE = 2;

static const uint8_t HEATER0 = 0;
static const uint8_t HEATER1 = 1;
static const uint8_t HEATER2 = 2;
static const uint8_t HEATER3 = 3;

static float process_temp_code(uint16_t temp_code);
static float process_humi_code(uint16_t humi_code);
static uint16_t convert_to_uint16(uint8_t bytes[]);

static float process_temp_code(uint16_t temp_code)
{
	float value;

	value = (float)(((175.72 * temp_code) / 65536.0) - 46.85);

	return value;
}

static float process_humi_code(uint16_t humi_code)
{
	float value;

	value = (float)(((125.0 * humi_code) / 65536.0) - 6.0);

	if(value < 0)
	{
		return 0;
	}
	else if(value > 100)
	{
		return 100;
	}
	else
	{
		return (float)value;
	}
}

static uint16_t convert_to_uint16(uint8_t bytes[])
{
	return (uint16_t)((bytes[0]<<8) | bytes[1]);
}

void r_single_Si7021(float* data, Si7021_measurement_type_t type)
{
	uint8_t cmd;
	uint8_t buffer[2] = {0,0};
	uint16_t code;

	if(type == Humidity)
	{
		cmd = Humi_HM;
	}
	else if(type == Temperature)
	{
		cmd = Temp_HM;
	}

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, &cmd, 1, 10000);
	HAL_I2C_Master_Receive(&hi2c1, i2c_addr, buffer, 2, 10000);

	code = convert_to_uint16(buffer);

	if(type == Humidity)
	{
		*data = process_humi_code(code);
		*data = round_to(*data, 0);
	}
	else if(type == Temperature)
	{
		*data = process_temp_code(code);
		*data = round_to(*data, 1);
	}
}

void r_both_Si7021(float* humidity, float* temperature)
{
	uint8_t cmd = Humi_HM;
	uint8_t buffer[2] = {0,0};
	uint16_t code;

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, &cmd, 1, 10000);
	HAL_I2C_Master_Receive(&hi2c1, i2c_addr, buffer, 2, 10000);

	code = convert_to_uint16(buffer);
	*humidity = process_humi_code(code);
	*humidity = round_to(*humidity, 0);

	// There is a temperature measurement with each RH measurement
	cmd = Temp_AH;

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, &cmd, 1, 10000);
	HAL_I2C_Master_Receive(&hi2c1, i2c_addr, buffer, 2, 10000);

	code = convert_to_uint16(buffer);
	*temperature = process_temp_code(code);
	*temperature = round_to(*temperature, 1);
}

int8_t r_firmware_rev_Si7021()
{
  uint8_t cmd[2];
  uint8_t data;

  cmd[0] = R_Firm_rev1;
  cmd[1] = R_Firm_rev2;

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, cmd, 2, 10000);
	HAL_I2C_Master_Receive(&hi2c1, i2c_addr, &data, 1, 10000);

	switch(data)
	{
		case 0xFF: return 1;
		case 0x20: return 2;
		default: return -1;
	}
}

uint8_t r_reg_Si7021(Si7021_registers_t reg)
{
	uint8_t cmd;
	uint8_t data;

	if(reg == User_Register_1)
	{
		cmd = R_RHT_U_reg;
	}
	else if(reg == Heater_Control_Register)
	{
		cmd = R_Heater_C_reg;
	}

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, &cmd, 1, 10000);
	HAL_I2C_Master_Receive(&hi2c1, i2c_addr, &data, 1, 10000);

	return data;
}

void w_reg_Si7021(uint8_t bit_field, io_operation_t io, Si7021_registers_t reg)
{
	uint8_t cmd[2];

	cmd[1] = r_reg_Si7021(reg);

	if(reg == User_Register_1)
	{
		bit_field |= UR_bitmask;
		cmd[0] = W_RHT_U_reg;
	}
	else if(reg == Heater_Control_Register)
	{
		cmd[0] = W_Heater_C_reg;
	}

	if(io == set_bit)
	{
		cmd[1] |= bit_field;
	}
	else if(io == reset_bit)
	{
		cmd[1] &= bit_field;
	}

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, cmd, 2, 10000);
}

void set_resolution_Si7021(Si7021_resolution_t resolution)
{
	uint8_t bitfield;

	switch(resolution)
	{
		case H12_T14:
		{
			bitfield = (uint8_t)(~(1<<RES1) & ~(1<<RES0));
			w_reg_Si7021(bitfield, reset_bit, User_Register_1);
			break;
		}
		case H8_T12:
		{
			bitfield = (uint8_t)(~(1<<RES1));
			w_reg_Si7021(bitfield, reset_bit, User_Register_1);

			bitfield = (1<<RES0);
			w_reg_Si7021(bitfield, set_bit, User_Register_1);
			break;
		}
		case H10_T13:
		{
			bitfield = ~(1<<RES0);
			w_reg_Si7021(bitfield, reset_bit, User_Register_1);

			bitfield = (1<<RES1);
			w_reg_Si7021(bitfield, set_bit, User_Register_1);
			break;
		}
		case H11_T11:
		{
			bitfield = (1<<RES1) | (1<<RES0);
			w_reg_Si7021(bitfield, set_bit, User_Register_1);
			break;
		}
		default: return;
	}
}

void set_heater_current_Si7021(Si7021_heater_current_t val)
{
	uint8_t bitfield;

	switch(val)
	{
		case _3_mA:
		{
			bitfield = ~(1<<HEATER0) & ~(1<<HEATER1) & ~(1<<HEATER2) & ~(1<<HEATER3);
			w_reg_Si7021(bitfield, reset_bit, Heater_Control_Register);
			break;
		}
		case _9_mA:
		{
			bitfield = ~(1<<HEATER1) & ~(1<<HEATER2) & ~(1<<HEATER3);
			w_reg_Si7021(bitfield, reset_bit, Heater_Control_Register);

			bitfield = (1<<HEATER0);
			w_reg_Si7021(bitfield, set_bit, Heater_Control_Register);
			break;
		}
		case _15_mA:
		{
			bitfield = ~(1<<HEATER0) & ~(1<<HEATER2) & ~(1<<HEATER3);
			w_reg_Si7021(bitfield, reset_bit, Heater_Control_Register);

			bitfield = (1<<HEATER1);
			w_reg_Si7021(bitfield, set_bit, Heater_Control_Register);
			break;
		}
		case _27_mA:
		{
			bitfield = ~(1<<HEATER0) & ~(1<<HEATER1) & ~(1<<HEATER3);
			w_reg_Si7021(bitfield, reset_bit, Heater_Control_Register);

			bitfield = (1<<HEATER2);
			w_reg_Si7021(bitfield, set_bit, Heater_Control_Register);
			break;
		}
		case _51_mA:
		{
			bitfield = ~(1<<HEATER0) & ~(1<<HEATER1) & ~(1<<HEATER2);
			w_reg_Si7021(bitfield, reset_bit, Heater_Control_Register);

			bitfield = (1<<HEATER3);
			w_reg_Si7021(bitfield, set_bit, Heater_Control_Register);
			break;
		}
		case _94_mA:
		{
			bitfield = (1<<HEATER0) | (1<<HEATER1) | (1<<HEATER2) | (1<<HEATER3);
			w_reg_Si7021(bitfield, set_bit, Heater_Control_Register);
			break;
		}
		default: return;
	}
}

boolean_t VDD_warning_Si7021()
{
	uint8_t u_reg = r_reg_Si7021(User_Register_1);

	if(u_reg & (1<<VDDS))
	{
		return False;
	}
	else
	{
		return True;
	}
}

void c_heater_Si7021(boolean_t val)
{
    uint8_t bitfield;

    if(val == True)
    {
    	bitfield = (1<<HTRE);
    	w_reg_Si7021(bitfield, set_bit, User_Register_1);
    }
    else
    {
    	bitfield = ~(1<<HTRE);
    	w_reg_Si7021(bitfield, reset_bit, User_Register_1);
    }
}

void reset_Si7021()
{
	uint8_t cmd = Si7021_Reset;
	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, &cmd, 1, 10000);
}





