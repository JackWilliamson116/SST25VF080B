#include <Arduino.h>

#include <SST25VF080B.h>

SST25VF080B eeprom;
uint8_t stat;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(eeprom.CS,OUTPUT);
  Serial.begin(500000);
  delay(1000);
  Serial.println("Serial started");
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
  delay(35);
  eeprom.writeSR(0x1c);
  //delay(5);
  Serial.print("FirstStatus: ");
  Serial.println(eeprom.getStatus());
  //delay(5);
  eeprom.writeSR(0x00);
  //delay(5);
  Serial.print("Second Status: ");
  Serial.println(eeprom.getStatus());
  //delay(5);
  //Serial.print("AfterWRSRStatus: ");
  //Serial.println(eeprom.getStatus());
  //delay(500);
  eeprom.erase4k(0xa5);
  //delay(500);
  //eeprom.writeSR(0x00);
  delay(500);
  Serial.print("Third Status: ");
  Serial.println(eeprom.getStatus());
  eeprom.writeSR(0x00);
  delay(500);
  //delay(500);
  eeprom.writeByte(0xa5,0x5a);
  delay(500);
  //delay(5);
  //delay(500);
  //SPI.endTransaction();
  /*stat = eeprom.getStatus();
  Serial.print("Status: ");
  Serial.println(stat);*/
  //delay(5);
  uint8_t bytes[1];
  eeprom.read(0xa5, bytes,sizeof(bytes));
  for(int i=0;i<sizeof(bytes);i++){
    Serial.print("Byte ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bytes[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(500);
  //stat = eeprom.getStatus();
  //Serial.print("Status: ");
  //Serial.println(stat);
  delay(500);
  //read(0x00,1);
}