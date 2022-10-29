#ifndef ADC_CONF_H_
#define ADC_CONF_H_

void Setup_ADC(uint8_t* pBuff);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

ADC_HandleTypeDef    AdcHandle;
DMA_HandleTypeDef    DmaAdcHandle;
int ADC_READY;


#endif /* ADC_CONF_H_ */
