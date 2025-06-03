#pragma once

#include <stdio.h>

const uint8_t RES_WIDTH = 64;
const uint8_t RES_HEIGHT = 32;

struct Gpu
{
    uint8_t buffer[RES_WIDTH * RES_HEIGHT] = {0x00};
    
    void clear(void)
    {
        memset(buffer, 0, RES_WIDTH * RES_HEIGHT);
    }

    bool draw(uint8_t x, uint8_t y, uint8_t rowData)
    {
        bool carry = false;
        for (uint8_t px = 0; px < 8; ++px)
        {
            uint8_t col = x + px;
            uint8_t bitMask = (rowData >> (7 - px)) & 0x1;
            if (col < RES_WIDTH && y < RES_HEIGHT)
            {
                uint8_t& pixel = buffer[(y * RES_WIDTH)+ col];
                if ((pixel ^ bitMask) == 0x0 && bitMask != 0x0)
                {
                    carry = true;
                }
                pixel ^= bitMask; 
            }
        }
        return carry;
    }
};
