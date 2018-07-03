#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include <init.h>

unsigned char OVF_count1 = 0, OVF_count2 = 0, del_T1H, del_T1L;
unsigned int t01 = 0, t02 = 0, t11 = 0, t12 = 0, PWM_count1 = 0, PWM_count2 = 0,
		del_T1 = 0, del_T2 = 0, t_puls_PPM1 = 0, t_puls_PPM2 = 0;
int T_puls_PPM1, T_puls_PPM2;
double time_ms=0;

//обработка прерываний
//внешние
ISR(INT0_vect)
{
	if (PIN_PPM & (1<<PPM1))	//если на PPM1 HIGH
	{
		OVF_count1 = 0;
		t01 = TCNT1;
	}
	else
	{
		t02 = TCNT1;
		del_T1 = (t02 + (OVF_count1*TIM_COUNT_MAX) - t01); //записываем значение счётчика
		t_puls_PPM1 = del_T1 *FACT_COUNT*TIM_PRESCALER/F_CPU_calc;
		T_puls_PPM1 = t_puls_PPM1 - CONTROL_PULS_NUL;
	}
}

ISR(INT1_vect)
{
	if (PIN_PPM & (1<<PPM2))	//если на PPM2 HIGH
		{
			OVF_count2 = 0;
			t11 = TCNT1;
		}
		else
		{
			t12 = TCNT1;
			del_T2 = (t12 + (OVF_count2*TIM_COUNT_MAX) - t11); //записываем значение счётчика
			t_puls_PPM2 = del_T2 *FACT_COUNT*TIM_PRESCALER/F_CPU_calc;
			T_puls_PPM2 = t_puls_PPM2 - CONTROL_PULS_NUL;
		}
}

//таймер 0
ISR(TIMER1_OVF_vect)	//по переполнению
{
	OVF_count1++;
	OVF_count2++;
}

void USART_Transmit( unsigned char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
	// Put data into buffer, sends the data
	UDR0 = data;
}

//основная программа
int main (void)
{
	//инициализация
	TIM0_Init();
	TIM1_Init();
	INT01_Init();
	IO_Init();
	UART_Init(MYUBRR);

	//обнуление регистров, устанавливающих скважность ШИМ
	OCR0A = 0;
	OCR0B = 0;

	sei();	//глобальное разрешение прерываний


	while(1)
	{
		//проверка знака, определение направления вращения
		if (T_puls_PPM1 >= 0)
			IN1_PORT = (1<<INA1)|(0<<INB1);
		else
			IN1_PORT = (0<<INA1)|(1<<INB1);

		if (T_puls_PPM2 >= 0)
			IN2_PORT = (1<<INA2)|(0<<INB2);
		else
			IN2_PORT = (0<<INA2)|(1<<INB2);

		//расчёт скважности ШИМ в соответствии с управляющим импульсом
		PWM_count1 = abs(T_puls_PPM1)/TIM_PWM_CONST;
		PWM_count2 = abs(T_puls_PPM2)/TIM_PWM_CONST;

		/*del_T1H = (PWM_count1>>8);
		del_T1L = PWM_count1;*/

		//изменение скважности ШИМ 0..255
		if (PWM_count1 <= (TIM_PWM_MAX - 1))
			OCR0A =  PWM_count1;	//на выводе PWM1
		else OCR0A = TIM_PWM_MAX - 1;
		if (PWM_count2 <= (TIM_PWM_MAX - 1))
			OCR0B = PWM_count2;	//на выводе PWM2
		else OCR0B = (TIM_PWM_MAX - 1);


		/*USART_Transmit(del_T1H);
		USART_Transmit(del_T1L);*/
	}
}

