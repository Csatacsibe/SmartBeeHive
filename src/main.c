/*
 * main.c
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#include "stm32f0xx.h"
#include "stm32f0xx_hal_cortex.h"
#include "stm32f0xx_hal.h"

/* SBH peripherals*/
#include <device_management.h>
#include <GPRS_GSM_GPS/SIM808_driver.h>
#include <GPRS_GSM_GPS/SIM808_device.h>

#include <logger.h>

#define DEBUG_VERSION

void SystemClock_Config(void);

#ifdef DEBUG_VERSION
static void reset_debug_input(void);
#endif

volatile uint16_t user_input = 65535;

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  init_BSP();

  /* Initialize SBH peripherals */
  init_board();

  //__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

  //__HAL_PWR_GET_FLAG(PWR_FLAG_WU);
  //__HAL_PWR_GET_FLAG(PWR_FLAG_SB);

  while (1)
  {
    logger_state_machine();

    #ifdef DEBUG_VERSION

      if(r_push_button())
      {

      }

      if(get_switch_state())
      {

      }
      else
      {

      }

      switch(user_input)
      {
        case 1:
          ext_LED(TOGGLE);
          break;
        case 2:
          power_SIM808();
          reset_debug_input();
          break;
        case 55:
          reset_SIM808();
          reset_debug_input();
          break;
        case 44:
          put_s_SIM808("ATE0\r");
          reset_debug_input();
          break;
        case 3:
        {
          put_s_SIM808("AT\r");
          reset_debug_input();
          break;
        }
        case 5:
        {
          cmd_tmp_SIM808();
          reset_debug_input();
          break;
        }
        case 6:
        {
          cmd_vcc_SIM808();
          reset_debug_input();
          break;
        }
        case 7:
        {
          reset_debug_input();
          break;
        }
        case 8:
        {
          reset_debug_input();
          break;
        }
        case 9:
        {
          put_s_SIM808("AT+CANT=1,1,10\r");
          reset_debug_input();
          break;
        }
        case 10:
        {
          put_s_SIM808("AT+CSQ\r"); // signal strength
          reset_debug_input();
          break;
        }
        case 11:
        {
          put_s_SIM808("AT+CPOL?\r"); // preferred operator list
          reset_debug_input();
          break;
        }
        case 12:
        {
          put_s_SIM808("AT+COPN\r"); // read operator names
          reset_debug_input();
          break;
        }
        case 13:
        {
          put_s_SIM808("AT+CSPN?\r"); // AT+CSPN Get Service Provider Name from SIM
          reset_debug_input();
          break;
        }
        case 14:
        {
          put_s_SIM808("AT+COPS?\r");
          reset_debug_input();
          break;
        }
        case 15:
        {
          put_s_SIM808("AT+CMEE=2\r"); // Enable +CME ERROR: <err> result code and use verbose <err> values
          reset_debug_input();
          break;
        }
        case 16:
        {
          put_s_SIM808("AT&W\r"); // save config
          reset_debug_input();
          break;
        }
        default:
          break;
      }

    #endif
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

#ifdef DEBUG_VERSION

static void reset_debug_input()
{
    user_input = 65535;
    ext_LED(TOGGLE);
}

#endif

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
