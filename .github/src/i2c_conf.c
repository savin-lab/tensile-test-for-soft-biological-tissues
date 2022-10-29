#include "main.h"


void Setup_I2C(void){


	I2cHandle.Instance             = I2C1;
	I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	I2cHandle.Init.ClockSpeed      = 250000;
	I2cHandle.Init.OwnAddress1	   = 0x40;
	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* Enable I2C1 clock */
	__HAL_RCC_I2C1_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* I2C TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = GPIO_PIN_8;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* I2C RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*##-3- Configure the NVIC for I2C #########################################*/
	/* NVIC for I2C1 */
	HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 2);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

	HAL_I2C_Init(&I2cHandle);

    uint8_t subaddress = 0x00; //config register
    sendBuff[0] = 0x3F;
    sendBuff[1] = 0xFF;
	uint16_t I2C_ADDRESS = (uint16_t)slave_address<<1;
	//HAL_I2C_Mem_Write(&I2cHandle, I2C_ADDRESS, (uint16_t)subaddress, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&sendBuff, 2, 500);
	// DOUBLE CHECK THIS -- SEEMS TO BE INITIALISING TO 128 SAMPLE AVERAGING (WANT NO AVERAGING)
}

int get_current(void){

	uint8_t subaddress = 0x01;
	uint16_t I2C_ADDRESS = (uint16_t)slave_address<<1;
	uint16_t startstate = I2cHandle.State;
	HAL_StatusTypeDef return_val = HAL_I2C_Mem_Read(&I2cHandle, I2C_ADDRESS, (uint16_t)subaddress,
													I2C_MEMADD_SIZE_8BIT, (uint8_t*)&receiveBuff, 2, 5000);

	//int shuntV = ((((short)receiveBuff[0])<<8)| ((short)receiveBuff[1]));
	int shuntV = (int)((short)((short)receiveBuff[0]<<8) | receiveBuff[1]);
	return shuntV*100; //current in uA

}
int get_voltage(void){

	uint8_t subaddress = 0x02;
	uint16_t I2C_ADDRESS = (uint16_t)slave_address<<1;
	uint16_t startstate = I2cHandle.State;
	HAL_StatusTypeDef return_val = HAL_I2C_Mem_Read(&I2cHandle, I2C_ADDRESS, (uint16_t)subaddress,
													I2C_MEMADD_SIZE_8BIT, (uint8_t*)&receiveBuff, 2, 5000);

	//int shuntV = ((((short)receiveBuff[0])<<8)| ((short)receiveBuff[1]));
	int busV = (receiveBuff[0]*32+receiveBuff[1]/8)*4;//mV
	return busV;

}
