#include <SST25VF080B.h>

/////////////////
///Constructor///
/////////////////
SST25VF080B::SST25VF080B(int csPin) {
    CS = csPin;
}


/////////////////
////Functions////
/////////////////
void SST25VF080B::read(uint32_t Address, uint8_t *bytes, int numbytes){

    digitalWrite(CS,LOW);
    uint8_t byte;
    //Serial.println(numbytes);
    SPI.transfer(READ);
    SPI.transfer( Address & 0x000000ff);
    SPI.transfer((Address & 0x0000ff00) >> 8);
    SPI.transfer((Address & 0x00ff0000) >> 16);
    for(int i = 0; i<numbytes; i++){
        byte = SPI.transfer(0x01);
        bytes[i] = byte;
    }
    digitalWrite(CS,HIGH);
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
    uint8_t stat = getStatus();
    Serial.print("PreWBStatusCheck: ");
    Serial.println(stat);
    if(stat != 0x02){
        Serial.println("Unable To Write Because Invalid Status");
        return;
    }
    digitalWrite(CS,LOW);
    SPI.transfer(BYTE_PROGRAM);
    SPI.transfer( Address & 0x000000ff);
    SPI.transfer((Address & 0x0000ff00) >> 8);
    SPI.transfer((Address & 0x00ff0000) >> 16);
    SPI.transfer(value);
    digitalWrite(CS,HIGH);
    bool isDone = false;
    while(!isDone){
        stat = getStatus();
        if((stat & 0x01) == 0){
            isDone = true;
        }
    }
    Serial.print("PostWBStatusCheck: ");
    Serial.println(stat);
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
    Serial.println(getStatus());
    digitalWrite(CS,LOW);
    SPI.transfer(FOUR_KB_SECTOR_ERASE);
    SPI.transfer( Address & 0x000000ff);
    SPI.transfer((Address & 0x0000ff00) >> 8);
    SPI.transfer((Address & 0x00ff0000) >> 16);
    digitalWrite(CS,HIGH);
    Serial.print("afterErase: ");
    Serial.println(getStatus());
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