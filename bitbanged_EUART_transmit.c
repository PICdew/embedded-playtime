/*

Very simple bit banged UART transmit function with a baud rate of 4800 (ish)

*/
void init(void){
   //8 MHz
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF0 = 1;

    //set as outputs
    TRISIO = 0b00001000;

    //set output state
    GP2 = 1;
}

void eusartWriteBit(int b) {
    b = b & 0x01;
    if (b) {
        // Write '1' bit
        GP2 = 1;
        __delay_us(208);        //baud rate = 4800
    } else {
        GP2 = 0;
        __delay_us(208);
    }
}

void eusartWriteByte(char data) {
    // Loop to write each bit in the byte, LS-bit first
    GP2 = 0;
    __delay_us(100);
    for (int loop = 0; loop < 8; loop++) {
        eusartWriteBit(data & 0x01);

        // shift the data byte for the next bit
        data >>= 1;
    }
    GP2 = 1;
    __delay_us(208);
}

int main(){
    init();

    eusartWriteByte('c');

    return 0;
}