#include <stdio.h>
#include <thread>
#include <ctime>
#include <chrono>
#include "Chip8.hpp"
#include "Display.hpp"
#include "Keys.hpp"

Chip8 chip8;

int main(int argc, char* argv)
{
    printf("Hello World!\n");

    // chip8.loadROM("roms/test_opcode.ch8", 0x200);
    // chip8.loadROM("roms/2-ibm-logo.ch8", 0x200);
    chip8.loadROM("roms/space_invaders.ch8", 0x200);

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

    int timeCount = 0;
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
                    switch (event.key.scancode)
                    {
                        case SDL_SCANCODE_1: { chip8.keyInterrupt(KEY_1, true); } break;
                        case SDL_SCANCODE_2: { chip8.keyInterrupt(KEY_2, true); } break;
                        case SDL_SCANCODE_3: { chip8.keyInterrupt(KEY_3, true); } break;
                        case SDL_SCANCODE_4: { chip8.keyInterrupt(KEY_C, true); } break;
                        case SDL_SCANCODE_Q: { chip8.keyInterrupt(KEY_4, true); } break;
                        case SDL_SCANCODE_W: { chip8.keyInterrupt(KEY_5, true); } break;
                        case SDL_SCANCODE_E: { chip8.keyInterrupt(KEY_6, true); } break;
                        // SDL_SCANCODE_R: { chip8.keyInterrupt(KEY_C) } break;
                        case SDL_SCANCODE_ESCAPE: { running = false; } break;
                        default: break;
                    }
                } break;
                case SDL_EVENT_KEY_UP:
                {
                    switch (event.key.scancode)
                    {
                        case SDL_SCANCODE_1: { chip8.keyInterrupt(KEY_1, false); } break;
                        case SDL_SCANCODE_2: { chip8.keyInterrupt(KEY_2, false); } break;
                        case SDL_SCANCODE_3: { chip8.keyInterrupt(KEY_3, false); } break;
                        case SDL_SCANCODE_4: { chip8.keyInterrupt(KEY_C, false); } break;
                        case SDL_SCANCODE_Q: { chip8.keyInterrupt(KEY_4, false); } break;
                        case SDL_SCANCODE_W: { chip8.keyInterrupt(KEY_5, false); } break;
                        case SDL_SCANCODE_E: { chip8.keyInterrupt(KEY_6, false); } break;
                        // SDL_SCANCODE_R: { chip8.keyInterrupt(KEY_C) } break;
                        default: break;
                    }
                } break;
                default: break;
            }
        }
        tick(chip8);
        chip8.display.draw(chip8.gpu.buffer);
        
        float clockSpeed = 1e6;
        int delay = (int) (1e6 / clockSpeed);

        timeCount++;
        if (timeCount >= (int) (2000 / 60))
        {
            timeCount = 0;
            if (chip8.cpu.delayTimer > 0)
            {
                chip8.cpu.delayTimer--;
            }

            if (chip8.cpu.soundTimer > 0)
            {
                chip8.cpu.soundTimer--;
            }
        }
        // std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }

    return 0;
}
