#include "Chip8.hpp"

void tick(Chip8& chip8)
{
    Cpu& cpu = chip8.cpu;
    Gpu& gpu = chip8.gpu;
    Display& display = chip8.display;

    uint16_t operation = cpu.readOperation();
    // printf("[Operation]: 0x%04x\n", operation);

    uint8_t opcodeMask = (operation >> 12);
    switch (opcodeMask)
    {
        case SYS:
        {
            uint16_t sysOp = (operation & 0xFF);
            switch (sysOp)
            {
                case CLEAR:
                {
                    gpu.clear();
                    display.clear();
                    cpu.PC += 2;
                } break;
                case RETURN:
                {
                    // printf("[RETURN]\n");
                    uint16_t address = 0x0000;
                    if (cpu.popFromStack(address))
                    {
                        cpu.PC = address + 2;
                    }
                    else
                    {
                        // TODO: error here
                    }
                } break;
                default: break;
            }
        } break;
        case JUMP: // 0x1NNN
        {
            uint16_t address = (operation & 0xFFF);
            // printf("[JUMP]: 0x%04x\n", address);
            cpu.PC = address;
        } break;
        case FUNC: // 0x2NNN
        {
            uint16_t address = (operation & 0xFFF);
            // printf("[FUNC]: 0x%04x\n", address);
            cpu.pushToStack(cpu.PC);
            cpu.PC = address;
        } break;
        case EQ: // 0x3XNN
        {
            uint8_t x = (operation & 0xF00) >> 8;
            uint16_t value = (operation & 0xFF);
            // printf("[EQ %d] %d == %d\n", x, cpu.registers[x], value);
            if (cpu.registers[x] == value)
            {
                cpu.PC += 4;
            }
            else
            {
                cpu.PC += 2;
            }
        } break;
        case NE: // 0x4XNN
        {
            uint8_t x = (operation & 0xF00) >> 8;
            uint16_t value = (operation & 0xFF);
            // printf("[NE %d] %d != %d\n", x, cpu.registers[x], value);
            if (cpu.registers[x] != value)
            {
                cpu.PC += 4;
            }
            else
            {
                cpu.PC += 2;
            }
        } break;
        case EQ_VX_VY: // 0x5XY0
        {
            if ((operation & 0x1) == 0)
            {
                uint8_t x = (operation & 0xF00) >> 8;
                uint8_t y = (operation & 0xF0) >> 4;
                // printf("[EQ %d %d] %d == %d\n", x, y, cpu.registers[x], cpu.registers[y]);
                if (cpu.registers[x] == cpu.registers[y])
                {
                    cpu.PC += 4;
                }
                else
                {
                    cpu.PC += 2;
                }
            }
            
        } break;
        case NE_VX_VY: // 0x9XY0
        {
            if ((operation & 0x1) == 0)
            {
                uint8_t x = (operation & 0xF00) >> 8;
                uint8_t y = (operation & 0xF0) >> 4;
                // printf("[NE %d %d] %d != %d\n", x, y, cpu.registers[x], cpu.registers[y]);
                if (cpu.registers[x] != cpu.registers[y])
                {
                    cpu.PC += 4;
                }
                else
                {
                    cpu.PC += 2;
                }
            }
        } break;
        case SET_VX: // 0x6XNN
        {
            uint8_t x = (operation & 0xF00) >> 8;
            uint16_t value = (operation & 0xFF); 
            // printf("[VAR %d] = %d\n", x, value);
            cpu.registers[x] = value;
            cpu.PC += 2;
        } break;
        case ADD_VX: // 0x7XNN
        {
            uint8_t x = (operation & 0xF00) >> 8;
            uint16_t value = (operation & 0xFF);
            // printf("[VAR %d] += %d\n", x, value);
            cpu.registers[x] += value;
            cpu.PC += 2;
        } break;
        case MATH: // 0x8XY?
        {
            uint8_t x = (operation & 0xF00) >> 8;
            uint8_t y = (operation & 0xF0) >> 4;
            uint8_t mathOp = (operation & 0xF);
            switch (mathOp)
            {
                case ASSIGN:
                {
                    cpu.registers[x] = cpu.registers[y];
                } break;
                case OR:
                {
                    cpu.registers[x] |= cpu.registers[y];
                } break;
                case AND:
                {
                    cpu.registers[x] &= cpu.registers[y];
                } break;
                case XOR:
                {
                    if (cpu.registers[x] >= cpu.registers[y])
                    {
                        cpu.registers[0xF] = 1;
                    }
                    else
                    {
                        cpu.registers[0xF] = 0;
                    }
                    cpu.registers[x] ^= cpu.registers[y];
                } break;
                case ADD:
                {
                    cpu.registers[x] += cpu.registers[y];
                } break;
                case SUB_VX_VY:
                {
                    cpu.registers[x] -= cpu.registers[y];
                } break;
                case SHIFT_RIGHT:
                {
                    cpu.registers[x] >>= 1;
                } break;
                case SUB_VY_VX:
                {
                    if (cpu.registers[y] >= cpu.registers[x])
                    {
                        cpu.registers[0xF] = 1;
                    }
                    else
                    {
                        cpu.registers[0xF] = 0;
                    }
                    cpu.registers[x] = cpu.registers[y] - cpu.registers[x];
                } break;
                case SHIFT_LEFT:
                {
                    cpu.registers[0xF] = (cpu.registers[x] >> 7);
                    cpu.registers[x] <<= 1;
                } break;
                default: break;
            }
            cpu.PC += 2;
        } break;
        case SET_I: // 0xANNN
        {
            uint16_t value = (operation & 0xFFF);
            // printf("[I] = 0x%03x\n", value);
            cpu.I = value;
            cpu.PC += 2;
        } break;
        case DRAW: // 0xDXYN
        {
            uint8_t x = (operation & 0xF00) >> 8;
            uint8_t y = (operation & 0xF0) >> 4;
            uint8_t height = (operation & 0xF);
            
            uint8_t coordX = cpu.registers[x];
            uint8_t coordY = cpu.registers[y];
            // printf("[DRAW (%d, %d)]: (I=0x%03x, N=%d)\n", coordX, coordY, cpu.I, height);

            cpu.registers[0xF] = false;
            for (int i = 0; i < height; ++i)
            {
                uint8_t rowData = cpu.memory[cpu.I + i];
                // printf("[ROW_DATA] = 0x%02x\n", rowData);
                if (gpu.draw(coordX, coordY + i, rowData))
                {
                    cpu.registers[0xF] = true;
                }
            }
            cpu.PC += 2;
        } break;
        case KEY: // EX??
        {
            uint8_t x = (operation & 0xF00) >> 8;
            uint16_t keyOp = (operation & 0xFF);
            uint8_t key = cpu.registers[x] & 0xF;
            switch (keyOp)
            {
                case EQ_KEY_VX:
                {
                    if (chip8.keyPad[key])
                    {
                        cpu.PC += 4;
                    }
                    else
                    {
                        cpu.PC += 2;
                    }
                } break;
                case NE_KEY_VX:
                {
                    if (!chip8.keyPad[key])
                    {
                        cpu.PC += 4;
                    }
                    else
                    {
                        cpu.PC += 2;
                    }
                } break;
                default: break;
            }
        } break;
        case HW: // 0xFX??
        {
            cpu.PC += 2;
            uint8_t x = (operation & 0xF00) >> 8;
            uint16_t hwOp = (operation & 0xFF);
            switch (hwOp)
            {
                case GET_DELAY:
                {
                    cpu.registers[x] = cpu.delayTimer;
                } break;
                case SET_DELAY:
                {
                    cpu.delayTimer = cpu.registers[x];
                } break;
                case WAIT_KEY:
                {
                    if (!chip8.newPress)
                    {
                        cpu.PC -= 2;
                    }
                } break;
                case STORE_BCD:
                {
                    uint16_t value = cpu.registers[x];
                    uint8_t index = 2;
                    while (value > 0)
                    {
                        // NOTE: Ideally could also reverse number using
                        //       divides and mods and update based on reversed
                        cpu.memory[cpu.I + index] = value % 10;
                        value /= 10;
                        index--;
                    }
                } break;
                case ADD_I_VX:
                {
                    cpu.I += cpu.registers[x];
                } break;
                case REG_DUMP:
                {
                    memcpy(&cpu.memory[cpu.I], cpu.registers, x + 1);
                } break;
                case REG_LOAD:
                {
                    memcpy(cpu.registers, &cpu.memory[cpu.I], x + 1);
                }; break;
                default: break;
            }
        } break;
        default: break;
    } 
}

void updateTimers(Chip8& chip8)
{
    uint8_t& delayTimer = chip8.cpu.delayTimer;
    uint8_t& soundTimer = chip8.cpu.soundTimer;

    if (delayTimer > 0)
    {
        delayTimer--;
    }
    if (soundTimer > 0)
    {
        soundTimer--;
    }
}
