#pragma once
#include <cstdint>

// Shared state and prototypes across translation units. Definitions live in the
// module named in each section's comment.

// --- CPU (cpu.cpp) ---
struct Registers
{
    uint8_t a, f, b, c, d, e, h, l;
    uint16_t pc, sp;
};
extern Registers registers;
extern int m_cycles;
extern bool stopped, halted, ime_flag, ei_flag, ei;

void execute_instruction();
void execute_prefix_instruction();
void handle_interrupts();
void set_post_boot_state();
void debug_execute();

// --- Memory bus / cartridge (memory.cpp) ---
extern uint8_t *memory;   // 0x8000-0xFFFF working memory
extern uint8_t *rom_data; // full cartridge ROM (all banks)
extern uint32_t rom_size;
extern uint8_t ext_ram[0x8000]; // external cartridge RAM
extern int mbc_type, rom_bank, ram_bank;
extern bool ram_enabled;
extern uint8_t joypad_buttons;

uint8_t read8(uint16_t addr);
void write8(uint16_t addr, uint8_t val);

// --- Timer (timer.cpp) ---
extern uint16_t div_counter;
extern int tima_counter;
void timer_step(int m_cycles_elapsed);
