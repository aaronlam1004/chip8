#pragma once

#include <stdint.h>

enum OpcodeMasks : uint8_t
{
    SYS      = 0x0, // 0x0NNN
    JUMP     = 0x1, // 0x1NNN
    FUNC     = 0x2, // 0x2NNN
    EQ       = 0x3, // 0x3XNN
    NE       = 0x4, // 0x3XNN
    EQ_VX_VY = 0x5, // 0x5XY0
    SET_VX   = 0x6, // 0x6XNN
    ADD_VX   = 0x7, // 0x7XNN
    MATH     = 0x8, // 0x8XY?
    NE_VX_VY = 0x9, // 0x9XY0
    SET_I    = 0xA, // 0xANNN
    DRAW     = 0xD, // 0xDXYN
    KEY      = 0xE, // 0xEX??
    HW       = 0xF, // 0xFX??
};

enum SysOperations : uint8_t
{
    CLEAR = 0xE0,
    RETURN = 0xEE
};

enum MathOperations : uint8_t
{
    ASSIGN = 0,
    OR,
    AND,
    XOR,
    ADD,
    SUB_VX_VY,
    SHIFT_RIGHT,
    SUB_VY_VX,
    SHIFT_LEFT = 0xE
};

enum KeyOperations : uint8_t
{
    EQ_KEY_VX = 0x9E,
    NE_KEY_VX = 0xA1
};

enum TimerOperations : uint8_t
{
    SET_DELAY_TIMER = 0x07,
};

enum MemoryOperations : uint8_t
{
    GET_DELAY = 0x07,
    WAIT_KEY  = 0x0A,
    SET_DELAY = 0x15,
    ADD_I_VX  = 0x1E,
    STORE_BCD = 0x33,
    REG_DUMP  = 0x55,
    REG_LOAD  = 0x65,
};
