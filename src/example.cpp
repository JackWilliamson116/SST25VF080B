#include <Arduino.h>

#include <SST25VF080B.h>

SST25VF080B eeprom;
uint8_t stat;
unsigned long time;

void setup() {
  
  // Set up chip select pin
  pinMode(eeprom.CS,OUTPUT);
  // Start serial
  Serial.begin(500000);
  delay(1000);
  Serial.println("Serial started");
  // Begin and initialize SPI communication with the eeprom
  eeprom.init(SPISettings(1000000,MSBFIRST,SPI_MODE0));
  delay(35);

  // Get the initial status
  Serial.print("FirstStatus: ");
  Serial.println(eeprom.getStatus());

  // Clear the write protect bits in status register
  eeprom.writeSR(0x00);

  // Show the new status
  Serial.print("Second Status: ");
  Serial.println(eeprom.getStatus());

  // Usage example of erasing operations
  //eeprom.eraseChip();
  //eeprom.erase4k(0x00);
  //eeprom.erase64k(0xa5);
  //eeprom.writeSR(0x00);

  //uint8_t bytes[10] = {'H','i',' ','m','o','m','!','!','!','!'};
  // Write the array "bytes" to Address 0
  // Remember, it array writes in sets of two so only even arrays
  //String himom = "Hi mom!!!!";
  // Write the array "bytes" to Address 0
  // Remember, it array writes in sets of two so only even arrays
  /*char* char_ary;
  char_ary = &himom[0];
  uint8_t bytes[sizeof(himom)] = {};
  for(int i = 0; i<sizeof(himom); i++){
    bytes[i] = himom[i];
  }*/
  //unsigned long time = micros();
  //eeprom.writeByte(0x00000001,'W');
  //eeprom.writeByte(0x00000004,'A');
  //eeprom.writeArray(0x00000020,bytes,sizeof(bytes));
  /*Serial.print("Third Status: ");
  Serial.println(eeprom.getStatus());*/
  //time = micros()-time;
  //Serial.println(time);

  //eeprom.writeByte(0x00,'W');

  // To read you must pass in an array of bytes of the length that you want to read
  uint8_t rdbytes[62000];
  eeprom.read(0x00,rdbytes,sizeof(rdbytes));
  for(int i=0;i<sizeof(rdbytes);i++){
      Serial.print((char)rdbytes[i]);
  }
  /*uint32_t totalbytes = 0;
  while(totalbytes < 0x100000){
    uint8_t rdbytes[0xFF];
    eeprom.read(totalbytes, rdbytes,sizeof(rdbytes));
    totalbytes = totalbytes + 0x100;
    for(int i=0;i<sizeof(rdbytes);i++){
      //Serial.print((char)rdbytes[i]);
    }
  }
  Serial.println(totalbytes);*/
}

void loop() {

  //Test your eeprom mainly in the setup section so you don't use up your write capacity
  delay(500);
}