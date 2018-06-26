#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

#define FACT_COUNT 1000000	//множитель, отвечающий за точность измерения управл.имп.
#define F_CPU	16000000UL	//для delay
#define F_CPU_calc	16000000  // 16 MHz [для упрощения расчётов F_CPU/FACT_COUNT]
#define TIM_PRESCALER	1	//предделитель таймера
#define	TIM_COUNT_MAX	65536
#define TIM_PWM_MAX	256

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define PIN_PPM	PIND
#define PPM1	2
#define PPM2	3

#define LED_DDR	DDRB
#define LED_PORT	PORTB
#define LED1	PB5
#define LED2	PB1

#define PWM_DDR	DDRD
#define PWM_PORT	PORTD
#define PWM1	PD6
#define PWM2	PD5

#define CONTROL_PULS_MAX	2000 //2мс * FACT_COUNT	максимальное значение управляющиего импульса
#define CONTROL_PULS_MIN	1000	//1мс * FACT_COUNT	минимальное -||-
#define CONTROL_PULS_NUL	1500
#define TIM_PWM_CONST 1.95	//множитель 1.95 при 1000..2000

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
		LED_PORT ^= (1<<LED1);
	}
	else
	{
		t02 = TCNT1;
		del_T1 = (t02 + (OVF_count1*TIM_COUNT_MAX) - t01); //записываем значение счётчика
		t_puls_PPM1 = del_T1 *FACT_COUNT*TIM_PRESCALER/F_CPU_calc;

		del_T1H = (t_puls_PPM1>>8);
		del_T1L = t_puls_PPM1;


		T_puls_PPM1 = t_puls_PPM1 - CONTROL_PULS_NUL;

	/*	if ((t_puls_PPM1<=CONTROL_PULS_MAX) & (t_puls_PPM1>=CONTROL_PULS_MIN))
			t_puls_PPM1 = t_puls_PPM1 - CONTROL_PULS_NUL;
		else t_puls_PPM1 = 0;*/

	}
}

ISR(INT1_vect)
{
	if (PIN_PPM&(1<<PPM2))	//если на PPM1 HIGH
		{
			OVF_count2 = 0;
			t11 = TCNT1;
		}
		else
		{
			t12 = TCNT1;
			del_T2 = t12 + OVF_count2*TIM_COUNT_MAX - t11; //записываем значение счётчика
			t_puls_PPM2 = del_T2 *FACT_COUNT*TIM_PRESCALER/F_CPU_calc;
		}
	t_puls_PPM2 = t_puls_PPM2 - CONTROL_PULS_NUL;
	/*if ((t_puls_PPM2<=CONTROL_PULS_MAX) &(t_puls_PPM2>=CONTROL_PULS_MIN))
		t_puls_PPM2 = t_puls_PPM2 - CONTROL_PULS_NUL;
	else t_puls_PPM2 = 0;*/

}

//таймер 0
ISR(TIMER1_OVF_vect)	//по переполнению
{
	OVF_count1++;
	OVF_count2++;
}

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

	//предделитель
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00)|(0<<WGM02);
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

void IO_Init()
{
	LED_DDR = (1<<LED1)|(1<<LED2);	//светодиоды
	PWM_DDR = (1<<PWM1)|(1<<PWM2);	//выводы ШИМ от таймера 0
}

void UART_Init(unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) );
/* Put data into buffer, sends the data */
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

	OCR0A = 0;
	OCR0B = 0;

	sei();	//глобальное разрешение прерываний


	while(1)
	{

	/*	if (t_puls_PPM1>=0)
		{

		}
		else
		{

		}*/

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

		/*PWM_count1 = del_T1;
		PWM_count2 = del_T2;

		if (PWM_count1 < (TIM_PWM_MAX - 1))
			OCR0A = PWM_count1;	//на выводе PWM1
		else OCR0A = (TIM_PWM_MAX - 1);
		if (PWM_count2 < (TIM_PWM_MAX - 1))
			OCR0B = PWM_count2;	//на выводе PWM2
		else OCR0B = (TIM_PWM_MAX - 1);*/

		/*USART_Transmit(del_T1H);
		USART_Transmit(del_T1L);*/
		/*USART_Transmit(PWM_count1);

		_delay_ms(2000);*/
	}

}

