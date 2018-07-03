/*
 * main.h
 *
 *  Created on: 03.07.2018
 *      Author: vikulya96
 */

#ifndef MAIN_H_
#define MAIN_H_

#endif /* MAIN_H_ */

//для расчётов
#define FACT_COUNT 1000000	//множитель, отвечающий за точность измерения управл.имп.
#define F_CPU	16000000UL	//для delay
#define F_CPU_calc	16000000  // 16 MHz
#define TIM_PRESCALER	1	//предделитель таймера
#define	TIM_COUNT_MAX	65536
#define TIM_PWM_MAX	256
#define CONTROL_PULS_MAX	2000 //2мс * FACT_COUNT	максимальное значение управляющиего импульса
#define CONTROL_PULS_MIN	1000	//1мс * FACT_COUNT	минимальное -||-
#define CONTROL_PULS_NUL	1500
#define TIM_PWM_CONST 1.95	//множитель 1.95 при 1000..2000

//для UART
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

//выводы управляющего сигнала
#define PIN_PPM	PIND
#define PPM1	2
#define PPM2	3

//выводы, подключенные к светодиодам
#define LED_DDR	DDRB
#define LED_PORT	PORTB
#define LED1	5
#define LED2	1

//выводы драйвера
//ШИМ
#define PWM_DDR	DDRD
#define PWM_PORT	PORTD
#define PWM1	6
#define PWM2	5
//Вывод включения драйвера
#define EN_DDR	DDRC
#define EN_PORT	PORTC
#define EN1	0
#define EN2	3
//Выводы направления вращения
#define IN1_DDR	DDRC
#define IN1_PORT	PORTC
#define INA1	1
#define INB1	2
#define IN2_DDR	DDRC
#define IN2_PORT	PORTC
#define INA2	4
#define INB2	5
//Вывод с датчика тока
#define CS_DDR	DDRD
#define CS_PORT	PORTD
#define CS1	7
#define CS2	7

//void USART_Transmit();
