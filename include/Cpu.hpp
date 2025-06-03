#pragma once

#include <stdint.h>
#include "Opcodes.hpp"

struct Cpu
{
    // Counters/trackers
    uint16_t PC = 0x0200; // Program Counter
    uint16_t I  = 0x0000; // Index Register
    uint8_t SP = 0x0; // Stack pointer
   
    // Timers
    uint8_t delayTimer = 0x00; // Decremented at 60 Hz (60 times per second) until 0
    uint8_t soundTimer = 0x00; // Decremented at 60 Hz (60 times per second) until 0 - beeps when not 0

    // Memory
    uint8_t memory[4096] = { 0x00 };
    uint16_t stack[16] = { 0x0000 };
    uint8_t registers[16] = { 0x00 };

    bool pushToStack(uint16_t value)
    {
        if (SP < 16)
        {
            stack[SP] = value; 
            SP++;
            return true;
        }
        return false;
    }

    bool popFromStack(uint16_t& value)
    {
        if (SP > 0)
        {
            value = stack[SP - 1];
            SP--;
            return true;
        }
        return false;
    }

    uint16_t readOperation(void)
    {
        uint16_t operation = (memory[PC] << 8) | memory[PC + 1];
        return operation;
    }
};
