/*

Author: Simon Barker

Created: 11/5/2012

Purpose: Read accelerometer data from ADXL345 and send to laptop over serial

*/
#include "mbed.h"
#include "ADXL345sb.h"

DigitalOut myled(LED1);
ADXL345sb accel(p5, p6, p7, p8); // mosi, miso, sclk, ncs

Serial pc(USBTX, USBRX);         // tx, rx

int main() {

    float readings[3];
    
    while(1) {       
        wait(0.1);
       
        accel.adxlwrite(DATA_FORMAT,0x01);
        accel.adxlwrite(POWER_CTL ,0x08);
        
        accel.adxlreadXYZ(readings);
        
        float angleX = acos(readings[0]);
        float angleY = acos(readings[1]);
        float angleZ = acos(readings[2]);
        
        if(isnan(angleX))
            angleX = 0;
        if(isnan(angleY))
            angleY = 0;
        if(isnan(angleZ))
            angleZ = 0;
               
        printf("%f,%f,%f,\r\n",angleX,angleY,angleZ);
    }
}
