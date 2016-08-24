/*
 * adc.c
 *
 *  Created on: 2016. jul. 18.
 *      Author: Bence
 */

#include <STM32_bsp/adc.h>
#include <STM32_bsp/gpio.h>

ADC_HandleTypeDef hadc;

void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC;
  hadc.Init.Resolution = ADC_RESOLUTION12b;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = OVR_DATA_PRESERVED;
  HAL_ADC_Init(&hadc);

  sConfig.Channel = STRAIN_GAUGE;
  sConfig.Rank = ADC_RANK_NONE;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  sConfig.Channel = SUPPLY_CURRENT;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  sConfig.Channel = BATTERY_VOLTAGE;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  sConfig.Channel = ADC_CHANNEL_VREFINT;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
    __ADC1_CLK_ENABLE();
  
    /**ADC GPIO Configuration    
    PA4     ------> ADC_IN4
    PA5     ------> ADC_IN5
    PA6     ------> ADC_IN6 
    */
    GPIO_InitStruct.Pin = GAUGE_OUT_Pin|I_SUPP_Pin|V_BAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
    __ADC1_CLK_DISABLE();
  
    /**ADC GPIO Configuration    
    PA4     ------> ADC_IN4
    PA5     ------> ADC_IN5
    PA6     ------> ADC_IN6 
    */
    HAL_GPIO_DeInit(GPIOA, GAUGE_OUT_Pin|I_SUPP_Pin|V_BAT_Pin);

    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  }
}

void config_ext_channel_ADC(uint32_t channel, boolean_t val)
{
  ADC_ChannelConfTypeDef sConfig;

  sConfig.Channel = channel;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

  if(True == val)
  {
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  }
  else
  {
    sConfig.Rank = ADC_RANK_NONE;
  }

  HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

uint32_t r_single_ext_channel_ADC(uint32_t channel)
{
  uint32_t digital_result;

  config_ext_channel_ADC(channel, True);

  HAL_ADCEx_Calibration_Start(&hadc);

  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, 1000);
  digital_result = HAL_ADC_GetValue(&hadc);
  HAL_ADC_Stop(&hadc);

  config_ext_channel_ADC(channel, False);

  return digital_result;
}

void config_int_channel_ADC(uint32_t channel, boolean_t val)
{
  ADC_ChannelConfTypeDef sConfig;
  sConfig.Channel = channel;

  if(val == True)
  {
    if(channel == ADC_CHANNEL_VREFINT)
    {
      ADC->CCR |= ADC_CCR_VREFEN;
      hadc.Instance->CHSELR = (uint32_t)(ADC_CHSELR_CHSEL17);
    }
    else if(channel == ADC_CHANNEL_TEMPSENSOR)
    {
      ADC->CCR |= ADC_CCR_TSEN;
      hadc.Instance->CHSELR = (uint32_t)(ADC_CHSELR_CHSEL16);
    }

    sConfig.Rank          = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime  = ADC_SAMPLETIME_239CYCLES_5;
  }
  else if(val == False)
  {
    if(channel == ADC_CHANNEL_VREFINT)
    {
      ADC->CCR &= ~ADC_CCR_VREFEN;
      hadc.Instance->CHSELR = 0;
    }
    else if(channel == ADC_CHANNEL_TEMPSENSOR)
    {
      ADC->CCR &= ~ADC_CCR_TSEN;
      hadc.Instance->CHSELR = 0;
    }

    sConfig.Rank          = ADC_RANK_NONE;
    sConfig.SamplingTime  = ADC_SAMPLETIME_239CYCLES_5;
  }

  HAL_ADC_ConfigChannel(&hadc,&sConfig);
}

uint32_t r_single_int_channel_ADC(uint32_t channel)
{
  uint32_t digital_result;

  config_int_channel_ADC(channel, True);

  HAL_ADCEx_Calibration_Start(&hadc);

  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, 1000);
  digital_result = HAL_ADC_GetValue(&hadc);
  HAL_ADC_Stop(&hadc);

  config_int_channel_ADC(channel, False);

  return digital_result;
}

