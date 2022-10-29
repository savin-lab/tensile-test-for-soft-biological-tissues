#include "main.h"

static void SystemClock_Config(void);
static void Error_Handler(void);

int mode = 0;

int main(void)
{

	ADC_READY = 0;
	RECORDING = 0;
	CONTROLLING = 0;
	DATA_INDEX = 0;
	HAL_Init();
	SystemClock_Config();

	Setup_Motor_Timer();
	Setup_I2C();
	Setup_UART();
	Setup_fM_Timer();
	Setup_ADC_Trig_Timer();
	Setup_ICG_SH_Timer();
	Setup_ADC(CCD_Buffer);

	switch(mode) {
		case 0 : ///// Pass/allow interrupts to handle stuff
		{
			uint8_t command = 0;
			R = 15000;
			I_max = 0.4;
			CONTROLLING = 1;
			while(1){
				HAL_UART_Receive(&hUART, &command, 1, 0xFFFF);
				HAL_Delay(500);
				if(command){
					RECORDING = 1;
					HAL_Delay(N_DATA*1600/200);
					printf("\n\r STARTING DATA READOUT");
					for(int i = 0; i<N_DATA; i++){
						printf("\n\r%d, %d, %d", REFS[i], POSITIONS[i], CURRENTS[i]);
					}
					printf("\n\r END OF DATA READOUT");
					DATA_INDEX  = 0;
					HAL_Delay(100);
					command = 0;
					R = 15000;
				}
			}
			break;
		}
		case 1 : ///// Test Electromagnet
		{
			int current;
			int voltage;
			int i = 0;
			while(1){
				i++;
				i = i%100;
				//i = 99;
				__HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_1, i);
				HAL_Delay(1000);
				current = get_current();
				voltage = get_voltage();
				printf("\n\r PWM %d Current %d Voltage %d", i, current, voltage);
			}
			break;
		}
		case 2 : //// TEST POSITION ALGORITHM
		{
			uint32_t t1; uint32_t t2; short x;
			while(1){
				HAL_Delay(500);
				ADC_READY = 0;
				HAL_Delay(50);
				t1 = HAL_GetTick();
				for(short i = 100; i>0; i--){
					x = get_position(CCD_Buffer);
				}
				t2 = HAL_GetTick();
				printf("\n\r Position is %d, duration %d", x,t2-t1);
				HAL_Delay(50);
				ADC_READY = 1;
			}
			break;
		}
		case 3 : /// Oscilloscope - use with scope.py
		{
			uint8_t command = 0;
			while(1){
				HAL_UART_Receive(&hUART, &command, 1, 0xFFFF);
				if(command){
					ADC_READY = 0;
					HAL_Delay(50);
					HAL_UART_Transmit(&hUART, CCD_Buffer, CCD_LENGTH, 0xFFFF);
					HAL_Delay(50);
					ADC_READY =1;
					command = 0;
				}
			}
			break;
		}
		case 4 : /////acceleration test
		{
			uint8_t command = 0;
			int current;
			int voltage;
			uint32_t t1; uint32_t t2;
			__HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_1, 0);
			while(1){
				HAL_UART_Receive(&hUART, &command, 1, 0xFFFF);
				HAL_Delay(500);
				if(command){
					RECORDING = 1;
					__HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_1, 99);
					HAL_Delay(N_DATA*1000/150);
					printf("\n\r STARTING DATA READOUT");
					for(int i = 0; i<N_DATA; i++){
						printf("\n\r%d, %d, %d",POSITIONS[i], CURRENTS[i], VOLTAGES[i]);
					}
					t1 = HAL_GetTick();
					current = get_current();
					voltage = get_voltage();
					t2 = HAL_GetTick();
					printf("\n\r Current is %d, voltage is %d, I2C time is %d", current, voltage, t2-t1);
					printf("\n\r END OF DATA READOUT");
					__HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_1, 0);
					DATA_INDEX  = 0;
					HAL_Delay(100);
					command = 0;
					}
			}
			break;
		}
	}


}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&hUART, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	/* Enable HSI Oscillator and activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 0x10;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

static void Error_Handler(void)
{
  while(1)
  {
  }
}
