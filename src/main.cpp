#include <Arduino.h>
#include <SPI.h>

/////////////////
//// OPCODES ////
/////////////////

uint8_t READ = 0x03;
uint8_t HIGH_SPEED_READ = 0x0B;
uint8_t FOUR_KB_SECTOR_ERASE = 0x20;
uint8_t THIRTY_TWO_KB_BLOCK_ERASE = 0x52;
uint8_t SIXTY_FOUR_KB_BLOCK_ERASE = 0xD8;
uint8_t CHIP_ERASE = 0x60;
uint8_t BYTE_PROGRAM = 0x02;
uint8_t AAI_WORD_PROGRAM = 0xAD;
uint8_t RDSR = 0x05;
uint8_t EWSR = 0x50;
uint8_t WRSR = 0x01;
uint8_t WREN = 0x06;
uint8_t WRDI = 0x04;
uint8_t RDID = 0x90;
uint8_t JEDECID = 0x9F;
uint8_t EBSY = 0x70;
uint8_t DBSY = 0x80;

int CS = 10;

void read(uint32_t Address, int numbytes){

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

void writeEnable(){

  digitalWrite(CS,LOW);
  SPI.transfer(WREN);
  digitalWrite(CS,HIGH);
}

void enableWriteSR(){

  digitalWrite(CS,LOW);
  SPI.transfer(EWSR);
  digitalWrite(CS,HIGH);
}

uint8_t getStatus(){

  uint8_t status;
  digitalWrite(CS,LOW);
  SPI.transfer(RDSR);
  status = SPI.transfer(0x00);
  digitalWrite(CS,HIGH);
  Serial.print("Status = ");
  Serial.println(status);
  return status;
}

void writeSR(uint8_t newSR){

  enableWriteSR();
  digitalWrite(CS,LOW);
  SPI.transfer(WRSR);
  SPI.transfer(newSR);
  digitalWrite(CS,HIGH);
}

void writeDisable(){

  digitalWrite(CS,LOW);
  SPI.transfer(WRDI);
  digitalWrite(CS,HIGH);
}

void writeByte(uint32_t Address, uint8_t value){

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

void writeArray(uint32_t Address, int numbytes, uint8_t *bytes){

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

void erase4k(uint32_t Address){

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

void eraseChip() {
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


void setup() {
  // put your setup code here, to run once:
  pinMode(CS,OUTPUT);
  Serial.begin(500000);
  delay(2500);
  Serial.println("Serial started");
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000,MSBFIRST,SPI_MODE0));
  Serial.print("FirstStatus: ");
  getStatus();
  delay(500);
  writeSR(0x00);
  delay(500);
  Serial.print("AfterWRSRStatus: ");
  getStatus();
  writeByte(0x10,0x11);
  read(0x00,1);
  SPI.endTransaction();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  //read(0x00,1);
}