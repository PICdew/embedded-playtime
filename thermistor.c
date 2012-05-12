/*

Code to read and linarise a thermistor, pretty simple but fun to do on an afternoon

*/
//defines area
#define 	_XTAL_FREQ 8000000
//end defines

//includes
#include <htc.h>	//this is the header that does all the register mappings
#include <stdio.h>
#include <math.h>
#include "GenericTypeDeffs.h"


void init(void)		
{
	//portA
	TRISA = 0b00000001;		//0 = output 1 = input
	PORTA = 0b00000000;
	ANSEL = 0b00000001;
	ANSELH = 0b00001111;
	ADFM = 0;
	
	//set up Fosc = 8MHz
	IRCF0 = 1;
	IRCF1 = 1;
	IRCF2 = 1;
	
	FVREN = 1;
}

//init eusart
void eusart_init() {	
	SPBRG = 51;
	BRGH = 1;
	SYNC = 0;
	SPEN = 1;
	TXEN = 1;
}
//putch() required for printf() to work
void putch(char b){
	while(TRMT == 0)continue;
		TXREG = b;
}

int res1 = 0, res2 = 0, vref1 = 0, vref2 = 0;
WORD reading, tSample, vRefSample;

/* Return an 8 bit result */
WORD read_a2d(){
	ADCON0	= 0b10000001;		//set to channel 0 and enable adc module
	__delay_ms(2);
	GO_DONE = 1;
	while(GO_DONE == 1)continue;		// loop until complete
	WORD result = 0;
	result = ADRESH;
	result = result << 2;
	ADRESL = ADRESL >> 6;
	result = result + ADRESL;
	return result;
}
WORD read_a2d_vref(){
	ADCON0	= 0b10111101;		//set to channel vref and enable adc module
	__delay_ms(2);
	GO_DONE = 1;
	while(GO_DONE == 1)continue;		// loop until complete
	WORD result = 0;
	result = ADRESH;
	result = result << 2;
	ADRESL = ADRESL >> 6;
	result = result + ADRESL;
	return result;
}

void printStr(int *ptr, int length, char type){
	for(int i = 0; i < length; i++){
		if(type == 'h')
			printf("%x ",ptr[i]);
		if(type == 'c')
			printf("%c",ptr[i]);
		if(type == 'd')
			printf("%d ",ptr[i]);
		
	}
	printf("\r\n");	
}	

void interrupt intFunction(void){
	
	return;
}	
float voltage;
float T, Res, Vdd, I;

void main(void)
{
 	init();
 	eusart_init();

	printf("start:\r\n");

	while(1 == 1){
		
		vRefSample = read_a2d_vref();
		tSample = read_a2d();
		
		printf("%d %d ", vRefSample, tSample);
		
		Vdd = (0.574/(float)vRefSample)*1024;
		
		voltage = (float)tSample/1024;
		voltage = voltage * Vdd;
		voltage = Vdd - voltage;
		
		I = voltage / 960;
		Res = (Vdd - voltage);
		Res = Res / I;
		T = log(995/Res);
		T = T/3700;
		T = (1/297.15) + T;
		T = (1/T)-272.15;
		
		printf("%.2f %.2f \r\n", T, Vdd);
		
		__delay_ms(100);
		
		CLRWDT();		//watchdog is disabled in config but never hurts to have this in
	}
}