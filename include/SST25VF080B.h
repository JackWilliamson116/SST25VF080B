#include <Arduino.h>
#include <SPI.h>

class SST25VF080B {
    public:

        // Define CS Pin
        uint8_t CS;
        // Utility status variable
        uint8_t status;

        /////////////////
        //// OPCODES ////
        /////////////////
        const uint8_t READ = 0x03;
        const uint8_t HIGH_SPEED_READ = 0x0B;//
        const uint8_t FOUR_KB_SECTOR_ERASE = 0x20;
        const uint8_t THIRTY_TWO_KB_BLOCK_ERASE = 0x52;
        const uint8_t SIXTY_FOUR_KB_BLOCK_ERASE = 0xD8;
        const uint8_t CHIP_ERASE = 0x60;
        const uint8_t BYTE_PROGRAM = 0x02;
        const uint8_t AAI_WORD_PROGRAM = 0xAD;
        const uint8_t RDSR = 0x05;
        const uint8_t EWSR = 0x50;
        const uint8_t WRSR = 0x01;
        const uint8_t WREN = 0x06;
        const uint8_t WRDI = 0x04;
        const uint8_t RDID = 0x90;//
        const uint8_t JEDECID = 0x9F;//
        const uint8_t EBSY = 0x70;//
        const uint8_t DBSY = 0x80;//

        /////////////////
        ///Constructor///
        /////////////////
        SST25VF080B(uint8_t csPin = 10);

        /////////////////
        ////Functions////
        /////////////////
        void init(SPISettings set);
        void read(uint32_t Address, uint8_t *bytes, int numbytes);
        void writeEnable();
        void enableWriteSR();
        uint8_t getStatus();
        void writeSR(uint8_t newSR);
        void writeDisable();
        void writeByte(uint32_t Address, uint8_t value);
        void writeArray(uint32_t Address, uint8_t *bytes, int numbytes);
        void erase4k(uint32_t Address);
        void erase32k(uint32_t Address);
        void erase64k(uint32_t Address);
        void eraseChip();
};







