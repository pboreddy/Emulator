#include <iostream>
#include "Memory.h"
#include "Chip8.h"
#include <SDL2/SDL.h>
//#include <gtest/gtest.h>

int main() {
//    std::cout << "Enter the filepath of the ROM that you would like to load into the emulator.\n";
    std::string filepath = "a";
//    std::cin >> filepath;
    Chip8 chip = Chip8(filepath);
    // Initialize SDL with video
    SDL_Init(SDL_INIT_VIDEO);

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_OPENGL);

    // if failed to create a window
    if(!window)
    {
        // we'll print an error message and exit
        std::cerr << "Issue with creating window!\n";
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        std::cerr << "There was a problem with making the renderer.\n";
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    SDL_Event event;	 // used to store any events from the OS
    bool running = true; // used to determine if we're running the game
    int pitch = 64 * sizeof(uint32_t);

    while(running)
    {
        chip.run();
        SDL_UpdateTexture(texture, nullptr, chip.display, pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        // poll for events from SDL
        while(SDL_PollEvent(&event))
        {
            switch(event.type) {
                case SDL_KEYUP:
                    std::cout << "key up " << SDL_GetKeyName(event.key.keysym.sym) << "\n";
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            chip.input_keys[0] = 0;
//                            std::cout << "1 pressed" << "\n";
                            break;
                        case SDLK_2:
                            chip.input_keys[1] = 0;
//                            std::cout << "2 pressed" << "\n";
                            break;
                        case SDLK_3:
                            chip.input_keys[2] = 0;
//                            std::cout << "3 pressed" << "\n";
                            break;
                        case SDLK_4:
                            chip.input_keys[3] = 0;
//                            std::cout << "4 pressed" << "\n";
                            break;
                        case SDLK_q:
                            chip.input_keys[4] = 0;
//                            std::cout << "q pressed" << "\n";
                            break;
                        case SDLK_w:
                            chip.input_keys[5] = 0;
//                            std::cout << "w pressed" << "\n";
                            break;
                        case SDLK_e:
                            chip.input_keys[6] = 0;
//                            std::cout << "e pressed" << "\n";
                            break;
                        case SDLK_r:
                            chip.input_keys[7] = 0;
//                            std::cout << "r pressed" << "\n";
                            break;
                        case SDLK_a:
                            chip.input_keys[8] = 0;
//                            std::cout << "a pressed" << "\n";
                            break;
                        case SDLK_s:
                            chip.input_keys[9] = 0;
//                            std::cout << "s pressed" << "\n";
                            break;
                        case SDLK_d:
                            chip.input_keys[10] = 0;
//                            std::cout << "d pressed" << "\n";
                            break;
                        case SDLK_f:
                            chip.input_keys[11] = 0;
//                            std::cout << "f pressed" << "\n";
                            break;
                        case SDLK_z:
                            chip.input_keys[12] = 0;
//                            std::cout << "z pressed" << "\n";
                            break;
                        case SDLK_x:
                            chip.input_keys[13] = 0;
//                            std::cout << "x pressed" << "\n";
                            break;
                        case SDLK_c:
                            chip.input_keys[14] = 0;
//                            std::cout << "c pressed" << "\n";
                            break;
                        case SDLK_v:
                            chip.input_keys[15] = 0;
//                            std::cout << "v pressed" << "\n";
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    std::cout << "key down " << SDL_GetKeyName(event.key.keysym.sym) << "\n";
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            chip.input_keys[0] = 1;
//                            std::cout << "1 pressed" << "\n";
                            break;
                        case SDLK_2:
                            chip.input_keys[1] = 1;
//                            std::cout << "2 pressed" << "\n";
                            break;
                        case SDLK_3:
                            chip.input_keys[2] = 1;
//                            std::cout << "3 pressed" << "\n";
                            break;
                        case SDLK_4:
                            chip.input_keys[3] = 1;
//                            std::cout << "4 pressed" << "\n";
                            break;
                        case SDLK_q:
                            chip.input_keys[4] = 1;
//                            std::cout << "q pressed" << "\n";
                            break;
                        case SDLK_w:
                            chip.input_keys[5] = 1;
//                            std::cout << "w pressed" << "\n";
                            break;
                        case SDLK_e:
                            chip.input_keys[6] = 1;
//                            std::cout << "e pressed" << "\n";
                            break;
                        case SDLK_r:
                            chip.input_keys[7] = 1;
//                            std::cout << "r pressed" << "\n";
                            break;
                        case SDLK_a:
                            chip.input_keys[8] = 1;
//                            std::cout << "a pressed" << "\n";
                            break;
                        case SDLK_s:
                            chip.input_keys[9] = 1;
//                            std::cout << "s pressed" << "\n";
                            break;
                        case SDLK_d:
                            chip.input_keys[10] = 1;
//                            std::cout << "d pressed" << "\n";
                            break;
                        case SDLK_f:
                            chip.input_keys[11] = 1;
//                            std::cout << "f pressed" << "\n";
                            break;
                        case SDLK_z:
                            chip.input_keys[12] = 1;
//                            std::cout << "z pressed" << "\n";
                            break;
                        case SDLK_x:
                            chip.input_keys[13] = 1;
//                            std::cout << "x pressed" << "\n";
                            break;
                        case SDLK_c:
                            chip.input_keys[14] = 1;
//                            std::cout << "c pressed" << "\n";
                            break;
                        case SDLK_v:
                            chip.input_keys[15] = 1;
//                            std::cout << "v pressed" << "\n";
                            break;
                    }
                    break;
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

    }

    // Destroy things
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // quit SDL
    SDL_Quit();

    return 0;
}
