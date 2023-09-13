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
            // determine if the user still wants to have the window open
            // (this basically checks if the user has pressed 'X')
            running = event.type != SDL_QUIT;
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
