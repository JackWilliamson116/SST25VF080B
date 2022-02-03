#include <SST25VF080B.h>

/////////////////
///Constructor///
/////////////////
SST25VF080B::SST25VF080B(uint8_t csPin) {
    CS = csPin;
}

/////////////////
////Functions////
/////////////////

void SST25VF080B::init(SPISettings set){

    SPI.begin();
    SPI.beginTransaction(set);
}

void SST25VF080B::read(uint32_t Address, uint8_t *bytes, int numbytes){

    digitalWrite(CS,LOW);
    uint8_t byte;
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
    status = getStatus();
    if(status != 0x02){
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
    while(true){
        status = getStatus();
        if((status & 0x01) == 0){
            break;
        }
    }
    // Time = 17
}

void SST25VF080B::writeArray(uint32_t Address, uint8_t *bytes, int numbytes){

    if(numbytes%2 == 1){
        Serial.println("Input must be even or else you will lose the last byte");
        return;
    }
    writeEnable();
    digitalWrite(CS,LOW);
    SPI.transfer(AAI_WORD_PROGRAM);
    SPI.transfer( Address & 0x000000ff);
    SPI.transfer((Address & 0x0000ff00) >> 8);
    SPI.transfer((Address & 0x00ff0000) >> 16);
    SPI.transfer(bytes[0]);
    for(int i = 1; i<numbytes; i++){
        SPI.transfer(bytes[i]);
        if(i%2 == 1 && i+1< numbytes){
            digitalWrite(CS,HIGH);
            while(true){
                status = getStatus();
                if((status & 0x01) == 0){
                    break;
                }
            }
            digitalWrite(CS,LOW);
            SPI.transfer(AAI_WORD_PROGRAM);
        }
    }
    digitalWrite(CS,HIGH);
    while(true){
        status = getStatus();
        if((status & 0x01) == 0){
            break;
        }
    }
    writeDisable();
}

void SST25VF080B::erase4k(uint32_t Address){

    writeEnable();
    digitalWrite(CS,LOW);
    SPI.transfer(FOUR_KB_SECTOR_ERASE);
    SPI.transfer( Address & 0x000000ff);
    SPI.transfer((Address & 0x0000ff00) >> 8);
    SPI.transfer((Address & 0x00ff0000) >> 16);
    digitalWrite(CS,HIGH);
    while(true){
        status = getStatus();
        if((status & 0x01) == 0){
            break;
        }
        delayMicroseconds(220);
    }
}

void SST25VF080B::erase32k(uint32_t Address){

    writeEnable();
    digitalWrite(CS,LOW);
    SPI.transfer(THIRTY_TWO_KB_BLOCK_ERASE);
    SPI.transfer( Address & 0x000000ff);
    SPI.transfer((Address & 0x0000ff00) >> 8);
    SPI.transfer((Address & 0x00ff0000) >> 16);
    digitalWrite(CS,HIGH);
    while(true){
        status = getStatus();
        if((status & 0x01) == 0){
            break;
        }
        delayMicroseconds(220);
    }
}

void SST25VF080B::erase64k(uint32_t Address){
    
    writeEnable();
    digitalWrite(CS,LOW);
    SPI.transfer(SIXTY_FOUR_KB_BLOCK_ERASE);
    SPI.transfer( Address & 0x000000ff);
    SPI.transfer((Address & 0x0000ff00) >> 8);
    SPI.transfer((Address & 0x00ff0000) >> 16);
    digitalWrite(CS,HIGH);
    while(true){
        status = getStatus();
        if((status & 0x01) == 0){
            break;
        }
        delayMicroseconds(220);
    }
}

void SST25VF080B::eraseChip(){

    writeEnable();
    digitalWrite(CS,LOW);
    SPI.transfer(CHIP_ERASE);
    digitalWrite(CS,HIGH);
    while(true){
        status = getStatus();
        if((status & 0x01) == 0){
            break;
        }
        delayMicroseconds(500);
    }
}