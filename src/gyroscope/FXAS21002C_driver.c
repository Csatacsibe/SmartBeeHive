/*
 * FXAS21002C.c
 *
 *  Created on: 2016. júl. 1.
 *      Author: Bence
 */

#include <gyroscope/FXAS21002C_driver.h>
#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>
#include <STM32_bsp/i2c.h>

static const uint16_t i2c_addr = (0x20<<1); // FXAS21002 I2C address

/* INT_SOURCE-FLAG register bits [read only] */
#define BOOTEND  3 // Boot sequence complete event flag
#define SRC_RT   1 // Rate threshold event source flag

/* CTRL_REG0 register bits*/
#define FS1 1 // Full-scale range selection
#define FS0 0 // Full-scale range selection

/* CTRL_REG3 register bits */
#define FS_DOUBLE 0 // Full-scale range expansion enable, double it

/* RT_CFG register bits */
#define ELE   3 // Event latch enable
#define ZTEFE 2 // Event flag enable on Z-axis rate
#define YTEFE 1 // Event flag enable on Y-axis rate
#define XTEFE 0 // Event flag enable on X-axis rate

/* RT_SRC register bits [read only] */
#define EA       6 // Event active flag
#define ZRT      5 // Z rate event
#define Z_RT_POL 4 // Polarity of Z event
#define YRT      3 // Y rate event
#define Y_RT_POL 2 // Polarity of Y event
#define XRT      1 // X rate event
#define X_RT_POL 0 // Polarity of X event

/* RT_THS register bits */
#define DBCNTM 7 // Debounce counter mode selection
                 // THS 6:0 Unsigned 7-bit rate threshold value

/* CTRL_REG1 register bits */
#define RST    6 // Software Reset
#define ST     5 // Self-Test enable
#define DR2    4 // Output Data Rate selection MSB
#define DR1    3 // Output Data Rate selection
#define DR0    2 // Output Data Rate selection LSB
#define ACTIVE 1 // Standby/Active mode selection
#define READY  0 // Standby/Ready mode selection

/* CTRL_REG2: Interrupt Enable register bits */
#define INT_CFG_RT 5 // Rate threshold interrupt pin routing
#define INT_EN_RT  4 // Rate threshold interrupt enable
#define IPOL       1 // Interrupt logic polarity
#define PP_OD      0 // INT1 and INT2 pin output driver configuration

static void r_regs_FXAS21002C(FXAS21002C_registers_t address, uint16_t reg_nbr, uint8_t buffer[]);
static void w_regs_FXAS21002C(FXAS21002C_registers_t address, uint16_t reg_nbr, uint8_t buffer[]);
static void s_double_FS_FXAS21002C(boolean_t enable);
static boolean_t is_FS_doubled_FXAS21002C(void);

static void r_regs_FXAS21002C(FXAS21002C_registers_t address, uint16_t reg_nbr, uint8_t buffer[])
{
	uint8_t cmd = address;

	HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, &cmd, 1, 10000);
	HAL_I2C_Master_Receive(&hi2c1, i2c_addr, buffer, reg_nbr, 10000);
}

static void w_regs_FXAS21002C(FXAS21002C_registers_t address, uint16_t reg_nbr, uint8_t buffer[])
{
  if(r_mode_FXAS21002C() != Standby)
  {
    // TODO: enter_mode_FXAS21002C(Standby);
    return;
  }

	uint8_t i;
	uint8_t cmd[reg_nbr + 1];

  cmd[0] = address;

  for(i = 1; i < reg_nbr + 1; i++)
  {
      cmd[i] = buffer[i-1];
  }

  HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, cmd, reg_nbr + 1, 10000);
}

void enter_mode_FXAS21002C(FXAS21002C_modes_t mode)
{
  uint8_t reg;
  r_regs_FXAS21002C(CTRL_REG1, 1, &reg);

  switch(mode)
  {
    case Active:
      reg &= ~(1<<READY);
      reg |= (1<<ACTIVE);
      break;
    case Ready:
      reg &= ~(1<<ACTIVE);
      reg |= (1<<READY);
      break;
    case Standby:
      reg &= ~(1<<READY) & ~(1<<ACTIVE);
      break;
    default:
      return;
  }

  w_regs_FXAS21002C(CTRL_REG1, 1, &reg);
}

FXAS21002C_modes_t r_mode_FXAS21002C()
{
  uint8_t reg;
  r_regs_FXAS21002C(CTRL_REG1, 1, &reg);

  return (reg &= 0x03);
}

boolean_t is_boot_ended_FXAS21002(void)
{
  uint8_t reg;
  r_regs_FXAS21002C(INT_SRC_FLAG, 1, &reg);

  if(reg & (1<<BOOTEND))
    return True;
  else
    return False;
}

void s_ODR_FXAS21002C(FXAS21002C_ODR_t rate)
{
  uint8_t reg;
  r_regs_FXAS21002C(CTRL_REG1, 1, &reg);

  switch(rate)
  {
    case _800Hz:
      reg &= ~(1<<DR2) & ~(1<<DR1) & ~(1<<DR0);
      break;
    case _400Hz:
      reg &= ~(1<<DR2) & ~(1<<DR1);
      reg |= (1<<DR0);
      break;
    case _200Hz:
      reg &= ~(1<<DR2) & ~(1<<DR0);
      reg |= (1<<DR1);
      break;
    case _100Hz:
      reg &= ~(1<<DR2);
      reg |= (1<<DR1) | (1<<DR0);
      break;
    case _50Hz:
      reg &= ~(1<<DR1) & ~(1<<DR0);
      reg |= (1<<DR2);
      break;
    case _25Hz:
      reg &= ~(1<<DR1);
      reg |= (1<<DR2) | (1<<DR0);
      break;
    case _12Hz5:
      reg &= ~(1<<DR0);
      reg |= (1<<DR2) | (1<<DR1);
      break;
    default:
      return;
  }

  w_regs_FXAS21002C(CTRL_REG1, 1, &reg);
}

FXAS21002C_ODR_t r_ODR_FXAS21002C()
{
  uint8_t reg;

  r_regs_FXAS21002C(CTRL_REG1, 1, &reg);

  reg &= ((1<<DR2) | (1<<DR1) | (1<<DR0));
  reg = reg >> DR0;

  switch(reg)
  {
    case 0:  return _800Hz;
    case 1:  return _400Hz;
    case 2:  return _200Hz;
    case 3:  return _100Hz;
    case 4:  return _50Hz;
    case 5:  return _25Hz;
    case 6:  return _12Hz5;
    case 7:  return _12Hz5;
    default: return _800Hz;
  }
}

void s_selftest_FXAS21002C(boolean_t enable)
{
  uint8_t reg;
  r_regs_FXAS21002C(CTRL_REG1, 1, &reg);

  if(enable == True)
    reg |= (1<<ST);
  else
    reg &= ~(1<<ST);

  w_regs_FXAS21002C(CTRL_REG1, 1, &reg);
}

void reset_hard_FXAS21002C()
{
  HAL_GPIO_WritePin(GY_RST_GPIO_Port, GY_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(150);
  HAL_GPIO_WritePin(GY_RST_GPIO_Port, GY_RST_Pin, GPIO_PIN_SET);
}

void reset_soft_FXAS21002C()
{
  uint8_t cmd = (1<<RST);

  w_regs_FXAS21002C(CTRL_REG1, 1, &cmd);
}

uint8_t who_am_I_FXAS21002C()
{
  uint8_t deviceID = 0;
  r_regs_FXAS21002C(WHO_AM_I, 1, &deviceID);
  return deviceID;
}

int8_t r_temp_FXAS21002C()
{
  int8_t temp = 0;
  r_regs_FXAS21002C(TEMP, 1, (uint8_t*)(&temp));
  return temp;
}

void FXAS21002C_init()
{
  HAL_GPIO_WritePin(GY_RST_GPIO_Port, GY_RST_Pin, GPIO_PIN_SET);
}

void s_RTD_on_axis_FXAS21002C(FXAS21002C_axis_t axis, boolean_t enable)
{
  uint8_t reg;
  r_regs_FXAS21002C(RT_CFG, 1, &reg);

  if(enable == True)
    reg |= (1<<axis);
  else
    reg &= ~(1<<axis);

  w_regs_FXAS21002C(RT_CFG, 1, &reg);
}

void s_RT_DCnt_mode_FXAS21002C(boolean_t set)
{
  uint8_t reg;
  r_regs_FXAS21002C(RT_THS, 1, &reg);

  if(set == True)
    reg |= (1<<DBCNTM);
  else
    reg &= ~(1<<DBCNTM);

  w_regs_FXAS21002C(RT_THS, 1, &reg);
}

void s_RT_threshold_FXAS21002C(uint16_t dps)
{
  uint8_t reg, ths;
  uint16_t fsr;
  float sensitivity;

  fsr = r_FSR_FXAS21002C();
  r_regs_FXAS21002C(RT_THS, 1, &reg);

  if(dps > fsr * 0.95)
      dps = fsr * 0.95;

  sensitivity = (fsr/1000.0)/32.0;
  ths = (dps/(sensitivity * 256)) - 1;

  reg &= (1<<DBCNTM);
  ths &= ~(1<<DBCNTM);
  reg |= ths;

  w_regs_FXAS21002C(RT_THS, 1, &reg);
}

uint8_t calculate_RT_DCnt_value(uint16_t milisec)
{
  FXAS21002C_ODR_t odr = r_ODR_FXAS21002C();
  uint16_t max  = (10000/odr) * 255;

  if(milisec > max)
    return 255;
  else
    return (milisec * odr)/10000;
}

void s_RT_count_FXAS21002C(uint8_t value)
{
  w_regs_FXAS21002C(RT_COUNT, 1, &value);
}

void s_FSR_FXAS21002C(FXAS21002C_FSR_t range)
{
  uint8_t reg;
  boolean_t set_doubled = False;
  r_regs_FXAS21002C(CTRL_REG0, 1, &reg);

  switch(range)
  {
    case _4000_dps:
      set_doubled = True;
      reg &= ~(1<<FS1) & ~(1<<FS0);
      break;
    case _2000_dps:
      reg &= ~(1<<FS1) & ~(1<<FS0);
      break;
    case _1000_dps:
      reg &= ~(1<<FS1);
      reg |= (1<<FS0);
      break;
    case _500_dps:
      reg &= ~(1<<FS0);
      reg |= (1<<FS1);
      break;
    case _250_dps:
      reg |= (1<<FS1) | (1<<FS0);
      break;
    default:
      return;
  }

  s_double_FS_FXAS21002C(set_doubled);
  w_regs_FXAS21002C(CTRL_REG0, 1, &reg);
}

FXAS21002C_FSR_t r_FSR_FXAS21002C()
{
  uint8_t reg;
  boolean_t doubled = is_FS_doubled_FXAS21002C();
  r_regs_FXAS21002C(CTRL_REG0, 1, &reg);

  reg &= (1<<FS1) | (1<<FS0);

  switch(reg)
  {
    case 0:  return (_2000_dps * (doubled + 1));
    case 1:  return (_1000_dps * (doubled + 1));
    case 2:  return (_500_dps * (doubled + 1));
    case 3:  return (_250_dps * (doubled + 1));
    default: return 0;
  }
}

static void s_double_FS_FXAS21002C(boolean_t enable)
{
  uint8_t reg;
  r_regs_FXAS21002C(CTRL_REG3, 1, &reg);

  if(enable == True)
    reg |= (1<<FS_DOUBLE);
  else
    reg &= ~(1<<FS_DOUBLE);

  w_regs_FXAS21002C(CTRL_REG3, 1, &reg);
}

static boolean_t is_FS_doubled_FXAS21002C()
{
  uint8_t reg;
  r_regs_FXAS21002C(CTRL_REG3, 1, &reg);

  return (reg & (1<<FS_DOUBLE));
}
