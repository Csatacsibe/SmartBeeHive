#include <STM32_bsp/adc.h>
#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>
#include "power_management.h"
#include <GPRS_GSM_GPS/SIM808_driver.h>

static float I_SENSE_GAIN = 725.6;   // I_supply [mA] = V_sense[V] * I_SENSE_GAIN[mA/V]
static float FACTORY_CALIB_VDD = 3.31;

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

boolean_t get_charger_stat()
{
  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(CHG_STAT_GPIO_Port, CHG_STAT_Pin))
  {
    return True;
  }
  else
  {
    return False;
  }

}

void power_mngt_init()
{
  _4V2_converter_set(True); // default value: 4V2 rail enabled
  batt_charger_set(False);  // default value: LiPo charger disabled
}

float calculate_MCU_Vdd()
{
  float analog_Vdd;
  uint16_t val_Vref_int = r_single_int_channel_ADC(ADC_CHANNEL_VREFINT);

  analog_Vdd = (FACTORY_CALIB_VDD * (*VREFINT_CAL_ADDR))/val_Vref_int;

  return analog_Vdd;
}

float r_battery_voltage()
{
  float vbat;
  float mcu_vdd = calculate_MCU_Vdd();
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(BATTERY_VOLTAGE);
  vbat = (mcu_vdd/4095) * digital_val;
  vbat = vbat * 2;         // 1/2 voltage divider

  return vbat;
}

float r_supply_current()
{
  float v_sense, current;
  float mcu_vdd = calculate_MCU_Vdd();
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(SUPPLY_CURRENT);
  v_sense = (mcu_vdd/4095) * digital_val;
  current = v_sense * I_SENSE_GAIN;

  return current;
}

float r_MCU_temp()
{
  float temp;
  float mcu_vdd = calculate_MCU_Vdd();
  float slope = (110.0 - 30.0)/((*TEMP110_CAL_ADDR) - (*TEMP30_CAL_ADDR));

  uint16_t ts_data = r_single_int_channel_ADC(ADC_CHANNEL_TEMPSENSOR);

  temp = (mcu_vdd/FACTORY_CALIB_VDD) * ts_data;
  temp = slope * (temp - (*TEMP30_CAL_ADDR)) + 30;

  return temp;
}

void enter_mode(power_saving_mode_t mode)
{
  _4V2_converter_set(False);
  GPS_ant_pwr(False);

  HAL_SuspendTick();

  switch(mode)
  {
    case SLEEP: HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;
    case STOP: HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;
    case STANDBY: HAL_PWR_EnterSTANDBYMode();
      break;
  }

  HAL_ResumeTick();
}
