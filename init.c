/*
 * init.c

 *
 *  Created on: 03.07.2018
 *      Author: vikulya96
 */

#include <init.h>

//настройка прерываний INT0 и INT1
//INTn	ISCn1 ISCn0: 10 - спад; 11 - фронт; 01 - любой перепад
void INT01_Init()
{
	EICRA = (0<<ISC11)|(1<<ISC10)|(0<<ISC01)|(1<<ISC00);
	EIMSK = (1<<INT1)|(1<<INT0);	//разрешение прерываний
}


//инициализация таймера 0 (ШИМ по двум каналам)
void TIM0_Init()
{
	//предделитель 8 (~7кГц)
	TCCR0B = (0<<CS02)|(1<<CS01)|(0<<CS00)|(0<<WGM02);
	//COM0A1 COM0A0: 10 - non-inverting mode; 11 - inverting mode
	TCCR0A = (1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0)|(1<<WGM01)|(1<<WGM00);
	OCR0A = 0;
	OCR0B = 0;
}

//инициализация таймера 1 (счётчик)
void TIM1_Init()
{
	//предделитель 8 [если без предделителя, сменить CS11 на CS10]
	TCCR1B = (1<<CS10);
	TIMSK1 = (1<<TOIE1);
}

//настройка выводов
void IO_Init()
{
	LED_DDR = (1<<LED1)|(1<<LED2);	//светодиоды
	PWM_DDR = (1<<PWM1)|(1<<PWM2);	//выводы ШИМ от таймера 0
	EN_DDR = (1<<EN1)|(1<<EN2);
	IN1_DDR = (1<<INA1)|(1<<INB1);
	IN2_DDR = (1<<INA2)|(1<<INB2);

	EN_PORT = (1<<EN1)|(1<<EN2); //включение драйвера
}

//настройка UART
void UART_Init(unsigned int ubrr)
{
	//Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

/*
void ADC_Init() //изменить Vref на внешний (1<<REF0)
{
	ADMUX = (0<<REF0)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADIE);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	//предделитель 128
}*/
