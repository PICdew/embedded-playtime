void onewireWriteBit(int b) {
    b = b & 0x01;
    if (b) {
        // Write '1' bit
        onewireDirection = 0;
        onewire = 0;
        __delay_us(5);
        onewireDirection = 1;
        __delay_us(60);
    } else {
        // Write '0' bit
        onewireDirection = 0;
        onewire = 0;
        __delay_us(70);
        onewireDirection = 1;
        __delay_us(2);
    }
}

int onewireReadBit() {
    char result;

    onewireDirection = 0;
    onewire = 0;
    __delay_us(1);
    onewireDirection = 1;
    __delay_us(5);
    result = onewire;
    __delay_us(55);
    return result;

}

int onewireInit() {
    onewireDirection = 0;
    onewire = 0;
    __delay_us(480);
    onewireDirection = 1;
    __delay_us(60);
    if (onewire == 0) {
        __delay_us(100);
        return 1;
    }
    return 0;
}
int onewireReadByte() {
    int result = 0;

    for (int loop = 0; loop < 8; loop++) {
        // shift the result to get it ready for the next bit
        result >>= 1;

        // if result is one, then set MS bit
        if (onewireReadBit())
            result |= 0x80;
    }
    return result;
}
void onewireWriteByte(char data) {
    // Loop to write each bit in the byte, LS-bit first
    for (int loop = 0; loop < 8; loop++) {
        onewireWriteBit(data & 0x01);

        // shift the data byte for the next bit
        data >>= 1;
    }
}
unsigned char onewireCRC(unsigned char* addr, unsigned char len) {
    unsigned char i, j;
    unsigned char crc = 0;

    for (i = 0; i < len; i++) {
        unsigned char inbyte = addr[i];
        for (j = 0; j < 8; j++) {
            unsigned char mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }

    return crc;
}

void printSingleAddress() {
    onewireInit();
    //attach one sensor to port 25 and this will print out it's address
    unsigned char address[8]= {0,0,0,0,0,0,0,0};

    onewireWriteByte(0x33);

    for (int i = 0; i<8; i++)
        address[i] = onewireReadByte();
    for (int i = 0; i<8; i++)
        printf("0x%x,",address[i]);
    printf("\r\n");
    //check crc
    unsigned char crc = onewireCRC(address, 7);
    printf("crc = %x \r\n",crc);
}
void broadcastConvert() {
    //broadcast that temp conversions should begin, all at once so saves time
    onewireInit();
    onewireWriteByte(0xCC);
    onewireWriteByte(0x44);

    while (1) {
        if (onewireReadBit())
            break;
    }
}
float getTemperature(unsigned char* address) {
    //get temperature from the device with address address
    float temperature;
    int scratchPad[9] = {0,0,0,0,0,0,0,0,0};

    onewireInit();
    onewireWriteByte(0x55);
    for (int i = 0; i < 8; i++)
        onewireWriteByte(address[i]);
    onewireWriteByte(0xBE);

    for (int i = 0; i < 2; i++) {
        scratchPad[i] = onewireReadByte();
    }
    onewireInit();
    temperature = ((scratchPad[1] * 256) + scratchPad[0])*0.0625;

    return temperature;
}
