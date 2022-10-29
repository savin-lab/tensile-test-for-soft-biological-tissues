/*
 * timer_conf.h
 *
 *  Created on: 14 Dec 2017
 *      Author: Laurence
 */

#ifndef TIMER_CONF_H_
#define TIMER_CONF_H_

void Setup_fM_Timer(void);
void Setup_ICG_SH_Timer(void);
void Setup_ADC_Trig_Timer(void);

TIM_HandleTypeDef	htim_fM;
TIM_HandleTypeDef	htim_adctrig;
TIM_HandleTypeDef	htim_SH;
TIM_HandleTypeDef	htim_ICG;

#endif /* TIMER_CONF_H_ */
