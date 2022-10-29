#include "main.h"

void Setup_UART(void){

	hUART.Instance          = USART2;

	hUART.Init.BaudRate     = 57600;
	hUART.Init.WordLength   = UART_WORDLENGTH_9B;
	hUART.Init.StopBits     = UART_STOPBITS_1;
	hUART.Init.Parity       = UART_PARITY_ODD;
	hUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	hUART.Init.Mode         = UART_MODE_TX_RX;
	hUART.Init.OverSampling = UART_OVERSAMPLING_16;

	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	GPIO_InitStruct.Pin       = GPIO_PIN_2; //TX
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3; //RX
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	HAL_UART_Init(&hUART);
}


void USART2_IRQHandler(void){

	  HAL_UART_IRQHandler(& hUART);

}
