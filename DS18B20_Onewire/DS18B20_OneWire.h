#ifndef _onewire_h
#define _onewire_h

#define onewire RA2		//set which port pin you want use
#define onewireDirection TRISA2 // 1 = input, 0 = output

void onewireWriteBit(int b);

int onewireReadBit();

int onewireInit();

int onewireReadByte();

void onewireWriteByte(char data);

unsigned char onewireCRC(unsigned char* addr, unsigned char len);

void printSingleAddress();

void broadcastConvert();

float getTemperature(unsigned char* address);

#endif