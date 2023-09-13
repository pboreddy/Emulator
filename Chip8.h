#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H
#include <cstdint>
#include <string>
#include <fstream>
#include <stdexcept>
#include <random>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Memory.h"

class Chip8 {
public:
    Chip8(std::string filename);
    static const int DISPLAY_WIDTH = 64;
    static const int DISPLAY_HEIGHT = 32;
    uint8_t registers[16] = {};
    Memory memory;
    uint16_t index_reg;
    uint16_t prog_counter = Memory::ROM_MEMORY_START;
    uint16_t stack[16] = {};
    uint8_t stack_p = 0;
    uint8_t delay_t;
    uint8_t sound_t;
    uint8_t input_keys[16];
    uint32_t display[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {};
    uint16_t op;
    uint8_t genRand();
    void loadROM(std::string filename);
    void loadFonts();
    void op_00E0();
    void op_00EE();
    void op_1nnn();
    void op_2nnn();
    void op_3xkk();
    void op_4xkk();
    void op_5xy0();
    void op_6xkk();
    void op_7xkk();
    void op_8xy0();
    void op_8xy1();
    void op_8xy2();
    void op_8xy3();
    void op_8xy4();
    void op_8xy5();
    void op_8xy6();
    void op_8xy7();
    void op_8xyE();
    void op_9xy0();
    void op_Annn();
    void op_Bnnn();
    void op_Cxkk();
    void op_Dxyn();
    void op_Ex9E();
    void op_ExA1();
    void op_Fx07();
    void op_Fx0A();
    void op_Fx15();
    void op_Fx18();
    void op_Fx1E();
    void op_Fx29();
    void op_Fx33();
    void op_Fx55();
    void op_Fx65();
    void run();
};


#endif //CHIP8_CHIP8_H
