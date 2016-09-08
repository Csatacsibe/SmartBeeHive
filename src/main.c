/*
 * main.c
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#include "stm32f0xx.h"
#include "stm32f0xx_hal_cortex.h"
#include "stm32f0xx_hal.h"

/* STM32 peripherals */
#include <STM32_bsp/adc.h>
#include <STM32_bsp/gpio.h>
#include <STM32_bsp/i2c.h>
#include <STM32_bsp/usart.h>

/* SBH peripherals*/
#include "power_management.h"
#include "device_management.h"
#include <gyroscope/FXAS21002C_driver.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <hum_temp_sensor/Si7021_driver.h>

#include "eeprom.h"

#include "math.h"

void SystemClock_Config(void);

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_GPIO_Init();

  /* Initialize SBH peripherals */
  power_mngt_init();
  SIM808_init();
  FXAS21002C_init();

  /* Local variables */
  //volatile float vdd, vbat, idd, temp_MCU;
  //float temp, humi;

  while (1)
  {
    /*vdd  = calculate_MCU_Vdd();
    vbat = r_battery_voltage();
    idd  = r_supply_current();
    temp_MCU = r_MCU_temp();

    r_both_Si7021(&humi, &temp);*/

    if(r_push_button())
    {
      toggle_switch_state();
      power_SIM808();
    }

    if(get_switch_state())
    {
      ext_LED(1);
    }
    else
    {
      ext_LED(0);
    }
  }
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif
