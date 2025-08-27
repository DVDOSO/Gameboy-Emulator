#include <iostream>
#include <stdio.h>
#include <fstream>
#include "ppu.hpp"
#include <SDL2/SDL.h>

using namespace std;

uint8_t *rom_data;
uint8_t *memory;
int m_cycles = 0;
bool stopped = false, halted = false, ime_flag = false, ei_flag = false, ei = false;

struct{
    uint8_t a;
    uint8_t f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t pc;
    uint16_t sp;
} registers;

void setZFlag(bool val){
    val? registers.f |= (1 << 7) : registers.f &= ~(1 << 7);
}

void setNFlag(bool val){
    val? registers.f |= (1 << 6) : registers.f &= ~(1 << 6);
}

void setHFlag(bool val){
    val? registers.f |= (1 << 5) : registers.f &= ~(1 << 5);
}

void setCFlag(bool val){
    val? registers.f |= (1 << 4) : registers.f &= ~(1 << 4);
}

void execute_prefix_instruction(){
    uint8_t instruction = rom_data[registers.pc];
    switch(instruction){
        case 0x00: { // RLC B
            uint8_t bit7 = (registers.b >> 7) & 1;
            registers.b = (registers.b << 1) | bit7;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x01: { // RLC C
            uint8_t bit7 = (registers.c >> 7) & 1;
            registers.c = (registers.c << 1) | bit7;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x02: { // RLC D
            uint8_t bit7 = (registers.d >> 7) & 1;
            registers.d = (registers.d << 1) | bit7;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x03: { // RLC E
            uint8_t bit7 = (registers.e >> 7) & 1;
            registers.e = (registers.e << 1) | bit7;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x04: { // RLC H
            uint8_t bit7 = (registers.h >> 7) & 1;
            registers.h = (registers.h << 1) | bit7;
            registers.h == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x05: { // RLC L
            uint8_t bit7 = (registers.l >> 7) & 1;
            registers.l = (registers.l << 1) | bit7;
            registers.l == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x06: { // RLC [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t bit7 = (memory[hl] >> 7) & 1;
            memory[hl] = (memory[hl] << 1) | bit7;
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x07: { // RLC A
            uint8_t bit7 = (registers.a >> 7) & 1;
            registers.a = (registers.a << 1) | bit7;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x08: { // RRC B
            uint8_t bit0 = registers.b & 1;
            registers.b = (registers.b >> 1) | (bit0 << 7);
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x09: { // RRC C
            uint8_t bit0 = registers.c & 1;
            registers.c = (registers.c >> 1) | (bit0 << 7);
            registers.c == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x0A: { // RRC D
            uint8_t bit0 = registers.d & 1;
            registers.d = (registers.d >> 1) | (bit0 << 7);
            registers.d == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x0B: { // RRC E
            uint8_t bit0 = registers.e & 1;
            registers.e = (registers.e >> 1) | (bit0 << 7);
            registers.e == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x0C: { // RRC H
            uint8_t bit0 = registers.h & 1;
            registers.h = (registers.h >> 1) | (bit0 << 7);
            registers.h == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x0D: { // RRC L
            uint8_t bit0 = registers.l & 1;
            registers.l = (registers.l >> 1) | (bit0 << 7);
            registers.l == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x0E: { // RRC [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t bit0 = memory[hl] & 1;
            memory[hl] = (memory[hl] >> 1) | (bit0 << 7);
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x0F: { // RRC A
            uint8_t bit0 = registers.a & 1;
            registers.a = (registers.a >> 1) | (bit0 << 7);
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x10: { // RL B
            uint8_t bit7 = (1 << 7) & registers.b;
            registers.b = (registers.b << 1) | ((registers.f >> 4) & 1);
            registers.b == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x11: { // RL C
            uint8_t bit7 = (1 << 7) & registers.c;
            registers.c = (registers.c << 1) | ((registers.f >> 4) & 1);
            registers.c == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x12: { // RL D
            uint8_t bit7 = (1 << 7) & registers.d;
            registers.d = (registers.d << 1) | ((registers.f >> 4) & 1);
            registers.d == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x13: { // RL E
            uint8_t bit7 = (1 << 7) & registers.e;
            registers.e = (registers.e << 1) | ((registers.f >> 4) & 1);
            registers.e == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x14: { // RL H
            uint8_t bit7 = (1 << 7) & registers.h;
            registers.h = (registers.h << 1) | ((registers.f >> 4) & 1);
            registers.h == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x15: { // RL L
            uint8_t bit7 = (1 << 7) & registers.l;
            registers.l = (registers.l << 1) | ((registers.f >> 4) & 1);
            registers.l == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x16: { // RL [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t bit7 = (1 << 7) & memory[hl];
            memory[hl] = (memory[hl] << 1) | ((registers.f >> 4) & 1);
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x17: { // RL A
            uint8_t bit7 = (1 << 7) & registers.a;
            registers.a = (registers.a << 1) | ((registers.f >> 4) & 1);
            registers.a == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x18: { // RR B
            uint8_t bit0 = registers.b & 1;
            registers.b = (registers.b >> 1) | (((registers.f >> 4) & 1) << 7);
            registers.b == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x19: { // RR C
            uint8_t bit0 = registers.c & 1;
            registers.c = (registers.c >> 1) | (((registers.f >> 4) & 1) << 7);
            registers.c == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x1A: { // RR D
            uint8_t bit0 = registers.d & 1;
            registers.d = (registers.d >> 1) | (((registers.f >> 4) & 1) << 7);
            registers.d == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x1B: { // RR E
            uint8_t bit0 = registers.e & 1;
            registers.e = (registers.e >> 1) | (((registers.f >> 4) & 1) << 7);
            registers.e == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x1C: { // RR H
            uint8_t bit0 = registers.h & 1;
            registers.h = (registers.h >> 1) | (((registers.f >> 4) & 1) << 7);
            registers.h == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x1D: { // RR L
            uint8_t bit0 = registers.l & 1;
            registers.l = (registers.l >> 1) | (((registers.f >> 4) & 1) << 7);
            registers.l == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x1E: { // RR [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t bit0 = memory[hl] & 1;
            memory[hl] = (memory[hl] >> 1) | (((registers.f >> 4) & 1) << 7);
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x1F: { // RR A
            uint8_t bit0 = registers.a & 1;
            registers.a = (registers.a >> 1) | (((registers.f >> 4) & 1) << 7);
            registers.a == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x20: { // SLA B
            uint8_t bit7 = (1 << 7) & registers.b;
            registers.b = registers.b << 1;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x21: { // SLA C
            uint8_t bit7 = (1 << 7) & registers.c;
            registers.c = registers.c << 1;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x22: { // SLA D
            uint8_t bit7 = (1 << 7) & registers.d;
            registers.d = registers.d << 1;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x23: { // SLA E
            uint8_t bit7 = (1 << 7) & registers.e;
            registers.e = registers.e << 1;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x24: { // SLA H
            uint8_t bit7 = (1 << 7) & registers.h;
            registers.h = registers.h << 1;
            registers.h == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x25: { // SLA L
            uint8_t bit7 = (1 << 7) & registers.l;
            registers.l = registers.l << 1;
            registers.l == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x26: { // SLA [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t bit7 = (1 << 7) & memory[hl];
            memory[hl] = memory[hl] << 1;
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x27: { // SLA A
            uint8_t bit7 = (1 << 7) & registers.a;
            registers.a = registers.a << 1;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x28: { // SRA B
            uint8_t bit0 = registers.b & 1;
            uint8_t bit7 = (1 << 7) & registers.b;
            registers.b = (registers.b >> 1) | bit7;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x29: { // SRA C
            uint8_t bit0 = registers.c & 1;
            uint8_t bit7 = (1 << 7) & registers.c;
            registers.c = (registers.c >> 1) | bit7;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x2A: { // SRA D
            uint8_t bit0 = registers.d & 1;
            uint8_t bit7 = (1 << 7) & registers.d;
            registers.d = (registers.d >> 1) | bit7;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x2B: { // SRA E
            uint8_t bit0 = registers.e & 1;
            uint8_t bit7 = (1 << 7) & registers.e;
            registers.e = (registers.e >> 1) | bit7;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x2C: { // SRA H
            uint8_t bit0 = registers.h & 1;
            uint8_t bit7 = (1 << 7) & registers.h;
            registers.h = (registers.h >> 1) | bit7;
            registers.h == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x2D: { // SRA L
            uint8_t bit0 = registers.l & 1;
            uint8_t bit7 = (1 << 7) & registers.l;
            registers.l = (registers.l >> 1) | bit7;
            registers.l == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x2E: { // SRA [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t bit0 = memory[hl] & 1;
            uint8_t bit7 = (1 << 7) & memory[hl];
            memory[hl] = (memory[hl] >> 1) | bit7;
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x2F: { // SRA A
            uint8_t bit0 = registers.a & 1;
            uint8_t bit7 = (1 << 7) & registers.a;
            registers.a = (registers.a >> 1) | bit7;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x30: { // SWAP B
            registers.b = ((registers.b >> 4) & 0x0F) | ((registers.b << 4) & 0xF0);
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x31: { // SWAP C
            registers.c = ((registers.c >> 4) & 0x0F) | ((registers.c << 4) & 0xF0);
            registers.c == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x32: { // SWAP D
            registers.d = ((registers.d >> 4) & 0x0F) | ((registers.d << 4) & 0xF0);
            registers.d == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x33: { // SWAP E
            registers.e = ((registers.e >> 4) & 0x0F) | ((registers.e << 4) & 0xF0);
            registers.e == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x34: { // SWAP H
            registers.h = ((registers.h >> 4) & 0x0F) | ((registers.h << 4) & 0xF0);
            registers.h == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x35: { // SWAP L
            registers.l = ((registers.l >> 4) & 0x0F) | ((registers.l << 4) & 0xF0);
            registers.l == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x36: { // SWAP [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = ((memory[hl] >> 4) & 0x0F) | ((memory[hl] << 4) & 0xF0);
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x37: { // SWAP A
            registers.a = ((registers.a >> 4) & 0x0F) | ((registers.a << 4) & 0xF0);
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x38: { // SRL B
            uint8_t bit0 = registers.b & 1;
            registers.b = registers.b >> 1;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x39: { // SRL C
            uint8_t bit0 = registers.c & 1;
            registers.c = registers.c >> 1;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x3A: { // SRL D
            uint8_t bit0 = registers.d & 1;
            registers.d = registers.d >> 1;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x3B: { // SRL E
            uint8_t bit0 = registers.e & 1;
            registers.e = registers.e >> 1;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x3C: { // SRL H
            uint8_t bit0 = registers.h & 1;
            registers.h = registers.h >> 1;
            registers.h == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x3D: { // SRL L
            uint8_t bit0 = registers.l & 1;
            registers.l = registers.l >> 1;
            registers.l == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x3E: { // SRL [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t bit0 = memory[hl] & 1;
            memory[hl] = memory[hl] >> 1;
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x3F: { // SRL A
            uint8_t bit0 = registers.a & 1;
            registers.a = registers.a >> 1;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            setHFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x40: { // BIT 0, B
            (registers.b & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x41: { // BIT 0, C
            (registers.c & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x42: { // BIT 0, D
            (registers.d & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x43: { // BIT 0, E
            (registers.e & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x44: { // BIT 0, H
            (registers.h & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x45: { // BIT 0, L
            (registers.l & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x46: { // BIT 0, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x47: { // BIT 0, A
            (registers.a & 1) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x48: { // BIT 1, B
            (registers.b & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x49: { // BIT 1, C
            (registers.c & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x4A: { // BIT 1, D
            (registers.d & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x4B: { // BIT 1, E
            (registers.e & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x4C: { // BIT 1, H
            (registers.h & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x4D: { // BIT 1, L
            (registers.l & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x4E: { // BIT 1, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x4F: { // BIT 1, A
            (registers.a & (1 << 1)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x50: { // BIT 2, B
            (registers.b & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x51: { // BIT 2, C
            (registers.c & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x52: { // BIT 2, D
            (registers.d & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x53: { // BIT 2, E
            (registers.e & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x54: { // BIT 2, H
            (registers.h & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x55: { // BIT 2, L
            (registers.l & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x56: { // BIT 2, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x57: { // BIT 2, A
            (registers.a & (1 << 2)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x58: { // BIT 3, B
            (registers.b & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x59: { // BIT 3, C
            (registers.c & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x5A: { // BIT 3, D
            (registers.d & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x5B: { // BIT 3, E
            (registers.e & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x5C: { // BIT 3, H
            (registers.h & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x5D: { // BIT 3, L
            (registers.l & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x5E: { // BIT 3, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x5F: { // BIT 3, A
            (registers.a & (1 << 3)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x60: { // BIT 4, B
            (registers.b & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x61: { // BIT 4, C
            (registers.c & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x62: { // BIT 4, D
            (registers.d & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x63: { // BIT 4, E
            (registers.e & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x64: { // BIT 4, H
            (registers.h & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x65: { // BIT 4, L
            (registers.l & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x66: { // BIT 4, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x67: { // BIT 4, A
            (registers.a & (1 << 4)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x68: { // BIT 5, B
            (registers.b & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x69: { // BIT 5, C
            (registers.c & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x6A: { // BIT 5, D
            (registers.d & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x6B: { // BIT 5, E
            (registers.e & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x6C: { // BIT 5, H
            (registers.h & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x6D: { // BIT 5, L
            (registers.l & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x6E: { // BIT 5, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x6F: { // BIT 5, A
            (registers.a & (1 << 5)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x70: { // BIT 6, B
            (registers.b & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x71: { // BIT 6, C
            (registers.c & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x72: { // BIT 6, D
            (registers.d & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x73: { // BIT 6, E
            (registers.e & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x74: { // BIT 6, H
            (registers.h & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x75: { // BIT 6, L
            (registers.l & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x76: { // BIT 6, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x77: { // BIT 6, A
            (registers.a & (1 << 6)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x78: { // BIT 7, B
            (registers.b & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x79: { // BIT 7, C
            (registers.c & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x7A: { // BIT 7, D
            (registers.d & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x7B: { // BIT 7, E
            (registers.e & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x7C: { // BIT 7, H
            (registers.h & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x7D: { // BIT 7, L
            (registers.l & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x7E: { // BIT 7, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x7F: { // BIT 7, A
            (registers.a & (1 << 7)) == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x80: { // RES 0, B
            registers.b &= ~(1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x81: { // RES 0, C
            registers.c &= ~(1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x82: { // RES 0, D
            registers.d &= ~(1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x83: { // RES 0, E
            registers.e &= ~(1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x84: { // RES 0, H
            registers.h &= ~(1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x85: { // RES 0, L
            registers.l &= ~(1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x86: { // RES 0, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 0);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x87: { // RES 0, A
            registers.a &= ~(1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x88: { // RES 1, B
            registers.b &= ~(1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x89: { // RES 1, C
            registers.c &= ~(1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8A: { // RES 1, D
            registers.d &= ~(1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8B: { // RES 1, E
            registers.e &= ~(1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8C: { // RES 1, H
            registers.h &= ~(1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8D: { // RES 1, L
            registers.l &= ~(1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8E: { // RES 1, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 1);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x8F: { // RES 1, A
            registers.a &= ~(1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x90: { // RES 2, B
            registers.b &= ~(1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x91: { // RES 2, C
            registers.c &= ~(1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x92: { // RES 2, D
            registers.d &= ~(1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x93: { // RES 2, E
            registers.e &= ~(1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x94: { // RES 2, H
            registers.h &= ~(1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x95: { // RES 2, L
            registers.l &= ~(1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x96: { // RES 2, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 2);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x97: { // RES 2, A
            registers.a &= ~(1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x98: { // RES 3, B
            registers.b &= ~(1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x99: { // RES 3, C
            registers.c &= ~(1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9A: { // RES 3, D
            registers.d &= ~(1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9B: { // RES 3, E
            registers.e &= ~(1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9C: { // RES 3, H
            registers.h &= ~(1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9D: { // RES 3, L
            registers.l &= ~(1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9E: { // RES 3, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 3);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x9F: { // RES 3, A
            registers.a &= ~(1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA0: { // RES 4, B
            registers.b &= ~(1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA1: { // RES 4, C
            registers.c &= ~(1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA2: { // RES 4, D
            registers.d &= ~(1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA3: { // RES 4, E
            registers.e &= ~(1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA4: { // RES 4, H
            registers.h &= ~(1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA5: { // RES 4, L
            registers.l &= ~(1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA6: { // RES 4, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 4);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xA7: { // RES 4, A
            registers.a &= ~(1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA8: { // RES 5, B
            registers.b &= ~(1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA9: { // RES 5, C
            registers.c &= ~(1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAA: { // RES 5, D
            registers.d &= ~(1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAB: { // RES 5, E
            registers.e &= ~(1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAC: { // RES 5, H
            registers.h &= ~(1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAD: { // RES 5, L
            registers.l &= ~(1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAE: { // RES 5, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 5);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xAF: { // RES 5, A
            registers.a &= ~(1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB0: { // RES 6, B
            registers.b &= ~(1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB1: { // RES 6, C
            registers.c &= ~(1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB2: { // RES 6, D
            registers.d &= ~(1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB3: { // RES 6, E
            registers.e &= ~(1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB4: { // RES 6, H
            registers.h &= ~(1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB5: { // RES 6, L
            registers.l &= ~(1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB6: { // RES 6, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 6);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xB7: { // RES 6, A
            registers.a &= ~(1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB8: { // RES 7, B
            registers.b &= ~(1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB9: { // RES 7, C
            registers.c &= ~(1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBA: { // RES 7, D
            registers.d &= ~(1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBB: { // RES 7, E
            registers.e &= ~(1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBC: { // RES 7, H
            registers.h &= ~(1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBD: { // RES 7, L
            registers.l &= ~(1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBE: { // RES 7, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] &= ~(1 << 7);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xBF: { // RES 7, A
            registers.a &= ~(1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC0: { // SET 0, B
            registers.b |= (1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC1: { // SET 0, C
            registers.c |= (1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC2: { // SET 0, D
            registers.d |= (1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC3: { // SET 0, E
            registers.e |= (1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC4: { // SET 0, H
            registers.h |= (1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC5: { // SET 0, L
            registers.l |= (1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC6: { // SET 0, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 0);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xC7: { // SET 0, A
            registers.a |= (1 << 0);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC8: { // SET 1, B
            registers.b |= (1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC9: { // SET 1, C
            registers.c |= (1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xCA: { // SET 1, D
            registers.d |= (1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xCB: { // SET 1, E
            registers.e |= (1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xCC: { // SET 1, H
            registers.h |= (1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xCD: { // SET 1, L
            registers.l |= (1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xCE: { // SET 1, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 1);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xCF: { // SET 1, A
            registers.a |= (1 << 1);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD0: { // SET 2, B
            registers.b |= (1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD1: { // SET 2, C
            registers.c |= (1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD2: { // SET 2, D
            registers.d |= (1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD3: { // SET 2, E
            registers.e |= (1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD4: { // SET 2, H
            registers.h |= (1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD5: { // SET 2, L
            registers.l |= (1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD6: { // SET 2, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 2);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xD7: { // SET 2, A
            registers.a |= (1 << 2);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD8: { // SET 3, B
            registers.b |= (1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xD9: { // SET 3, C
            registers.c |= (1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xDA: { // SET 3, D
            registers.d |= (1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xDB: { // SET 3, E
            registers.e |= (1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xDC: { // SET 3, H
            registers.h |= (1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xDD: { // SET 3, L
            registers.l |= (1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xDE: { // SET 3, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 3);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xDF: { // SET 3, A
            registers.a |= (1 << 3);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE0: { // SET 4, B
            registers.b |= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE1: { // SET 4, C
            registers.c |= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE2: { // SET 4, D
            registers.d |= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE3: { // SET 4, E
            registers.e |= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE4: { // SET 4, H
            registers.h |= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE5: { // SET 4, L
            registers.l |= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE6: { // SET 4, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 4);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xE7: { // SET 4, A
            registers.a |= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE8: { // SET 5, B
            registers.b |= (1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xE9: { // SET 5, C
            registers.c |= (1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xEA: { // SET 5, D
            registers.d |= (1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xEB: { // SET 5, E
            registers.e |= (1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xEC: { // SET 5, H
            registers.h |= (1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xED: { // SET 5, L
            registers.l |= (1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xEE: { // SET 5, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 5);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xEF: { // SET 5, A
            registers.a |= (1 << 5);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF0: { // SET 6, B
            registers.b |= (1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF1: { // SET 6, C
            registers.c |= (1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF2: { // SET 6, D
            registers.d |= (1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF3: { // SET 6, E
            registers.e |= (1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF4: { // SET 6, H
            registers.h |= (1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF5: { // SET 6, L
            registers.l |= (1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF6: { // SET 6, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 6);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xF7: { // SET 6, A
            registers.a |= (1 << 6);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF8: { // SET 7, B
            registers.b |= (1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF9: { // SET 7, C
            registers.c |= (1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xFA: { // SET 7, D
            registers.d |= (1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xFB: { // SET 7, E
            registers.e |= (1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xFC: { // SET 7, H
            registers.h |= (1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xFD: { // SET 7, L
            registers.l |= (1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xFE: { // SET 7, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] |= (1 << 7);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xFF: { // SET 7, A
            registers.a |= (1 << 7);
            m_cycles++;
            registers.pc++;
            break;
        }
    }
}

void execute_instruction(){
    uint8_t instruction = rom_data[registers.pc];
    switch(instruction){
        case 0x00: { // NOP
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x01: { // LD BC, n16
            registers.pc++;
            registers.c = rom_data[registers.pc];
            registers.pc++;
            registers.b = rom_data[registers.pc];
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x02: { // LD [BC], A
            uint16_t bc = (registers.b << 8) | registers.c;
            memory[bc] = registers.a;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x03: { // INC BC
            uint16_t bc = (registers.b << 8) | registers.c;
            bc++;
            registers.b = (bc >> 8) & 0xFF;
            registers.c = bc & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x04: { // INC B
            (registers.b & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.b++;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x05: { // DEC B
            (registers.b & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.b--;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x06: { // LD B, n8
            registers.pc++;
            registers.b = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x07: { // RLCA
            uint8_t bit7 = (registers.a >> 7) & 1;
            registers.a = (registers.a << 1) | bit7;
            setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x08: { // LD [a16], SP
            registers.pc++;
            uint16_t low = rom_data[registers.pc];
            registers.pc++;
            uint16_t high = rom_data[registers.pc];
            uint16_t dest = (high << 8) | low;
            memory[dest] = registers.sp & 0xFF;
            memory[dest + 1] = (registers.sp >> 8) & 0xFF;
            m_cycles += 5;
            registers.pc++;
            break;
        }
        case 0x09: { // ADD HL, BC
            uint16_t hl = (registers.h << 8) | registers.l;
            uint16_t bc = (registers.b << 8) | registers.c;
            ((hl & 0x0FFF) + (bc & 0x0FFF) > 0x0FFF)? setHFlag(true) : setHFlag(false);
            (hl + bc > 0xFFFF)? setCFlag(true) : setCFlag(false);
            hl += bc;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x0A: { // LD A, [BC]
            uint16_t bc = (registers.b << 8) | registers.c;
            registers.a = memory[bc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x0B: { // DEC BC
            uint16_t bc = (registers.b << 8) | registers.c;
            bc--;
            registers.b = (bc >> 8) & 0xFF;
            registers.c = bc & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x0C: { // INC C
            (registers.c & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.c++;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x0D: { // DEC C
            (registers.c & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.c--;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x0E: { // LD C, n8
            registers.pc++;
            registers.c = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x0F: { // RRCA
            uint8_t bit0 = registers.a & 1;
            registers.a = (registers.a >> 1) | (bit0 << 7);
            setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x10: { // STOP n8
            registers.pc++;
            stopped = true;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x11: { // LD DE, n16
            registers.pc++;
            registers.e = rom_data[registers.pc];
            registers.pc++;
            registers.d = rom_data[registers.pc];
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x12: { // LD [DE], A
            uint16_t de = (registers.d << 8) | registers.e;
            memory[de] = registers.a;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x13: { // INC DE
            uint16_t de = (registers.d << 8) | registers.e;
            de++;
            registers.d = (de >> 8) & 0xFF;
            registers.e = de & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x14: { // INC D
            (registers.d & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.d++;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x15: { // DEC D
            (registers.d & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.d--;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x16: { // LD D, n8
            registers.pc++;
            registers.d = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x17: { // RLA
            uint8_t bit7 = (1 << 7) & registers.a;
            registers.a = (registers.a << 1) | ((registers.f >> 4) & 1);
            bit7? setCFlag(true) : setCFlag(false);
            setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x18: { // JR e8
            registers.pc++;
            int8_t offset = rom_data[registers.pc];
            registers.pc++;
            registers.pc += offset;
            m_cycles += 3;
            break;
        }
        case 0x19: { // ADD HL, DE
            uint16_t hl = (registers.h << 8) | registers.l;
            uint16_t de = (registers.d << 8) | registers.e;
            ((hl & 0x0FFF) + (de & 0x0FFF) > 0x0FFF)? setHFlag(true) : setHFlag(false);
            (hl + de > 0xFFFF)? setCFlag(true) : setCFlag(false);
            hl += de;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x1A: { // LD A, [DE]
            uint16_t de = (registers.d << 8) | registers.e;
            registers.a = memory[de];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x1B: { // DEC DE
            uint16_t de = (registers.d << 8) | registers.e;
            de--;
            registers.d = (de >> 8) & 0xFF;
            registers.e = de & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x1C: { // INC E
            (registers.e & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.e++;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x1D: { // DEC E
            (registers.e & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.e--;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x1E: { // LD E, n8
            registers.pc++;
            registers.e = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x1F: { // RRA
            uint8_t bit0 = registers.a & 1;
            registers.a = (registers.a >> 1) | (((registers.f >> 4) & 1) << 7);
            bit0? setCFlag(true) : setCFlag(false);
            setNFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x20: { // JR NZ, e8
            registers.pc++;
            int8_t offset = rom_data[registers.pc];
            registers.pc++;
            if(!(registers.f & (1 << 7))){
                registers.pc += offset;
                m_cycles += 3;
            }
            else{
                m_cycles += 2;
            }
            break;
        }
        case 0x21: { // LD HL, n16
            registers.pc++;
            registers.l = rom_data[registers.pc];
            registers.pc++;
            registers.h = rom_data[registers.pc];
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x22: { // LD [HL+], A
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.a;
            hl++;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x23: { // INC HL
            uint16_t hl = (registers.h << 8) | registers.l;
            hl++;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x24: { // INC H
            (registers.h & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.h++;
            registers.h == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x25: { // DEC H
            (registers.h & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.h--;
            registers.h == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x26: { // LD H, n8
            registers.pc++;
            registers.h = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x27: { // DAA
            int adjustment = 0;
            if(registers.f & (1 << 6)){
                if(registers.f & (1 << 5)) adjustment += 0x6;
                if(registers.f & (1 << 4)) adjustment += 0x60;
                registers.a -= adjustment;
            }
            else{
                if((registers.f & (1 << 5)) || ((registers.a & 0xF) > 0x9)) adjustment += 0x6;
                if((registers.f & (1 << 4)) || (registers.a > 0x99)){
                    adjustment += 0x60;
                    setCFlag(true);
                }
                else setCFlag(false);
                registers.a += adjustment;
            }
            (registers.a == 0)? setZFlag(true) : setZFlag(false);
            setHFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x28: { // JR Z, e8
            registers.pc++;
            int8_t offset = rom_data[registers.pc];
            registers.pc++;
            if(registers.f & (1 << 7)){
                registers.pc += offset;
                m_cycles += 3;
            }
            else{
                m_cycles += 2;
            }
            break;
        }
        case 0x29: { // ADD HL, HL
            uint16_t hl = (registers.h << 8) | registers.l;
            ((hl & 0x0FFF) + (hl & 0x0FFF) > 0x0FFF)? setHFlag(true) : setHFlag(false);
            (hl + hl > 0xFFFF)? setCFlag(true) : setCFlag(false);
            hl += hl;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x2A: { // LD A, [HL+]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.a = memory[hl];
            hl++;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x2B: { // DEC HL
            uint16_t hl = (registers.h << 8) | registers.l;
            hl--;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x2C: { // INC L
            (registers.l & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.l++;
            registers.l == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x2D: { // DEC L
            (registers.l & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.l--;
            registers.l == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x2E: { // LD L, n8
            registers.pc++;
            registers.l = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x2F: { // CPL
            registers.a = ~registers.a;
            setNFlag(true);
            setHFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x30: { // JR NC, e8
            registers.pc++;
            int8_t offset = rom_data[registers.pc];
            registers.pc++;
            if(!(registers.f & (1 << 4))){
                registers.pc += offset;
                m_cycles += 3;
            }
            else{
                m_cycles += 2;
            }
            break;
        }
        case 0x31: { // LD SP, n16
            registers.pc++;
            uint8_t p = rom_data[registers.pc];
            registers.pc++;
            uint8_t s = rom_data[registers.pc];
            uint16_t sp = (s << 8) | p;
            registers.sp = sp;
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x32: { // LD [HL-], A
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.a;
            hl--;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x33: { // INC SP
            registers.sp++;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x34: { // INC [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            memory[hl]++;
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x35: { // DEC [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            (memory[hl] & 0xF) == 0x0? setHFlag(true) : setHFlag(false);
            memory[hl]--;
            memory[hl] == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x36: { // LD [HL], n8
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.pc++;
            memory[hl] = rom_data[registers.pc];
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0x37: { // SCF
            setNFlag(false);
            setHFlag(false);
            setCFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x38: { // JR C, e8
            registers.pc++;
            int8_t offset = rom_data[registers.pc];
            registers.pc++;
            if(registers.f & (1 << 4)){
                registers.pc += offset;
                m_cycles += 3;
            }
            else{
                m_cycles += 2;
            }
            break;
        }
        case 0x39: { // ADD HL, SP
            uint16_t hl = (registers.h << 8) | registers.l;
            ((hl & 0x0FFF) + (registers.sp & 0x0FFF) > 0x0FFF)? setHFlag(true) : setHFlag(false);
            (hl + registers.sp > 0xFFFF)? setCFlag(true) : setCFlag(false);
            hl += registers.sp;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x3A: { // LD A, [HL-]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.a = memory[hl];
            hl--;
            registers.h = (hl >> 8) & 0xFF;
            registers.l = hl & 0xFF;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x3B: { // DEC SP
            registers.sp--;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x3C: { // INC A
            (registers.a & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.a++;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x3D: { // DEC A
            (registers.a & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.a--;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x3E: { // LD A, n8
            registers.pc++;
            registers.a = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x3F: { // CCF
            setNFlag(false);
            setHFlag(false);
            registers.f ^= (1 << 4);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x40: { // LD B, B
            registers.b = registers.b;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x41: { // LD B, C
            registers.b = registers.c;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x42: { // LD B, D
            registers.b = registers.d;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x43: { // LD B, E
            registers.b = registers.e;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x44: { // LD B, H
            registers.b = registers.h;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x45: { // LD B, L
            registers.b = registers.l;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x46: { // LD B, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.b = memory[hl];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x47: { // LD B, A
            registers.b = registers.a;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x48: { // LD C, B
            registers.c = registers.b;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x49: { // LD C, C
            registers.c = registers.c;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x4A: { // LD C, D
            registers.c = registers.d;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x4B: { // LD C, E
            registers.c = registers.e;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x4C: { // LD C, H
            registers.c = registers.h;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x4D: { // LD C, L
            registers.c = registers.l;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x4E: { // LD C, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.c = memory[hl];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x4F: { // LD C, A
            registers.c = registers.a;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x50: { // LD D, B
            registers.d = registers.b;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x51: { // LD D, C
            registers.d = registers.c;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x52: { // LD D, D
            registers.d = registers.d;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x53: { // LD D, E
            registers.d = registers.e;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x54: { // LD D, H
            registers.d = registers.h;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x55: { // LD D, L
            registers.d = registers.l;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x56: { // LD D, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.c = memory[hl];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x57: { // LD D, A
            registers.d = registers.a;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x58: { // LD E, B
            registers.e = registers.b;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x59: { // LD E, C
            registers.e = registers.c;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x5A: { // LD E, D
            registers.e = registers.d;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x5B: { // LD E, E
            registers.e = registers.e;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x5C: { // LD E, H
            registers.e = registers.h;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x5D: { // LD E, L
            registers.e = registers.l;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x5E: { // LD E, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.e = memory[hl];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x5F: { // LD E, A
            registers.e = registers.a;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x60: { // LD H, B
            registers.h = registers.b;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x61: { // LD H, C
            registers.h = registers.c;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x62: { // LD H, D
            registers.h = registers.d;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x63: { // LD H, E
            registers.h = registers.e;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x64: { // LD H, H
            registers.h = registers.h;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x65: { // LD H, L
            registers.h = registers.l;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x66: { // LD H, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.h = memory[hl];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x67: { // LD H, A
            registers.h = registers.a;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x68: { // LD L, B
            registers.l = registers.b;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x69: { // LD L, C
            registers.l = registers.c;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x6A: { // LD L, D
            registers.l = registers.d;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x6B: { // LD L, E
            registers.l = registers.e;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x6C: { // LD L, H
            registers.l = registers.h;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x6D: { // LD L, L
            registers.l = registers.l;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x6E: { // LD L, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.l = memory[hl];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x6F: { // LD L, A
            registers.l = registers.a;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x70: { // LD [HL], B
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.b;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x71: { // LD [HL], C
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.c;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x72: { // LD [HL], D
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.d;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x73: { // LD [HL], E
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.e;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x74: { // LD [HL], H
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.h;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x75: { // LD [HL], L
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.l;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x76: { // HALT
            if(ime_flag && ((memory[0xFF0F] & memory[0xFFFF]) != 0)){
                registers.pc++;
                m_cycles += 1;
            }
            else{
                halted = true;
                m_cycles += 1;
            }
            break;
        }
        case 0x77: { // LD [HL], A
            uint16_t hl = (registers.h << 8) | registers.l;
            memory[hl] = registers.a;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x78: { // LD A, B
            registers.a = registers.b;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x79: { // LD A, C
            registers.a = registers.c;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x7A: { // LD A, D
            registers.a = registers.d;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x7B: { // LD A, E
            registers.a = registers.e;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x7C: { // LD A, H
            registers.a = registers.h;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x7D: { // LD A, L
            registers.a = registers.l;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x7E: { // LD A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.a = memory[hl];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x7F: { // LD A, A
            registers.a = registers.a;
            m_cycles += 1;
            registers.pc++;
            break;
        }
        case 0x80: { // ADD A, B
            uint16_t result = registers.a + registers.b;
            ((registers.a & 0x0F) + (registers.b & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x81: { // ADD A, C
            uint16_t result = registers.a + registers.c;
            ((registers.a & 0x0F) + (registers.c & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x82: { // ADD A, D
            uint16_t result = registers.a + registers.d;
            ((registers.a & 0x0F) + (registers.d & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x83: { // ADD A, E
            uint16_t result = registers.a + registers.e;
            ((registers.a & 0x0F) + (registers.e & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x84: { // ADD A, H
            uint16_t result = registers.a + registers.h;
            ((registers.a & 0x0F) + (registers.h & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x85: { // ADD A, L
            uint16_t result = registers.a + registers.l;
            ((registers.a & 0x0F) + (registers.l & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x86: { // ADD A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint16_t result = registers.a + memory[hl];
            ((registers.a & 0x0F) + (memory[hl] & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x87: { // ADD A, A
            uint16_t result = registers.a + registers.a;
            ((registers.a & 0x0F) + (registers.a & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x88: { // ADC A, B
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + registers.b + carry;
            ((registers.a & 0x0F) + (registers.b & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x89: { // ADC A, C
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + registers.c + carry;
            ((registers.a & 0x0F) + (registers.c & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8A: { // ADC A, D
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + registers.d + carry;
            ((registers.a & 0x0F) + (registers.d & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8B: { // ADC A, E
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + registers.e + carry;
            ((registers.a & 0x0F) + (registers.e & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8C: { // ADC A, H
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + registers.h + carry;
            ((registers.a & 0x0F) + (registers.h & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8D: { // ADC A, L
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + registers.l + carry;
            ((registers.a & 0x0F) + (registers.l & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x8E: { // ADC A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + memory[hl] + carry;
            ((registers.a & 0x0F) + (memory[hl] & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x8F: { // ADC A, A
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + registers.a + carry;
            ((registers.a & 0x0F) + (registers.a & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x90: { // SUB A, B
            uint16_t result = registers.a - registers.b;
            (registers.a & 0x0F) < (registers.b & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.b? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x91: { // SUB A, C
            uint16_t result = registers.a - registers.c;
            (registers.a & 0x0F) < (registers.c & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.c? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x92: { // SUB A, D
            uint16_t result = registers.a - registers.d;
            (registers.a & 0x0F) < (registers.d & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.d? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x93: { // SUB A, E
            uint16_t result = registers.a - registers.e;
            (registers.a & 0x0F) < (registers.e & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.e? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x94: { // SUB A, H
            uint16_t result = registers.a - registers.h;
            (registers.a & 0x0F) < (registers.h & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.h? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x95: { // SUB A, L
            uint16_t result = registers.a - registers.l;
            (registers.a & 0x0F) < (registers.l & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.l? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x96: { // SUB A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint16_t result = registers.a - memory[hl];
            (registers.a & 0x0F) < (memory[hl] & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < memory[hl]? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x97: { // SUB A, A
            uint16_t result = registers.a - registers.a;
            (registers.a & 0x0F) < (registers.a & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.a? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x98: { // SBC A, B
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - registers.b - carry;
            (((registers.a & 0x0F) - (registers.b & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (registers.b + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x99: { // SBC A, C
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - registers.c - carry;
            (((registers.a & 0x0F) - (registers.c & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (registers.c + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9A: { // SBC A, D
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - registers.d - carry;
            (((registers.a & 0x0F) - (registers.d & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (registers.d + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9B: { // SBC A, E
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - registers.e - carry;
            (((registers.a & 0x0F) - (registers.e & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (registers.e + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9C: { // SBC A, H
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - registers.h - carry;
            (((registers.a & 0x0F) - (registers.h & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (registers.h + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9D: { // SBC A, L
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - registers.l - carry;
            (((registers.a & 0x0F) - (registers.l & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (registers.l + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0x9E: { // SBC A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - memory[hl] - carry;
            (((registers.a & 0x0F) - (memory[hl] & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (memory[hl] + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0x9F: { // SBC A, A
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - registers.a - carry;
            (((registers.a & 0x0F) - (registers.a & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (registers.a + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA0: { // AND A, B
            registers.a &= registers.b;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA1: { // AND A, C
            registers.a &= registers.c;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA2: { // AND A, D
            registers.a &= registers.d;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA3: { // AND A, E
            registers.a &= registers.e;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA4: { // AND A, H
            registers.a &= registers.h;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA5: { // AND A, L
            registers.a &= registers.l;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA6: { // AND A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.a &= memory[hl];
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xA7: { // AND A, A
            registers.a &= registers.a;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA8: { // XOR A, B
            registers.a ^= registers.b;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xA9: { // XOR A, C
            registers.a ^= registers.c;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAA: { // XOR A, D
            registers.a ^= registers.d;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAB: { // XOR A, E
            registers.a ^= registers.e;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAC: { // XOR A, H
            registers.a ^= registers.h;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAD: { // XOR A, L
            registers.a ^= registers.l;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xAE: { // XOR A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.a ^= memory[hl];
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xAF: { // XOR A, A
            registers.a ^= registers.a;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB0: { // OR A, B
            registers.a |= registers.b;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB1: { // OR A, C
            registers.a |= registers.c;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB2: { // OR A, D
            registers.a |= registers.d;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB3: { // OR A, E
            registers.a |= registers.e;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB4: { // OR A, H
            registers.a |= registers.h;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB5: { // OR A, L
            registers.a |= registers.l;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB6: { // OR A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.a |= memory[hl];
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xB7: { // OR A, A
            registers.a |= registers.a;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB8: { // CP A, B
            registers.a == registers.b? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (registers.b & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.b? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xB9: { // CP A, C
            registers.a == registers.c? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (registers.c & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.c? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBA: { // CP A, D
            registers.a == registers.d? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (registers.d & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.d? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBB: { // CP A, E
            registers.a == registers.e? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (registers.e & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.e? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBC: { // CP A, H
            registers.a == registers.h? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (registers.h & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.h? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBD: { // CP A, L
            registers.a == registers.l? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (registers.l & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.l? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xBE: { // CP A, [HL]
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.a == memory[hl]? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (memory[hl] & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < memory[hl]? setCFlag(true) : setCFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xBF: { // CP A, A
            registers.a == registers.a? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (registers.a & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < registers.a? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xC0: { // RET NZ
            if(!(registers.f & (1 << 7))){
                uint8_t low = memory[registers.sp];
                registers.sp++;
                uint8_t high = memory[registers.sp];
                registers.sp++;
                registers.pc = (high << 8) | low;
                m_cycles += 5;
            }
            else{
                registers.pc++;
                m_cycles += 2;
            }
            break;
        }
        case 0xC1: { // POP BC
            uint8_t low = memory[registers.sp];
            registers.sp++;
            uint8_t high = memory[registers.sp];
            registers.sp++;
            registers.b = high;
            registers.c = low;
            registers.pc++;
            m_cycles += 3;
            break;
        }
        case 0xC2: { // JP NZ, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            if(!(registers.f & (1 << 7))){
                uint16_t dest = (high << 8) | low;
                registers.pc = dest;
                m_cycles += 4;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xC3: { // JP a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t dest = (high << 8) | low;
            registers.pc = dest;
            m_cycles += 4;
            break;
        }
        case 0xC4: { // CALL NZ, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t dest = (high << 8) | low;
            if(!(registers.f & (1 << 7))){
                registers.pc++;
                registers.sp--;
                memory[registers.sp] = (registers.pc >> 8) & 0xFF;
                registers.sp--;
                memory[registers.sp] = registers.pc & 0xFF;
                registers.pc = dest;
                m_cycles += 6;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xC5: { // PUSH BC
            registers.sp--;
            memory[registers.sp] = registers.b;
            registers.sp--;
            memory[registers.sp] = registers.c;
            m_cycles += 4;
            registers.pc++;
            break;
        }
        case 0xC6: { // ADD A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            uint16_t result = registers.a + n8;
            ((registers.a & 0x0F) + (n8 & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xC7: { // RST $00
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x00;
            m_cycles += 4;
            break;
        }
        case 0xC8: { // RET Z
            if((registers.f & (1 << 7))){
                uint8_t low = memory[registers.sp];
                registers.sp++;
                uint8_t high = memory[registers.sp];
                registers.sp++;
                registers.pc = (high << 8) | low;
                m_cycles += 5;
            }
            else{
                registers.pc++;
                m_cycles += 2;
            }
            break;
        }
        case 0xC9: { // RET
            uint8_t low = memory[registers.sp];
            registers.sp++;
            uint8_t high = memory[registers.sp];
            registers.sp++;
            registers.pc = (high << 8) | low;
            m_cycles += 4;
            break;
        }
        case 0xCA: { // JP Z, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            if(registers.f & (1 << 7)){
                uint16_t dest = (high << 8) | low;
                registers.pc = dest;
                m_cycles += 4;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xCB: { // PREFIX
            registers.pc++;
            m_cycles++;
            execute_prefix_instruction();
            break;
        }
        case 0xCC: { // CALL Z, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t dest = (high << 8) | low;
            if(registers.f & (1 << 7)){
                registers.pc++;
                registers.sp--;
                memory[registers.sp] = (registers.pc >> 8) & 0xFF;
                registers.sp--;
                memory[registers.sp] = registers.pc & 0xFF;
                registers.pc = dest;
                m_cycles += 6;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xCD: { // CALL a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t dest = (high << 8) | low;
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = dest;
            m_cycles += 6;
            break;
        }
        case 0xCE: { // ADC A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a + n8 + carry;
            ((registers.a & 0x0F) + (n8 & 0x0F) + carry) > 0x0F? setHFlag(true) : setHFlag(false);
            result > 0xFF? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xCF: { // RST $08
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x08;
            m_cycles += 4;
            break;
        }
        case 0xD0: { // RET NC
            if(!(registers.f & (1 << 4))){
                uint8_t low = memory[registers.sp];
                registers.sp++;
                uint8_t high = memory[registers.sp];
                registers.sp++;
                registers.pc = (high << 8) | low;
                m_cycles += 5;
            }
            else{
                registers.pc++;
                m_cycles += 2;
            }
            break;
        }
        case 0xD1: { // POP DE
            uint8_t low = memory[registers.sp];
            registers.sp++;
            uint8_t high = memory[registers.sp];
            registers.sp++;
            registers.d = high;
            registers.e = low;
            registers.pc++;
            m_cycles += 3;
            break;
        }
        case 0xD2: { // JP NC, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            if(!(registers.f & (1 << 4))){
                uint16_t dest = (high << 8) | low;
                registers.pc = dest;
                m_cycles += 4;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xD4: { // CALL NC, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t dest = (high << 8) | low;
            if(!(registers.f & (1 << 4))){
                registers.pc++;
                registers.sp--;
                memory[registers.sp] = (registers.pc >> 8) & 0xFF;
                registers.sp--;
                memory[registers.sp] = registers.pc & 0xFF;
                registers.pc = dest;
                m_cycles += 6;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xD5: { // PUSH DE
            registers.sp--;
            memory[registers.sp] = registers.d;
            registers.sp--;
            memory[registers.sp] = registers.e;
            m_cycles += 4;
            registers.pc++;
            break;
        }
        case 0xD6: { // SUB A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            uint16_t result = registers.a - n8;
            (registers.a & 0x0F) < (n8 & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < n8? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xD7: { // RST $10
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x10;
            m_cycles += 4;
            break;
        }
        case 0xD8: { // RET C
            if(registers.f & (1 << 4)){
                uint8_t low = memory[registers.sp];
                registers.sp++;
                uint8_t high = memory[registers.sp];
                registers.sp++;
                registers.pc = (high << 8) | low;
                m_cycles += 5;
            }
            else{
                registers.pc++;
                m_cycles += 2;
            }
            break;
        }
        case 0xD9: { // RETI
            uint8_t low = memory[registers.sp];
            registers.sp++;
            uint8_t high = memory[registers.sp];
            registers.sp++;
            registers.pc = (high << 8) | low;
            ime_flag = true;
            m_cycles += 4;
            break;
        }
        case 0xDA: { // JP C, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            if(registers.f & (1 << 4)){
                uint16_t dest = (high << 8) | low;
                registers.pc = dest;
                m_cycles += 4;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xDC: { // CALL C, a16
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t dest = (high << 8) | low;
            if(registers.f & (1 << 4)){
                registers.pc++;
                registers.sp--;
                memory[registers.sp] = (registers.pc >> 8) & 0xFF;
                registers.sp--;
                memory[registers.sp] = registers.pc & 0xFF;
                registers.pc = dest;
                m_cycles += 6;
            }
            else{
                registers.pc++;
                m_cycles += 3;
            }
            break;
        }
        case 0xDE: { // SBC A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            uint8_t carry = (registers.f & (1 << 4))? 1 : 0;
            uint16_t result = registers.a - n8 - carry;
            (((registers.a & 0x0F) - (n8 & 0x0F) - carry) & 0x10)? setHFlag(true) : setHFlag(false);
            (registers.a < (n8 + carry))? setCFlag(true) : setCFlag(false);
            registers.a = (uint8_t)result;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xDF: { // RST $18
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x18;
            m_cycles += 4;
            break;
        }
        case 0xE0: { // LDH [a8], A
            registers.pc++;
            uint8_t a8 = rom_data[registers.pc];
            uint16_t dest = 0xFF00 + a8;
            memory[dest] = registers.a;
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xE1: { // POP HL
            uint8_t low = memory[registers.sp];
            registers.sp++;
            uint8_t high = memory[registers.sp];
            registers.sp++;
            registers.h = high;
            registers.l = low;
            registers.pc++;
            m_cycles += 3;
            break;
        }
        case 0xE2: { // LDH [C], A
            uint16_t dest = 0xFF00 + registers.c;
            memory[dest] = registers.a;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xE5: { // PUSH HL
            registers.sp--;
            memory[registers.sp] = registers.h;
            registers.sp--;
            memory[registers.sp] = registers.l;
            m_cycles += 4;
            registers.pc++;
            break;
        }
        case 0xE6: { // AND A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            registers.a &= n8;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(true);
            setCFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xE7: { // RST $20
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x20;
            m_cycles += 4;
            break;
        }
        case 0xE8: { // ADD SP, e8
            registers.pc++;
            int8_t r8 = rom_data[registers.pc];
            ((registers.sp & 0x0F) + (r8 & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            ((registers.sp & 0xFF) + (r8 & 0xFF)) > 0xFF? setCFlag(true) : setCFlag(false);
            setZFlag(false);
            setNFlag(false);
            registers.sp += r8;
            registers.pc++;
            m_cycles += 4;
            break;
        }
        case 0xE9: { // JP HL
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.pc = hl;
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xEA: { // LD [a16], A
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t a16 = (high << 8) | low;
            memory[a16] = registers.a;
            m_cycles += 4;
            registers.pc++;
            break;
        }
        case 0xEE: { // XOR A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            registers.a ^= n8;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xEF: { // RST $28
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x28;
            m_cycles += 4;
            break;
        }
        case 0xF0: { // LDH A, [a8]
            registers.pc++;
            uint8_t a8 = rom_data[registers.pc];
            uint16_t dest = 0xFF00 + a8;
            registers.a = memory[dest];
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xF1: { // POP AF
            uint8_t low = memory[registers.sp];
            registers.sp++;
            uint8_t high = memory[registers.sp];
            registers.sp++;
            registers.a = high;
            registers.f = low & 0xF0;
            registers.pc++;
            m_cycles += 3;
            break;
        }
        case 0xF2: { // LDH A, [C]
            uint16_t dest = 0xFF00 + registers.c;
            registers.a = memory[dest];
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xF3: { // DI
            ime_flag = false;
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xF5: { // PUSH AF
            registers.sp--;
            memory[registers.sp] = registers.a;
            registers.sp--;
            memory[registers.sp] = registers.f;
            m_cycles += 4;
            registers.pc++;
            break;
        }
        case 0xF6: { // OR A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            registers.a |= n8;
            registers.a == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            setHFlag(false);
            setCFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xF7: { // RST $30
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x30;
            m_cycles += 4;
            break;
        }
        case 0xF8: { // LD HL, SP+e8
            registers.pc++;
            int8_t e8 = rom_data[registers.pc];
            uint16_t result = registers.sp + e8;
            setZFlag(false);
            setNFlag(false);
            ((registers.sp & 0x0F) + (e8 & 0x0F)) > 0x0F? setHFlag(true) : setHFlag(false);
            ((registers.sp & 0xFF) + (e8 & 0xFF)) > 0xFF? setCFlag(true) : setCFlag(false);
            registers.h = (result >> 8) & 0xFF;
            registers.l = result & 0xFF;
            m_cycles += 3;
            registers.pc++;
            break;
        }
        case 0xF9: { // LD SP, HL
            uint16_t hl = (registers.h << 8) | registers.l;
            registers.sp = hl;
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xFA: { // LD A, [a16]
            registers.pc++;
            uint8_t low = rom_data[registers.pc];
            registers.pc++;
            uint8_t high = rom_data[registers.pc];
            uint16_t a16 = (high << 8) | low;
            registers.a = memory[a16];
            m_cycles += 4;
            registers.pc++;
            break;
        }
        case 0xFB: { // EI
            ei_flag = true;
            ei = true;
            m_cycles++;
            registers.pc++;
            break;
        }
        case 0xFE: { // CP A, n8
            registers.pc++;
            uint8_t n8 = rom_data[registers.pc];
            registers.a == n8? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            (registers.a & 0x0F) < (n8 & 0x0F)? setHFlag(true) : setHFlag(false);
            registers.a < n8? setCFlag(true) : setCFlag(false);
            m_cycles += 2;
            registers.pc++;
            break;
        }
        case 0xFF: { // RST $38
            registers.pc++;
            registers.sp--;
            memory[registers.sp] = (registers.pc >> 8) & 0xFF;
            registers.sp--;
            memory[registers.sp] = registers.pc & 0xFF;
            registers.pc = 0x38;
            m_cycles += 4;
            break;
        }
    }

    if(ei_flag && !ei){
        ime_flag = true;
        ei_flag = false;
        ei = false;
    }
    else ei = false;
}

void debug_execute(){
    printf("Executing instruction %02x    ", rom_data[registers.pc]);
    execute_instruction();
    printf("AF: %02x%02x    BC: %02x%02x    DE: %02x%02x    HL: %02x%02x    SP: %02x    PC: %02x    Cycles: %d\n",
        registers.a, registers.f,
        registers.b, registers.c,
        registers.d, registers.e,
        registers.h, registers.l,
        registers.sp, registers.pc,
        m_cycles);
}

int main(int argc, char* args[]){
    registers.pc = 0x100;

    ifstream file("./test-cartridges/Tetris.gb", ios::in|ios::binary|ios::ate);
    if(file.is_open()){
        streamsize size = file.tellg();
        rom_data = new uint8_t[size];
        memory = new uint8_t[0x10000];
        file.seekg(0, ios::beg);
        if (file.read(reinterpret_cast<char*>(rom_data), size)) {
            cout << "File read successfully." << '\n';
        } else {
            cerr << "Error reading file." << '\n';
        }
        file.close();

        PPU *ppu = new PPU();
        ppu->memory = memory;
        ppu->cycles = 0;
        ppu->line = 0;
        ppu->mode = OAM_SCAN;


        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Window *window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH * 4, SCREEN_HEIGHT*4, SDL_WINDOW_ALLOW_HIGHDPI);
        if(window == NULL){
            cerr << "Could not create window: " << SDL_GetError() << '\n';
            return 1;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(renderer == NULL){
            cerr << "Could not create renderer: " << SDL_GetError() << '\n';
            return 1;
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 
            SCREEN_WIDTH, SCREEN_HEIGHT);
        if(texture == NULL){
            cerr << "Could not create texture: " << SDL_GetError() << '\n';
            return 1;
        }

        SDL_Event windowEvent;
        while(true){
            if(SDL_PollEvent(&windowEvent)){
                if(windowEvent.type == SDL_QUIT) break;
            }

            int initial_m_cycles = m_cycles;
            if(ime_flag){
                uint8_t if_reg = memory[0xFF0F];
                uint8_t ie_reg = memory[0xFFFF];
                uint8_t fired_interrupts = if_reg & ie_reg;

                if(fired_interrupts & 0x01){ //VBlank
                    ime_flag = false;
                    memory[0xFF0F] &= ~0x01;
                    registers.sp -= 2;
                    memory[registers.sp] = (registers.pc >> 8) & 0xFF;
                    memory[registers.sp - 1] = registers.pc & 0xFF;
                    registers.pc = 0x0040;
                    m_cycles += 5;
                }
                // other interrupts
            }

            execute_instruction();
            // debug_execute();
            ppu_step(ppu, m_cycles - initial_m_cycles, renderer, texture);
        }

        SDL_DestroyWindow(window);
        SDL_Quit();

        delete[] rom_data;
    }

    return 0;
}