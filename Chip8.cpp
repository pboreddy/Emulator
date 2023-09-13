#include "Chip8.h"
#include <chrono>
#include <thread>

Chip8::Chip8(std::string filename) {
//    loadROM(filename);
    loadROM("../test_opcode.ch8");
//    loadROM("../1-chip8-logo.ch8");
//    loadROM("../IBMLogo.ch8");
    loadFonts();
};

uint8_t Chip8::genRand(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    return dist(gen);
}

void Chip8::loadROM(std::string filename) {
    std::ifstream file (filename, std::ios::binary);
    if (file.is_open()) {
        file.seekg(0,std::ios::end);
        uint16_t file_size = file.tellg();
        file.seekg(0,std::ios::beg);
        char* buffer = new char[file_size];
        file.read(buffer, file_size);
        for (int i=0;i<file_size;i++){
            memory.wbyte(Memory::ROM_MEMORY_START+i,buffer[i]);
        }
        file.close();
        delete[] buffer;
    } else {
        std::cerr << "There was a problem with opening the file.";
    }
}
void Chip8::loadFonts() {
    uint8_t sprites[16][5] = {
            {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
            {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
            {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
            {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
            {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
            {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
            {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
            {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
            {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
            {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
            {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
            {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
            {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
            {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
            {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
            {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
    };
    for(int i = 0; i < 16; i++) {
        for (int j = 0; j < 5; j++) {
            memory.wbyte(Memory::FONT_MEMORY_START+i*5+j,sprites[i][j]);
        }
    }
}
void Chip8::op_00E0() {
    // Clear the display.
    memset(display,0,sizeof(display));
}

void Chip8::op_00EE() {
    // Return from a subroutine.
    stack_p--;
    prog_counter = stack[stack_p];
}

void Chip8::op_1nnn() {
    // Jump to location nnn.
    uint16_t nnn = op & 0x0FFF;
    prog_counter = nnn - 2;
}

void Chip8::op_2nnn() {
    // Call subroutine at nnn.
    uint16_t nnn = op & 0x0FFF;
    stack[stack_p] = prog_counter;
    stack_p++;
    prog_counter = nnn - 2;
}

void Chip8::op_3xkk() {
    // Skip next instruction if Vx = kk.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t kk = op & 0x00FF;
    if (registers[Vx] == kk){
        prog_counter += 2;
    }
}

void Chip8::op_4xkk() {
    //Skip next instruction if Vx != kk.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t kk = op & 0x00FF;
    if (registers[Vx] != kk){
        prog_counter += 2;
    }
}

void Chip8::op_5xy0() {
    // Skip next instruction if Vx = Vy.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    if(registers[Vx] == registers[Vy]){
        prog_counter += 2;
    }
}

void Chip8::op_6xkk(){
    // Set Vx = kk.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t kk = op & 0x00FF;
    registers[Vx] = kk;
}

void Chip8::op_7xkk() {
    // Set Vx = Vx + kk.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t kk = op & 0x00FF;
    registers[Vx] += kk;
}

void Chip8::op_8xy0() {
    // Set Vx = Vy.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    registers[Vx] = registers[Vy];
}

void Chip8::op_8xy1() {
    // Set Vx = Vx OR Vy.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    registers[Vx] |= registers[Vy];
}

void Chip8::op_8xy2() {
    // Set Vx = Vx AND Vy.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    registers[Vx] &= registers[Vy];
}

void Chip8::op_8xy3() {
    // Set Vx = Vx XOR Vy.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    registers[Vx] ^= registers[Vy];
}

void Chip8::op_8xy4() {
    // Set Vx = Vx + Vy, set VF = carry.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    uint16_t sum = registers[Vx] + registers[Vy];
    if (sum > 0x00FF) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }
    registers[Vx] = sum & 0x00FF;
}

void Chip8::op_8xy5() {
    // Set Vx = Vx - Vy, set VF = NOT borrow.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    if (registers[Vx] > registers[Vy]) {
        registers[0xF] = 1;
    } else{
        registers[0xF] = 0;
    }
    registers[Vx] -= registers[Vy];
}

void Chip8::op_8xy6() {
    // Set Vx = Vx SHR 1.
    uint8_t Vx = (op & 0x0F00) >> 8;
    registers[0xF] = registers[Vx] & 0x01;
    registers[Vx] >>= 1;
}

void Chip8::op_8xy7() {
    //Set Vx = Vy - Vx, set VF = NOT borrow.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    if (registers[Vy] > registers[Vx]) {
        registers[0xF] = 1;
    } else{
        registers[0xF] = 0;
    }
    registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::op_8xyE() {
    // Set Vx = Vx SHL 1.
    uint8_t Vx = (op & 0x0F00) >> 8;
    registers[0xF] = (registers[Vx] & 0x80) >> 7;
    registers[Vx] <<= 1;
}

void Chip8::op_9xy0() {
    // Skip next instruction if Vx != Vy.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    if (registers[Vx] != registers[Vy]) {
        prog_counter += 2;
    }
}

void Chip8::op_Annn() {
    //Set I = nnn.
    index_reg = op & 0x0FFF;
}

void Chip8::op_Bnnn() {
    // Jump to location nnn + V0.
    uint16_t nnn = op & 0x0FFF;
    prog_counter = nnn + registers[0];
}

void Chip8::op_Cxkk() {
    // Set Vx = random byte AND kk.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t kk = op & 0x00FF;
    registers[Vx] = genRand() & kk;
}

void Chip8::op_Dxyn() {
    // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t Vy = (op & 0x00F0) >> 4;
    uint8_t n = (op & 0x000F);
    registers[0xF] = 0;
    for (int i=0;i<n;i++) {
        int sprite_line = memory.rbyte(index_reg + i);
        for (int j=0;j<8;j++){
            uint32_t& displayPix = display[(registers[Vy] + i) % DISPLAY_HEIGHT][(registers[Vx] + j) % DISPLAY_WIDTH];
            uint8_t spritePix = (sprite_line >> (7-j)) & 0x1;
            if ((displayPix == 0xFFFFFFFF) && spritePix) {
                registers[0xF] = 1;
            }
            displayPix ^= spritePix ? 0xFFFFFFFF : 0;
        }
    }
}

void Chip8::op_Ex9E() {
    // Skip next instruction if key with the value of Vx is pressed.
    uint8_t Vx = (op & 0x0F00) >> 8;
    if (input_keys[registers[Vx]]) {
        prog_counter += 2;
    }
}

void Chip8::op_ExA1() {
    // Skip next instruction if key with the value of Vx is not pressed.
    uint8_t Vx = (op & 0x0F00) >> 8;
    if (!input_keys[registers[Vx]]) {
        prog_counter += 2;
    }
}

void Chip8::op_Fx07() {
    // Set Vx = delay timer value.
    uint8_t Vx = (op & 0x0F00) >> 8;
    registers[Vx] = delay_t;
}

void Chip8::op_Fx0A() {
    // Wait for a key press, store the value of the key in Vx.
    uint8_t Vx = (op & 0x0F00) >> 8;
    for(int i=0;i<0xF;i++){
        if (input_keys[i]){
            registers[Vx] = i;
            return;
        }
    }
    prog_counter -= 2;
}

void Chip8::op_Fx15() {
    // Set delay timer = Vx.
    uint8_t Vx = (op & 0x0F00) >> 8;
    delay_t = registers[Vx];
}

void Chip8::op_Fx18() {
    // Set sound timer = Vx.
    uint8_t Vx = (op & 0x0F00) >> 8;
    sound_t = registers[Vx];
}

void Chip8::op_Fx1E() {
    // Set I = I + Vx.
    uint8_t Vx = (op & 0x0F00) >> 8;
    index_reg += registers[Vx];
}

void Chip8::op_Fx29() {
    // Set I = location of sprite for digit Vx.
    uint8_t Vx = (op & 0x0F00) >> 8;
    index_reg = Memory::FONT_MEMORY_START + 5 * registers[Vx];
    // each sprite is 5 bytes, so the location is 5 * index
}

void Chip8::op_Fx33() {
    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
    uint8_t Vx = (op & 0x0F00) >> 8;
    uint8_t VxVal = registers[Vx];
    memory.wbyte(index_reg + 2, VxVal % 10);
    VxVal /= 10;
    memory.wbyte(index_reg + 1, VxVal % 10);
    VxVal /= 10;
    memory.wbyte(index_reg, VxVal % 10);


}

void Chip8::op_Fx55() {
    // Store registers V0 through Vx in memory starting at location I.
    uint8_t Vx = (op & 0x0F00) >> 8;
    for (int i = 0; i < Vx; i++) {
        memory.wbyte(index_reg+i,registers[i]);
    }
}

void Chip8::op_Fx65() {
    // Read registers V0 through Vx from memory starting at location I.
    uint8_t Vx = (op & 0x0F00) >> 8;
    for (int i = 0; i < Vx; i++) {
        registers[i] = memory.rbyte(index_reg+i);
    }
}

void Chip8::run() {
    try {
        op = memory.rword(prog_counter);
    } catch (std::invalid_argument const &e) {
        std::cerr << "Could not get opcode at memory value " << std::hex << prog_counter << "\n";
        exit(EXIT_FAILURE);
    }

//    std::cout << "opcode 0x" << std::setfill('0') << std::setw(4) << std::hex << op << " counter 0x" << std::setw(4) << prog_counter << "\n";
    switch((op & 0xF000) >> 12) {
        case 0x0:
            switch(op & 0x00FF) {
                case 0x00E0:
                    op_00E0();
                    break;
                case 0x00EE:
                    op_00EE();
                    break;
                case 0x0:
                    //change later
//                    exit(EXIT_SUCCESS);
//                    op_1nnn();
                    break;
                default:
//                    std::cerr << "Incorrect opcode " << std::hex << static_cast<uint16_t>(op) << " provided.\n";
//                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0x1:
            op_1nnn();
            break;
        case 0x2:
            op_2nnn();
            break;
        case 0x3:
            op_3xkk();
            break;
        case 0x4:
            op_4xkk();
            break;
        case 0x5:
            switch (op & 0x000F) {
                case 0x0:
                    op_5xy0();
                    break;
                default:
//                    std::cerr << "Incorrect opcode " << std::hex << static_cast<uint16_t>(op) << " provided.\n";
//                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0x6:
            op_6xkk();
            break;
        case 0x7:
            op_7xkk();
            break;
        case 0x8:
            switch (op & 0x000F) {
                case 0x0:
                    op_8xy0();
                    break;
                case 0x1:
                    op_8xy1();
                    break;
                case 0x2:
                    op_8xy2();
                    break;
                case 0x3:
                    op_8xy3();
                    break;
                case 0x4:
                    op_8xy4();
                    break;
                case 0x5:
                    op_8xy5();
                    break;
                case 0x6:
                    op_8xy6();
                    break;
                case 0x7:
                    op_8xy7();
                    break;
                case 0xE:
                    op_8xyE();
                    break;
                default:
//                    std::cerr << "Incorrect opcode " << std::hex << static_cast<uint16_t>(op) << " provided.\n";
//                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0x9:
            switch (op & 0x000F) {
                case 0x0:
                    op_9xy0();
                    break;
                default:
//                    std::cerr << "Incorrect opcode " << std::hex << static_cast<uint16_t>(op) << " provided.\n";
//                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0xA:
            op_Annn();
            break;
        case 0xB:
            op_Bnnn();
            break;
        case 0xC:
            op_Cxkk();
            break;
        case 0xD:
            op_Dxyn();
            break;
        case 0xE:
            switch (op & 0x00FF) {
                case 0x9E:
                    op_Ex9E();
                    break;
                case 0xA1:
                    op_ExA1();
                    break;
                default:
//                    std::cerr << "Incorrect opcode " << std::hex << static_cast<uint16_t>(op) << " provided.\n";
//                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0xF:
            switch (op & 0x00FF) {
                case 0x07:
                    op_Fx07();
                    break;
                case 0x0A:
                    op_Fx0A();
                    break;
                case 0x15:
                    op_Fx15();
                    break;
                case 0x18:
                    op_Fx18();
                    break;
                case 0x1E:
                    op_Fx1E();
                    break;
                case 0x29:
                    op_Fx29();
                    break;
                case 0x33:
                    op_Fx33();
                    break;
                case 0x55:
                    op_Fx55();
                    break;
                case 0x65:
                    op_Fx65();
                    break;
                default:
//                    std::cerr << "Incorrect opcode " << std::hex << static_cast<uint16_t>(op) << " provided.\n";
//                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        default:
//            std::cerr << "Opcode " << std::hex << static_cast<uint16_t>(op) << " not properly handled.\n";
//            exit(EXIT_FAILURE);
            break;
    }
    prog_counter += 2;
    if (sound_t > 0) {
        sound_t--;
    }
    if (delay_t > 0) {
        delay_t--;
    }
    std::this_thread::sleep_for(std::chrono::nanoseconds(25000000));
}

