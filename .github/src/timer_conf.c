#include "main.h"

/* fM Timer: 4 MHz signal, 50% PWM
 * Timer 4 Channel 1
 * Pin pb6
 */
void Setup_fM_Timer(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_OC_InitTypeDef 	sConfig;
	GPIO_InitTypeDef  	GPIO_InitStruct;

	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure the GPIO TIM4 Ch1 (PB6) */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Initialise TIM4 */
	htim_fM.Instance = TIM4;
	htim_fM.Init.Prescaler = 1-1;
	htim_fM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_fM.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_fM.Init.Period = 21-1;//42-1=>84 MHz/42 = 2 MHz *or* 21 for 4 MHz
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = 10;/////21; or 10; half of period
	sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfig.OCFastMode = TIM_OCFAST_ENABLE;

	HAL_TIM_Base_Init(&htim_fM);
	HAL_TIM_ConfigClockSource(&htim_fM, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim_fM);
	HAL_TIM_PWM_ConfigChannel(&htim_fM, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim_fM, TIM_CHANNEL_1);

}

/* ADC Tirgger Timer: 1 MHz signal, 50% PWM
 * Timer 3 Channel 1
 * No Pin
 */

void Setup_ADC_Trig_Timer(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_OC_InitTypeDef 	sConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	__HAL_RCC_TIM3_CLK_ENABLE();

	GPIO_InitTypeDef  	GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure the GPIO TIM3 Ch1 (PB4) */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Initialise TIM3 Ch1 */
	htim_adctrig.Instance = TIM3;
	htim_adctrig.Init.Prescaler = 1-1;
	htim_adctrig.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_adctrig.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_adctrig.Init.Period = 84;///////168-1=>84 MHz/168 = 0.5MHz ***OR** 84 for 1 MHz
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = 42;/////////84;42; half of period
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode = TIM_OCFAST_ENABLE;

	//HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(TIM3_IRQn);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim_adctrig, &sMasterConfig);

	HAL_TIM_ConfigClockSource(&htim_adctrig, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim_adctrig);
	HAL_TIM_PWM_ConfigChannel(&htim_adctrig, &sConfig, TIM_CHANNEL_1);
	//__HAL_TIM_ENABLE_IT(&htim_ICG, TIM_IT_CC3);
	HAL_TIM_PWM_Start(&htim_adctrig, TIM_CHANNEL_3);
	//__HAL_TIM_DISABLE(&htim_adctrig);
	TIM3->CR1 &= (uint16_t)~TIM_CR1_CEN; //Wait to be enabled by IG pulse interrupt
}


/* ICG Timer: 100 Hz signal, 5us Pulse
 * Timer 2 Channel 1
 * Pin PA0
 *
 * SH Timer: 50 us int time = 20kHz signal, 2.5us Pulse
 * Timer 5 Channel 2
 * Pin PA1
 */
void Setup_ICG_SH_Timer(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_OC_InitTypeDef 	sConfig;
	GPIO_InitTypeDef  	GPIO_InitStruct;

	__HAL_RCC_TIM5_CLK_ENABLE();

	/* Configure SH the GPIO TIM5 Ch2 (PA1) */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Initialise TIM5 */
	htim_SH.Instance = TIM5;
	htim_SH.Init.Prescaler = 42-1; //2 MHz base
	htim_SH.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_SH.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_SH.Init.Period = 40-1; // 100 = 20 KHz, 50 us integration time ****** 50 ->25 us **** 40 -> 20 us
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = 5; // 2.5 us pulse
	sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfig.OCFastMode = TIM_OCFAST_ENABLE;

	HAL_TIM_Base_Init(&htim_SH);
	HAL_TIM_ConfigClockSource(&htim_SH, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim_SH);
	HAL_TIM_PWM_ConfigChannel(&htim_SH, &sConfig, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim_SH, TIM_CHANNEL_2);

	__HAL_RCC_TIM2_CLK_ENABLE();

	/* Configure the ICG GPIO TIM2 Ch1 (PA0) ICG*/
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Initialise TIM2 ICG*/
	htim_ICG.Instance = TIM2;
	htim_ICG.Init.Prescaler = 42-1;  //2 MHz
	htim_ICG.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_ICG.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_ICG.Init.Period = 10000-1;///////////20000-1 => 100 Hz or 10000 for 200 Hz

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = 10; //5 us
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode = TIM_OCFAST_ENABLE;

	HAL_TIM_Base_Init(&htim_ICG);
	HAL_TIM_ConfigClockSource(&htim_ICG, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim_ICG);
	HAL_TIM_PWM_ConfigChannel(&htim_ICG, &sConfig, TIM_CHANNEL_1);
	__HAL_TIM_ENABLE_IT(&htim_ICG, TIM_IT_UPDATE);
	HAL_TIM_PWM_Start(&htim_ICG, TIM_CHANNEL_1);

	TIM5->CNT = 40-12;
	TIM2->CNT = 10000-11;//20000-11;
	TIM4->CNT = 3;

}


void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim_ICG);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim == &htim_ICG)
	{
		//Enable TIMER 3 to start ADC triggering and hence DMA
		if(ADC_READY)
		{
			__HAL_TIM_ENABLE(&htim_adctrig);
		}

	}

}


