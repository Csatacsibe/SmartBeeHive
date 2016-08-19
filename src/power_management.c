#include "power_management.h"

#include "gpio.h"
#include "adc.h"

#include "constants.h"

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

void power_mngt_init(void)
{
  _4V2_converter_set(True); // default value: 4V2 rail enabled
  batt_charger_set(False);  // default value: LiPo charger disabled
}
