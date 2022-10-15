#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define get_bit(reg,bitnum) ((reg & (1<<bitnum))>>bitnum)

volatile float duty_cycle =0;
const int toggleDirectionButton = 2;

int main(void)
{
	DDRA= 0x03;	// Setting first two pins of port A as output for controlling the driver
	PORTA = 0x05;	// Enable Pull-up resistor for pin 2 and initiating direction 01 for the driver
	DDRB |= (1<<PINB3);	// Setting pin 3 of port b as output for PWM signal generated at OC0

	// TIMER0
	TCCR0=0b01110101;	//Configure Timer0 to run at Phase correct PWM
	
	//EXTERNAL INTs
	PORTD=0b00001100;	// Enable Pull-up for INT 0 and INT 1
	MCUCR = 0b00001010; // INT 0 and INT 1 will be triggered when falling edge signal arrives
	GICR = 0b11000000;  // Enable INT 0 and INT 1
	sei();
	
	
	while(1)
	{	
		
		if(get_bit(PINA,toggleDirectionButton) == 0)
		{
			PORTA ^= (1<<PINA0)|(1<<PINA1);
			_delay_ms(500);
		}
		OCR0 = (uint8_t)(255* (1-duty_cycle));
	}

	return 0;
}
ISR(INT0_vect)
{
	//Increase the speed by 10%
	if(duty_cycle>0)
		duty_cycle -=0.1;
}
ISR(INT1_vect)
{
	//Decrease the speed by 10%
	if(duty_cycle<1)
		duty_cycle +=0.1;
}
