#pragma once

#include <stdio.h>
#include <SDL3/SDL.h>
#include "Gpu.hpp"

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Display
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    const char* name = "Chip-8";

    int scale = 12;
    int width = RES_WIDTH * scale;
    int height = RES_HEIGHT * scale;

    Color clearColor { 0x00, 0x00, 0x00 };
    Color fillColor  { 0xFF, 0xFF, 0xFF };

    bool init(void)
    { 
        window = SDL_CreateWindow(name, width, height, 0);
        if (window == nullptr)
        {
            printf("Error creating SDL window: %s\n", SDL_GetError());
            return false;
        }

        renderer = SDL_CreateRenderer(window, NULL);
        if (renderer == nullptr)
        {
            printf("Error creating SDL renderer: %s\n", SDL_GetError());
            return false;
        }
        return true;
    }

    void clearRenderBuffer(void)
    {
        SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, 1);
        SDL_RenderClear(renderer);
    }

    void clear(void)
    {
        clearRenderBuffer();
        SDL_RenderPresent(renderer);
    }

    void draw(uint8_t* buffer)
    {
        clearRenderBuffer();
        for (uint8_t row = 0; row < RES_HEIGHT; ++row)
        {
            for (uint8_t col = 0; col < RES_WIDTH; ++col)
            {
                if (buffer[(row * RES_WIDTH) + col] == 0x1)
                {
                    const SDL_FRect rect = {
                        (float) (col * scale),
                        (float) (row * scale),
                        (float) scale,
                        (float) scale 
                    };
                    SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, 1);
                    SDL_RenderFillRect(renderer, &rect);
                }
                else
                {
                    /*
                    const SDL_FRect rect = {
                        (float) (col * scale) - (scale / 2),
                        (float) (row * scale) - (scale / 2),
                        (float) scale,
                        (float) scale 
                    };
                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 1);
                    SDL_RenderFillRect(renderer, &rect);
                    */
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
};
