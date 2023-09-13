#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H
#include <cstdint>
#include <stdexcept>

class Memory {
    private:
        uint8_t memory[4096] = {};
    public:
        static const int FONT_MEMORY_START = 0x0;
        static const int ROM_MEMORY_START = 0x200;
        uint8_t rbyte(uint16_t loc);
        void wbyte(uint16_t loc, uint8_t val);
        uint16_t rword(uint16_t loc);
        void wword(uint16_t loc, uint16_t val);
};


#endif //CHIP8_MEMORY_H
