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
#include <STM32_bsp/rtc.h>

/* SBH peripherals*/
#include "power_management.h"
#include "device_management.h"
#include <weight_measurement.h>
#include <gyroscope/FXAS21002C_driver.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <hum_temp_sensor/Si7021_driver.h>

#include "eeprom.h"

void SystemClock_Config(void);

volatile uint16_t scale_raw;
volatile float scale_averaged;

device_t device;
hive_data_t hive;

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_DMA_Init();
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();

  /* Initialize SBH peripherals */
  power_mngt_init();
  SIM808_init();
  FXAS21002C_init();
  scale_init();

  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

  //__HAL_PWR_GET_FLAG(PWR_FLAG_WU);
  //__HAL_PWR_GET_FLAG(PWR_FLAG_SB);

  hive.mass = 0;

  while (1)
  {
    device.MCU_vcc         = calculate_MCU_vcc();
    device.vbat            = r_battery_voltage();
    device.current         = r_supply_current();
    device.MCU_temperature = r_MCU_temp();
    device.charger_status  = r_charger_stat();

    scale_raw = process_bridge_output(r_wheatstone_bridge());
    scale_averaged = averaging_filter(scale_raw);

    ext_LED(TOGGLE);

    hive.mass = mass_in_g(scale_averaged);

    //r_both_Si7021(&(hive.humidity), &(hive.temperature));

    if(r_push_button())
    {
      toggle_switch_state();
    }

    if(get_switch_state())
    {

    }
    else
    {

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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
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
