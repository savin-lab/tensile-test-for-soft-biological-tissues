/*
 * main.h
 *
 *  Created on: 28 Jan 2018
 *      Author: Laurence
 */

#ifndef MAIN_H_
#define MAIN_H_

/////INCLUDE EVERYTHING
#include "math.h"
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "timer_conf.h"
#include "adc_conf.h"
#include "uart_conf.h"
#include "motor_conf.h"
#include "i2c_conf.h"
#include "controller.h"
#include "reference.h"
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif


#define CCD_LENGTH 3694
uint8_t CCD_Buffer[CCD_LENGTH];
//short x; // ball position
//int c; //current /////////NOW DEFINED LOCALLY

////FOR DATA LOGGING
uint8_t RECORDING;
uint8_t CONTROLLING;
short DATA_INDEX;
#define N_DATA 5000
short REFERENCES[N_DATA];
short POSITIONS[N_DATA];
int CURRENTS[N_DATA];
int VOLTAGES[N_DATA];
#endif /* MAIN_H_ */
