#include <stdio.h>

#include <thread>
#include <chrono>

#include "Chip8.hpp"
#include "Display.hpp"

Chip8 chip8;

int main(int argc, char* argv)
{
    printf("Hello World!\n");

    // chip8.loadROM("roms/test_opcode.ch8", 0x200);
    chip8.loadROM("roms/2-ibm-logo.ch8", 0x200);
    // chip8.loadROM("roms/space_invaders.ch8", 0x200);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!chip8.display.init())
    {
        return 1;
    }

    bool running = true;
    SDL_Event event;
    SDL_zero(event);
    
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                } break;
                case SDL_EVENT_KEY_DOWN:
                {
                } break;
                default: break;
            }
        }
        tick(chip8);
        chip8.display.draw(chip8.gpu.buffer);
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
