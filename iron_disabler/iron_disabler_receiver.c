/*

This is the other half of the iron disabler code that deals with receiving the packet and switching the mains relaying on
or off.

Again, you wouldn't really want to use an RS232 protocol to do this over the 433MHz link but it worked for this example.

PIC16F886

This was my first piece of pic coding

*/
#include <htc.h>	//this is the header that does all the register mapping

int rx, i = 4, data1XOR, data1;

void init(void)		
{
	//portA
	TRISA = 0b00000011;		//0 = output 1 = input
	ANSEL = 0b00000001;
	ANSELH = 0b00001111;

	//set up adc
	ADCON0 = 0b00000001;	//adc is set up on ANO and is on but not operating
	ADCON1 = 0b00000000;
	
	//portB and C
	TRISB = 0b00000000;		//0 = output 1 = input
	TRISC = 0b00000000;		//0 = output 1 = input

	//pwm
	PR2 = 0b11100000;
	CCP1CON = 0b11101101;
	T2CON = 0b00111100;
}

//init eusart
void eusart_init() {
	SPBRG = 103;					//Baud = 2400 if SPBRG = 103 and 9600 if SPBRG = 25
	SPBRGH = 0;
	BAUDCTL = BAUDCTL | 0b00001000;
	TXSTA = TXSTA && 0b11101011;
	SPEN = 1;
	TXEN = 1;
	RX9 = 0;
	CREN = 1;
}

//interupt test
void interrupt flash(void) {
	if(RCIF == 1) {					//check what caused the interrupt
		if(OERR == 1) {				//check for an over run error and do the require clean up
			CREN = 0;				
			CREN = 1;
		}
		rx = RCREG;					//read in RX character
		
		//check for 'packet'
		if(rx == 'a') {
			i = 0;
		}
		if(i == 0 && rx != 'a') {
			data1 = rx;
			i = 1;
		}
		if(i == 1 && data1 != rx) {
			data1XOR = rx;
			i = 2;
		}
		if(rx == 'z' && i == 2) {
			i = 3;
			rx = 0;		
		}
	}
	return;
}

void main(void)
{
	int d = 0;
	char a, b, c, rx2;
	int result;
	init();						//setup ports	
	
	//set up interupts
	RCIE = 1;
	PEIE = 1;
	GIE = 1;

	PORTA = 0b00010000;
	a = 100;
	b = 0;
	pwmInit(a,b);
	eusart_init();
	while(1 == 1) {	
		if(i == 3) {
			if(data1XOR^data1 == 256) {
				if(data1 == 'g')
					PORTA = 0b00010000;
				if(data1 == 'o')
					PORTA = 0b00000000;
			i = 4;
			}
		}
	}
}
