#include "ADXL345sb.h"

ADXL345sb::ADXL345sb(PinName mosi, PinName miso, PinName sck, PinName cs) : spi_(mosi, miso, sck), nCS_(cs) {

    nCS_ = 1;
    
    spi_.frequency(1000000);
    spi_.format(8,3);

    wait_us(500);

}

int ADXL345sb::adxlread(int address){
    int d;
    //address MSB must be one as it's a read
    address = address | 0x80;
    nCS_ = 0;
    spi_.write(address);
    d = spi_.write(0x00);
    nCS_ = 1;   
    return d;
}
void ADXL345sb::adxlmultibyteread(int address, int numOfBytes, unsigned char *buf){

    //address MSB must be one as it's a read
    address = address | 0xC0;
    nCS_ = 0;
    spi_.write(address);
    for(int i = 0; i < numOfBytes; i++)
        buf[i] = spi_.write(0x00);
    nCS_ = 1;   
}
void ADXL345sb::adxlwrite(int address, int data){
    nCS_ = 0;
    
    spi_.write(address);
    spi_.write(data);
    
    nCS_ = 1;
}