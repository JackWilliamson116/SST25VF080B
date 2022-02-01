#include <Arduino.h>

#include <SST25VF080B.h>

void setup() {
  // put your setup code here, to run once:
  SST25VF080B eeprom;
  pinMode(eeprom.CS,OUTPUT);
  Serial.begin(500000);
  delay(2500);
  Serial.println("Serial started");
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000,MSBFIRST,SPI_MODE0));
  Serial.print("FirstStatus: ");
  eeprom.getStatus();
  delay(500);
  eeprom.writeSR(0x00);
  delay(500);
  Serial.print("AfterWRSRStatus: ");
  eeprom.getStatus();
  eeprom.writeByte(0x10,0x11);
  eeprom.read(0x00,1);
  SPI.endTransaction();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  //read(0x00,1);
}