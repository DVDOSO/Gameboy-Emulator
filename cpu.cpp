#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

char rom_data[10] = {0x04};
char memory[1000];
int m_cycles = 0;

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
        case 0x00: {
            registers.pc++;
            m_cycles++;
            return;
        }
        case 0x01: {
            registers.pc++;
            registers.b = rom_data[registers.pc];
            registers.pc++;
            registers.c = rom_data[registers.pc];
            registers.pc++;
            m_cycles += 3;
            return;
        }
        case 0x02: {
            uint16_t bc = (registers.b << 8) | registers.c;
            memory[bc] = registers.a;
            registers.pc++;
            m_cycles += 2;
            return;
        }
        case 0x03: {
            uint16_t bc = (registers.b << 8) | registers.c;
            bc++;
            registers.b = (bc >> 8) & 0xFF;
            registers.c = bc & 0xFF;
            registers.pc++;
            m_cycles += 2;
        }
        case 0x04: {
            (registers.b & 0xF) == 0xF? setHFlag(true) : setHFlag(false);
            registers.b++;
            registers.b == 0? setZFlag(true) : setZFlag(false);
            setNFlag(false);
            registers.pc++;
            m_cycles += 4;
        }
    }
}

int main(){ //for testing
    registers.pc = 0x00; //supposed to be 0x100
    registers.b = 0xFF;

    printf("--------------REGISTERS--------------\n");
    printf("a\tf\tb\tc\td\te\th\tl\tsp\tpc\n");
    printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t\n",
        registers.a, registers.f,
        registers.b, registers.c,
        registers.d, registers.e,
        registers.h, registers.l,
        registers.sp, registers.pc);
    execute_instruction();
    printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t\n",
        registers.a, registers.f,
        registers.b, registers.c,
        registers.d, registers.e,
        registers.h, registers.l,
        registers.sp, registers.pc);
    printf("-------------------------------------\n");

    return 0;
}