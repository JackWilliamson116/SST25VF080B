#include <SST25VF080B.h>

/////////////////
///Constructor///
/////////////////
SST25VF080B::SST25VF080B(int csPin = 10) {
    CS = csPin;
}


/////////////////
////Functions////
/////////////////
void SST25VF080B::read(uint32_t Address, int numbytes){

    //uint8_t bytes[numbytes];//bytes;
    uint8_t byte;
    SPI.transfer(READ);
    SPI.transfer(0x11);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    //bytes = SPI.transfer(0xFF);
    //Serial.print("READ VALUE: ");
    //Serial.println(bytes);
    //for(int i = 0; i<numbytes; i++){
    byte = SPI.transfer(0x01);
    //if(byte != 0){
    //Serial.print("Address: ");
    //Serial.print(i);
    //Serial.print(" Value: ");
    Serial.print(byte);
    //}
    //Serial.print(SPI.transfer(0x00));//bytes[i] = SPI.transfer(0x00);
    //}
    /*for(int i = 0; i < numbytes; i++){
    Serial.print(bytes[i]);
    }*/
}

void SST25VF080B::writeEnable(){

    digitalWrite(CS,LOW);
    SPI.transfer(WREN);
    digitalWrite(CS,HIGH);
}

void SST25VF080B::enableWriteSR(){

    digitalWrite(CS,LOW);
    SPI.transfer(EWSR);
    digitalWrite(CS,HIGH);
}

uint8_t SST25VF080B::getStatus(){

    uint8_t status;
    digitalWrite(CS,LOW);
    SPI.transfer(RDSR);
    status = SPI.transfer(0x00);
    digitalWrite(CS,HIGH);
    Serial.print("Status = ");
    Serial.println(status);
    return status;
}

void SST25VF080B::writeSR(uint8_t newSR){

    enableWriteSR();
    digitalWrite(CS,LOW);
    SPI.transfer(WRSR);
    SPI.transfer(newSR);
    digitalWrite(CS,HIGH);
}

void SST25VF080B::writeDisable(){

    digitalWrite(CS,LOW);
    SPI.transfer(WRDI);
    digitalWrite(CS,HIGH);
}

void SST25VF080B::writeByte(uint32_t Address, uint8_t value){

    writeEnable();
    Serial.print("beforeWriteByte: ");
    getStatus();
    delay(500);
    digitalWrite(CS,LOW);
    SPI.transfer(BYTE_PROGRAM);
    SPI.transfer(0x11);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x44);
    delayMicroseconds(20);
    digitalWrite(CS,HIGH);
    Serial.print("afterWriteByte: ");
    getStatus();
    uint8_t status;
    //status = getStatus();
    bool isDone = false;
    while(!isDone){
        Serial.print("afterWriteByte: ");
        status = getStatus();
        if((status & 0x01) == 0){
            isDone = true;
        }
    }
}

void SST25VF080B::writeArray(uint32_t Address, int numbytes, uint8_t *bytes){

    writeEnable();
    digitalWrite(CS,LOW);
    SPI.transfer(AAI_WORD_PROGRAM);
    SPI.transfer(Address);
    SPI.transfer(Address >> 8);
    SPI.transfer(Address >>16);
    for(int i = 0; i<numbytes; i++){
        SPI.transfer(bytes[i]);
    }
    digitalWrite(CS,HIGH);
    bool isDone = false;
    uint8_t status;
    while(!isDone){
        status = getStatus();
        if((status & 0x01) == 0){
            isDone = true;
        }
    }
}

void SST25VF080B::erase4k(uint32_t Address){

    writeEnable();
    Serial.print("statusb4erase: ");
    getStatus();
    digitalWrite(CS,LOW);
    SPI.transfer(FOUR_KB_SECTOR_ERASE);
    SPI.transfer(0x10);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    digitalWrite(CS,HIGH);
    Serial.print("afterErase: ");
    getStatus();
    bool isDone = false;
    uint8_t status;
    while(!isDone){
        status = getStatus();
        if((status & 0x01) == 0){
            isDone = true;
        }
    }
}

void SST25VF080B::eraseChip(){

    writeEnable();
    Serial.print("statusb4erase: ");
    getStatus();
    digitalWrite(CS,LOW);
    SPI.transfer(CHIP_ERASE);
    digitalWrite(CS,HIGH);
    Serial.print("afterErase: ");
    getStatus();
    bool isDone = false;
    uint8_t status;
    while(!isDone){
        status = getStatus();
        if((status & 0x01) == 0){
            isDone = true;
        }
    }
}