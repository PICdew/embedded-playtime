/*

This is the transmitter half of a system which was designed to detect when a light sensor had not
been dark for a certain period of time and then transmit a signal. The idea was just to mess about
with simple 433MHz transmitter and receiver, this system could be used as rhe base to a system designed 
to turn off a clothes iron after a certain period of time if left on, the light sensor would detect
if it had been covered by a hand and if not then transmit the off signal.

Really the transmit code should be more rubust than a simple RS232 protocol but this works, quick and dirty
but it does work.

PIC16F886

This was my first piece of pic coding

*/
#include <htc.h>	//this is the header that does all the register mapping

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
	SPBRG = 103;
	SPBRGH = 0;
	BAUDCTL = BAUDCTL | 0b00001000;
	TXSTA = TXSTA && 0b11101011;
	SPEN = 1;
	TXEN = 1;
}
/* Return an 8 bit result from AN0*/
unsigned char read_a2d_ch0(){
	ADCON0	= 0b00000011;		//set to channel 0 and enable adc module	
	while((ADCON0 && 0b00000010) == 0b00000010)continue;		// loop until complete
	return(ADRESH);				// return 8 MSB of the result
}
/* Return an 8 bit result from AN1*/
unsigned char read_a2d_ch1(){
	ADCON0	= 0b00000111;		//set to channel 0 and enable adc module	
	while((ADCON0 && 0b00000010) == 0b00000010)continue;		// loop until complete
	return(ADRESH);				// return 8 MSB of the result
}

void main(void)
{

	char a, b, c, txd=1;
	char array[4];
	int result, i;
	unsigned long counter = 0, timerResult = 0;
	init();						//setup ports	
	
	array[0] = 'a';
	array[3] = 'z';

	PORTA = 0b00000000;

	while(1 == 1) {	

		result = read_a2d_ch0();	//read "sensor" pin
		if(result < 100) {
			counter = 0;
		}
		else {
			counter++;
		}
		timerResult = read_a2d_ch1();	//read timer pin

		if(counter == timerResult) {
			//tx o to turn off power
			PORTA = 0b00000100;	//turn on transmitter

			eusart_init();
			array[1] = 'o';//result;
			array[2] = 'o'^0b11111111;//result^0b11111111;
			
			for(int j = 0; j <50; j++) {
				for(i = 0; i<4; i++) {
					while(TRMT == 0)continue;
					TXREG = array[i];
				}
			}
			txd = 1;
			PORTA = 0b00000000;	//turn off transmitter
		}
		if(counter == 0) {
			if(txd == 1) { //tx g to turn on power
				PORTA = 0b00000100;
			//	_delay(200);
				for(int j = 0; j<50; j++) {
					array[1] = 'g';
					array[2] = 'g'^0b11111111;

					for(i = 0; i<4; i++) {
						while(TRMT == 0)continue;
						TXREG = array[i];
					}
				}
				PORTA = 0b00000000;
			}
			txd = 0;
		}
		_delay(20000);
	}
}
