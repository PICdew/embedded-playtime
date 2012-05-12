/*

code written so my mate could put LEDs in his beard for a beard competition

designed for a PIC12F ... something, can't remember the exact model

connect a push to make switch on GPIO2 and the interrupt will change the flash mode

connect LEDs across from GPIO4 and 5 down to ground

*/

#include <htc.h>	//this is the header that does all the register mapping
int result, i, j, mode;
mode = 0;
char res;

void interrupt switchpressed(void){
	
	mode = mode+1;
	if(mode > 2)
		mode = 0;

	INTF = 0;
	//INTE = 0;
	return;
}


void init(void)		//main takes no argument and returns no argument
{
	GPIO = 0b00000000;
	TRISIO = 0b00000100;// 1 = input 0 = output
	ANSEL = 0b00000000;
	GIE = 1;
	//GPIE = 1;
	INTE = 1;
}


void main(void)
{
	init();						//setup ports	
		
	while(1 == 1){
		if(mode == 0){
			_delay(50000);
			GPIO = 0b00010000;
			_delay(50000);
			GPIO = 0b00000000;
			_delay(50000);
			GPIO = 0b00010000;
			_delay(50000);
			GPIO = 0b00000000;
			_delay(50000);
			GPIO = 0b00100000;
			_delay(50000);
		}
		if(mode == 1){
			_delay(50000);
			GPIO = 0b00010000;
			_delay(50000);
			GPIO = 0b00100000;
		}

		if(mode == 2){
			_delay(100000);
			GPIO = 0b00010000;
			_delay(100000);
			GPIO = 0b00100000;
		}

	}
}

