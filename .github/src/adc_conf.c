#include "main.h"

void Setup_ADC(uint8_t* pBuff)
{
	GPIO_InitTypeDef     GPIO_InitStruct;

	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*	Configure ADC1 Channel 10 pin (PC0) as analogue input */

	GPIO_InitStruct.Pin = 	GPIO_PIN_0;
	GPIO_InitStruct.Mode = 	GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = 	GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	ADC_ChannelConfTypeDef sConfig;

	/* ADC Initialisation */
	AdcHandle.Instance          		 = ADC1;
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;
	AdcHandle.Init.Resolution            = ADC_RESOLUTION8b;
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T3_TRGO;
	AdcHandle.Init.DMAContinuousRequests = ENABLE;
	AdcHandle.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.NbrOfConversion		 = 1;

	HAL_ADC_Init(&AdcHandle);

	/* Configure ADC regular channel */
	sConfig.Channel      = ADC_CHANNEL_10;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset 		 = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	DmaAdcHandle.Instance 					= DMA2_Stream0;
	DmaAdcHandle.Init.Channel 				= DMA_CHANNEL_0;
	DmaAdcHandle.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
	DmaAdcHandle.Init.PeriphInc		 		= DMA_PINC_DISABLE;
	DmaAdcHandle.Init.MemInc 				= DMA_MINC_ENABLE;
	DmaAdcHandle.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE;
	DmaAdcHandle.Init.MemDataAlignment		= DMA_MDATAALIGN_BYTE;
	DmaAdcHandle.Init.Mode 					= DMA_CIRCULAR;
	DmaAdcHandle.Init.Priority 				= DMA_PRIORITY_HIGH;
	DmaAdcHandle.Init.FIFOMode 				= DMA_FIFOMODE_DISABLE;
	DmaAdcHandle.Init.FIFOThreshold 		= DMA_FIFO_THRESHOLD_HALFFULL;
	DmaAdcHandle.Init.MemBurst 				= DMA_MBURST_SINGLE;
	DmaAdcHandle.Init.PeriphBurst 			= DMA_PBURST_SINGLE;

	HAL_DMA_Init(&DmaAdcHandle);
	__HAL_LINKDMA(&AdcHandle, DMA_Handle, DmaAdcHandle);

	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*) pBuff, CCD_LENGTH);
	ADC_READY = 1;
}



void DMA2_Stream0_IRQHandler(void)
{

	HAL_DMA_IRQHandler(&DmaAdcHandle);
}





void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	//Disable Timer 3
	TIM3->CR1 &= (uint16_t)~TIM_CR1_CEN;
	if(CONTROLLING)
	{
		update();
	}
//	if(RECORDING){ //////NOW INSIDE update()...NEED TO UNCOMMENT FOR ACCN TESTS ETC
//		x = get_position(CCD_Buffer);
//		c = get_current();
//		POSITIONS[DATA_INDEX] = x;
//		CURRENTS[DATA_INDEX] = c;
//		VOLTAGES[DATA_INDEX] = get_voltage();
//		DATA_INDEX++;
//		if(DATA_INDEX == N_DATA){
//			RECORDING = 0;
//		}
//	}

}

