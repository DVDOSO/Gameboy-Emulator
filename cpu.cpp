#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

uint8_t rom_data[10] = {0x17};
uint8_t memory[1000];
int m_cycles = 0;
bool stopped = false;

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

void execute_instruction(){
    switch(rom_data[registers.pc]){
        case 0x00: { // NOP
            m_cycles++;
            registers.pc++;
        }
        case 0x01: { // LD BC, n16
            registers.pc++;
            registers.b = rom_data[registers.pc];
            registers.pc++;
            registers.c = rom_data[registers.pc];
            m_cycles += 3;
            registers.pc++;
        }
        case 0x02: { // LD [BC], A
            uint16_t bc = (registers.b << 8) | registers.c;
            memory[bc] = registers.a;
            m_cycles += 2;
            registers.pc++;
        }
        case 0x03: { // INC BC
            uint16_t bc = (registers.b << 8) | registers.c;
            bc++;
            registers.b = (bc >> 8) & 0xFF;
            registers.c = bc & 0xFF;
            m_cycles += 2;
            registers.pc++;
        }
        case 0x04: { // INC B
            (registers.b & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.b++;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x05: { // DEC B
            (registers.b & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.b--;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x06: { // LD B, n8
            registers.pc++;
            registers.b = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
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
        }
        case 0x0A: { // LD A, [BC]
            uint16_t bc = (registers.b << 8) | registers.c;
            registers.a = memory[bc];
            m_cycles += 2;
            registers.pc++;
        }
        case 0x0B: { //DEC BC
            uint16_t bc = (registers.b << 8) | registers.c;
            bc--;
            registers.b = (bc >> 8) & 0xFF;
            registers.c = bc & 0xFF;
            m_cycles += 2;
            registers.pc++;
        }
        case 0x0C: { // INC C
            (registers.c & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.c++;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x0D: { //DEC C
            (registers.c & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.c--;
            registers.c == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x0E: { // LD C, n8
            registers.pc++;
            registers.c = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
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
        }
        case 0x10: { // STOP n8
            registers.pc++;
            stopped = true;
            m_cycles += 1;
            registers.pc++;
        }
        case 0x11: { // LD DE, n16
            registers.pc++;
            registers.d = rom_data[registers.pc];
            registers.pc++;
            registers.e = rom_data[registers.pc];
            m_cycles += 3;
            registers.pc++;
        }
        case 0x12: { // LD [DE], A
            uint16_t de = (registers.d << 8) | registers.e;
            memory[de] = registers.a;
            m_cycles += 2;
            registers.pc++;
        }
        case 0x13: { // INC DE
            uint16_t de = (registers.d << 8) | registers.e;
            de++;
            registers.d = (de >> 8) & 0xFF;
            registers.e = de & 0xFF;
            m_cycles += 2;
            registers.pc++;
        }
        case 0x14: { // INC D
            (registers.d & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.d++;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x15: { // DEC D
            (registers.d & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.d--;
            registers.d == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x16: { // LD D, n8
            registers.pc++;
            registers.d = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
        }
        case 0x17: { // RLA
            uint8_t bit7 = (1 << 7) & registers.a;
            registers.a = (registers.a << 1) | ((registers.f >> 4) & 1);
            bit7? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
        }
        case 0x18: { // JR e8
            registers.pc++;
            int8_t offset = rom_data[registers.pc];
            registers.pc++;
            registers.pc += offset;
            m_cycles += 3;
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
        }
        case 0x1A: { // LD A, [DE]
            uint16_t de = (registers.d << 8) | registers.e;
            registers.a = memory[de];
            m_cycles += 2;
            registers.pc++;
        }
        case 0x1B: { // DEC DE
            uint16_t de = (registers.d << 8) | registers.e;
            de--;
            registers.d = (de >> 8) & 0xFF;
            registers.e = de & 0xFF;
            m_cycles += 2;
            registers.pc++;
        }
        case 0x1C: { // INC E
            (registers.e & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.e++;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x1D: { // DEC E
            (registers.e & 0x0F) == 0x00? setHFlag(true) : setHFlag(false);
            registers.e--;
            registers.e == 0? setZFlag(true) : setZFlag(false);
            setNFlag(true);
            m_cycles += 1;
            registers.pc++;
        }
        case 0x1E: { // LD E, n8
            registers.pc++;
            registers.e = rom_data[registers.pc];
            m_cycles += 2;
            registers.pc++;
        }
        case 0x1F: { // RRA
            uint8_t bit0 = registers.a & 1;
            registers.a = (registers.a >> 1) | (((registers.f >> 4) & 1) << 7);
            bit0? setCFlag(true) : setCFlag(false);
            m_cycles++;
            registers.pc++;
        }
    }
}

int main(){ //for testing
    registers.pc = 0x00; //supposed to be 0x100
    registers.a = 0xBB;
    registers.f = 0;

    printf("--------------REGISTERS--------------\n");
    printf("a\tf\tb\tc\td\te\th\tl\tsp\tpc\tm_cycles\n");
    printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%d\n",
        registers.a, registers.f,
        registers.b, registers.c,
        registers.d, registers.e,
        registers.h, registers.l,
        registers.sp, registers.pc,
        m_cycles);
    execute_instruction();
    printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%d\n",
        registers.a, registers.f,
        registers.b, registers.c,
        registers.d, registers.e,
        registers.h, registers.l,
        registers.sp, registers.pc,
        m_cycles);
    printf("-------------------------------------\n");

    return 0;
}