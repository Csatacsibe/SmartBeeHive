/*
 * dma.c
 *
 *  Created on: 2016. okt. 27.
 *      Author: Bence
 */

#include <STM32_bsp/dma.h>

void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}
