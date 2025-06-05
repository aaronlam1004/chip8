#include <stdio.h>
#include <thread>
#include <ctime>
#include <chrono>
#include "Chip8.hpp"
#include "Display.hpp"
#include "Keys.hpp"

static const int INSTRUCTIONS_PER_STEP = 700;
static const int REFRESH_RATE = 60;

Chip8 chip8;

int main(int argc, char* argv[])
{
    printf("Hello Chip-8!\n");
    if (argc == 2)
    {
        printf("%s\n", argv[1]);
        chip8.loadROM(argv[1], 0x200);
    }
    else
    {
        chip8.loadROM("roms/test_opcode.ch8", 0x200);
    }


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
                    switch (event.key.scancode)
                    {
                        case SDL_SCANCODE_1: { chip8.keyInterrupt(KEY_1, true); } break;
                        case SDL_SCANCODE_2: { chip8.keyInterrupt(KEY_2, true); } break;
                        case SDL_SCANCODE_3: { chip8.keyInterrupt(KEY_3, true); } break;
                        case SDL_SCANCODE_4: { chip8.keyInterrupt(KEY_C, true); } break;
                        case SDL_SCANCODE_Q: { chip8.keyInterrupt(KEY_4, true); } break;
                        case SDL_SCANCODE_W: { chip8.keyInterrupt(KEY_5, true); } break;
                        case SDL_SCANCODE_E: { chip8.keyInterrupt(KEY_6, true); } break;
                        // case SDL_SCANCODE_R: { chip8.keyInterrupt(KEY_C) } break;
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
                        // case SDL_SCANCODE_R: { chip8.keyInterrupt(KEY_C) } break;
                        default: break;
                    }
                } break;
                default: break;
            }
        }

        for (int i = 0; i < (int) (INSTRUCTIONS_PER_STEP / REFRESH_RATE); ++i)
        {
            tick(chip8);
        }
        updateTimers(chip8);
        chip8.display.draw(chip8.gpu.buffer);
        std::this_thread::sleep_for(std::chrono::milliseconds(INSTRUCTIONS_PER_STEP / REFRESH_RATE));
    }

    return 0;
}
