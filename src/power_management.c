#include <STM32_bsp/adc.h>
#include <STM32_bsp/constants.h>
#include <STM32_bsp/gpio.h>
#include <power_management.h>
#include <device_management.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <state_machine.h>

static float I_SENSE_GAIN = 0.7143;   // I_supply [mA] = V_sense[mV] * I_SENSE_GAIN[A/V]
static float FACTORY_CALIB_VDD = 3.31;

boolean_t diagnostic = False;

void enable_4V2_converter(boolean_t val)
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

void enable_bat_charger(boolean_t val)
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

boolean_t r_charger_stat()
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

void charge_control(uint16_t vbat)
{
  if(vbat < 4000)
  {
    enable_bat_charger(True);
  }
}

void power_mngt_init()
{
  enable_4V2_converter(True); // default value: 4V2 rail enabled
  enable_bat_charger(False);  // default value: LiPo charger disabled
}

uint16_t calculate_MCU_vcc()
{
  float analog_Vdd;
  uint16_t val_Vref_int = r_single_int_channel_ADC(ADC_CHANNEL_VREFINT);

  analog_Vdd = (FACTORY_CALIB_VDD * (*VREFINT_CAL_ADDR))/val_Vref_int;

  return analog_Vdd * 1000;
}

uint16_t r_battery_voltage(uint16_t mcu_vcc)
{
  float vbat;
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(BATTERY_VOLTAGE);
  vbat = (mcu_vcc/4095.0) * digital_val;
  vbat = vbat * 2;         // 1/2 voltage divider

  return vbat;
}

uint16_t r_supply_current(uint16_t mcu_vcc)
{
  float v_sense, current;
  uint16_t digital_val;

  digital_val = r_single_ext_channel_ADC(SUPPLY_CURRENT);
  v_sense = (mcu_vcc/4095.0) * digital_val;
  current = v_sense * I_SENSE_GAIN;

  return current;
}

float r_MCU_temp(uint16_t mcu_vcc)
{
  float temp;
  float slope = ((110.0 - 30.0)/((*TEMP110_CAL_ADDR) - (*TEMP30_CAL_ADDR)));

  uint16_t ts_data = r_single_int_channel_ADC(ADC_CHANNEL_TEMPSENSOR);

  temp = ((mcu_vcc/FACTORY_CALIB_VDD) * ts_data)/1000;
  temp = slope * (temp - (*TEMP30_CAL_ADDR)) + 30;

  return round_to(temp, 0);
}

void enter_mode(power_saving_mode_t mode)
{
  enable_4V2_converter(False);
  enable_GPS_antenna(False);
  reset_wake_up_counter();

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
