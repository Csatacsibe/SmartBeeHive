#include <STM32_bsp/adc.h>
#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>
#include "power_management.h"

static float i_coeff = 725.6;   // I_supply [mA] = V_sense[V] * I_coeff[mA/V]

void _4V2_converter_set(boolean_t val)
{
  if(val == True)
  {
    HAL_GPIO_WritePin(_4_2V_EN_GPIO_Port, _4_2V_EN_Pin, GPIO_PIN_SET);
  }
  else if(val == False)
  {
    HAL_GPIO_WritePin(_4_2V_EN_GPIO_Port, _4_2V_EN_Pin, GPIO_PIN_RESET);
  }
}

void batt_charger_set(boolean_t val)
{
  if(val == True)
  {
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
  }
  else if(val == False)
  {
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
  }
}

GPIO_PinState get_charger_stat()
{
  return HAL_GPIO_ReadPin(CHG_STAT_GPIO_Port, CHG_STAT_Pin);
}

void power_mngt_init()
{
  _4V2_converter_set(True); // default value: 4V2 rail enabled
  batt_charger_set(False);  // default value: LiPo charger disabled
}

float calculate_MCU_Vdd()
{
  float analog_Vdd;
  uint16_t val_Vref_int_cal= *((uint16_t*)VREFINT_CAL_ADDR);
  uint16_t val_Vref_int = r_single_int_channel_ADC(ADC_CHANNEL_VREFINT);

  analog_Vdd = (3.31 * val_Vref_int_cal)/val_Vref_int;

  return analog_Vdd;
}

float r_battery_voltage()
{
  float vbat;
  float mcu_vdd = calculate_MCU_Vdd();
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(BATTERY_VOLTAGE);
  vbat = (mcu_vdd/4096) * digital_val;
  vbat = vbat * 2;         // 1/2 voltage divider

  return vbat;
}

float r_supply_current()
{
  float v_sense, current;
  float mcu_vdd = calculate_MCU_Vdd();
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(SUPPLY_CURRENT);
  v_sense = (mcu_vdd/4096) * digital_val;
  current = v_sense * i_coeff;

  return current;
}

float r_mcu_temp()
{
  return 0;
}
