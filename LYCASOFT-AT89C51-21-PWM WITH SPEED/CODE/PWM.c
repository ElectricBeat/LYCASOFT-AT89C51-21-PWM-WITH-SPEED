#include<reg51.h>
#include "8051_delay.h"

sbit PWM_Pin = P2^0;

unsigned char PWM = 0;	  // It can have a value from 0 (0% duty cycle) to 255 (100% duty cycle)
unsigned int temp = 0;
 
#define PWM_Freq_Num 1	 // Highest possible PWM Frequency

/*  
 *	So, in general you can calculate PWM frequency by using the formula
 *     PWM Frequency = 1000000/(PWM_Freq_Num*255)
 */
 
void InitTimer0(void)
{
	TMOD &= 0xF0;    // Clear 4bit field for timer0
	TMOD |= 0x01;    // Set timer0 in mode 1 = 16bit mode

	TH0 = 0x00;      // First time value
	TL0 = 0x00;      // Set arbitrarily zero

	ET0 = 1;         // Enable Timer0 interrupts
	EA  = 1;         // Global interrupt enable

	TR0 = 1;         // Start Timer 0
}

void Timer0_ISR (void) interrupt 1   
{
	TR0 = 0;    // Stop Timer 0
 
	if(PWM_Pin)	// if PWM_Pin is high
	{
		PWM_Pin = 0;
		temp = (255-PWM)*PWM_Freq_Num;
		TH0  = 0xFF - (temp>>8)&0xFF;
		TL0  = 0xFF - temp&0xFF;	
	}
	else	     // if PWM_Pin is low
	{
		PWM_Pin = 1;
		temp = PWM*PWM_Freq_Num;
		TH0  = 0xFF - (temp>>8)&0xFF;
		TL0  = 0xFF - temp&0xFF;
	}
 
	TF0 = 0;     // Clear the interrupt flag
	TR0 = 1;     // Start Timer 0
}

void main()
{
	InitTimer0();
 
   while(1)
   {
			int i;
			i++;
			delay(10);
			PWM = i;
			if(i>=255)
				i=0;
	 }
}