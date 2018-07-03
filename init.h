/*
 * init.h
 *
 *  Created on: 03.07.2018
 *      Author: vikulya96
 */

#ifndef INIT_H_
#define INIT_H_

#endif /* INIT_H_ */

#include "main.h"
#include <avr/io.h>

void INT01_Init();
void TIM0_Init();	//инициализация таймера 0 (ШИМ по двум каналам)
void TIM1_Init();	//инициализация таймера 1 (счётчик)
void IO_Init();		//настройка выводов
void UART_Init();	//настройка UART
void ADC_Init();	//настройка АЦП
