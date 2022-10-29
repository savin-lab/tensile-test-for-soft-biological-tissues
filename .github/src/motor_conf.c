#include "main.h"

void Setup_Motor_Timer(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_OC_InitTypeDef 	sConfig;
	GPIO_InitTypeDef  	GPIO_InitStruct;

	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Configure the GPIO TIM1 Ch1 (PA8) */
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Initialise TIM1 */
	htim_motor.Instance = TIM1;
	htim_motor.Init.Prescaler = 4200-1;  // 2 MHz
	htim_motor.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_motor.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_motor.Init.Period = 100-1; //2 MHz / 100 = 20 kHz //MAYBE INCREASE FOR MORE RESOLUTION?
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.Pulse = 0;
	sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfig.OCFastMode = TIM_OCFAST_ENABLE;

	HAL_TIM_Base_Init(&htim_motor);
	HAL_TIM_ConfigClockSource(&htim_motor, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim_motor);
	HAL_TIM_PWM_ConfigChannel(&htim_motor, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim_motor, TIM_CHANNEL_1);

}
