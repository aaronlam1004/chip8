#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Cpu.hpp"
#include "Gpu.hpp"
#include "Display.hpp"
#include "Keys.hpp"

struct Chip8
{
    Cpu cpu;
    Gpu gpu;
    Display display;

    bool newPress = false;
    bool keyPad[NUM_KEYS] = {false};

    void keyInterrupt(uint8_t key, bool pressed)
    {
        if (pressed && keyPad[key] != pressed)
        {
            newPress = true;
        }
        else
        {
            newPress = false;
        }
        keyPad[key] = pressed;
    }

    void loadROM(const char* romFile, uint16_t startAddress)
    {
        printf("[LOADING] %s\n", romFile);
        uint8_t romMemory[4096] = {0x00};

        // Read bytes from ROM
        std::ifstream rom(romFile, std::ios::binary);
        rom.seekg(0, std::ios::end);
        size_t romMemoryLength = rom.tellg();
        rom.seekg(0, std::ios::beg);
        rom.read(reinterpret_cast<char*>(romMemory), romMemoryLength);
        rom.close();
       
        // Load ROM memory into CPU
        memcpy(cpu.memory + startAddress, romMemory, romMemoryLength);
        cpu.PC = startAddress;
    }
};

void tick(Chip8& chip8);
void updateTimers(Chip8& chip8);
