#include "Memory.h"
#include <iostream>

uint8_t Memory::rbyte(uint16_t loc){
    if (loc >= 0 && loc < sizeof(memory)) {
        return memory[loc];
    } else {
        throw std::invalid_argument("Requested location out of memory bounds");
    }
}

void Memory::wbyte(uint16_t loc, uint8_t val){
    if (loc >= 0 && loc < sizeof(memory)) {
        memory[loc] = val;
    } else {
        throw std::invalid_argument("Requested location out of memory bounds");
    }
}

uint16_t Memory::rword(uint16_t loc) {
    if (loc >= 0 && loc < sizeof(memory) - 1) {
        uint8_t byte1 = memory[loc];
        uint8_t byte2 = memory[loc+1];
        uint16_t res = (static_cast<uint16_t>(byte1) << 8) | byte2;
        return res;
    } else {
        throw std::invalid_argument("Requested location out of memory bounds");
    }
}

void Memory::wword(uint16_t loc, uint16_t val){
    if (loc >= 0 && loc < sizeof(memory) - 1) {
        uint8_t byte1 = (val & 0xFF00);
        uint8_t byte2 = (val & 0x00FF);
        memory[loc] = byte1;
        memory[loc + 1] = byte2;
    } else {
        throw std::invalid_argument("Requested location out of memory bounds");
    }
}


