#include "emulator.hpp"
#include <cstdio>

// Cartridge / MBC state. memory[] holds 0x8000-0xFFFF; cartridge ROM lives in
// rom_data (all banks) and external cart RAM in ext_ram, both routed by the MBC.
uint8_t *memory;
uint8_t *rom_data = nullptr;
uint32_t rom_size = 0;
uint8_t ext_ram[0x8000] = {0};
int mbc_type = 0;
int rom_bank = 1;
int ram_bank = 0;
bool ram_enabled = false;

// Joypad: 1 = released, 0 = pressed. Low nibble = action, high nibble = d-pad.
uint8_t joypad_buttons = 0xFF;

// --- Memory bus -----------------------------------------------------------
uint8_t read8(uint16_t addr)
{
#ifndef SST_TEST
    if (addr < 0x4000)
        return rom_data[addr]; // ROM bank 0 (fixed)
    if (addr < 0x8000)         // ROM bank N (switchable)
    {
        uint32_t off = (uint32_t)rom_bank * 0x4000 + (addr - 0x4000);
        return (off < rom_size) ? rom_data[off] : 0xFF;
    }
    if (addr >= 0xA000 && addr < 0xC000) // external cartridge RAM
    {
        if (!ram_enabled)
            return 0xFF;
        return ext_ram[(ram_bank * 0x2000 + (addr - 0xA000)) & 0x7FFF];
    }
    if (addr == 0xFF00)
    {
        // Bits 4/5 select the button group (0 = selected); return its state in bits 0-3 (0 = pressed).
        uint8_t select = memory[0xFF00] & 0x30;
        uint8_t buttons = 0x0F; // default: nothing pressed
        if (!(select & 0x10)) // d-pad selected
            buttons &= (joypad_buttons >> 4) & 0x0F;
        if (!(select & 0x20)) // action buttons selected
            buttons &= joypad_buttons & 0x0F;
        return 0xC0 | select | buttons; // bits 6-7 always read 1
    }
#endif
    return memory[addr];
}

void write8(uint16_t addr, uint8_t val)
{
#ifndef SST_TEST
    if (addr < 0x8000)
    {
        // Writes to the ROM area are MBC control registers.
        if (mbc_type == 0)
            return; // ROM only: ignore
        if (addr < 0x2000)
            ram_enabled = ((val & 0x0F) == 0x0A);
        else if (addr < 0x4000)
        {
            if (mbc_type == 3)
                rom_bank = (val & 0x7F) ? (val & 0x7F) : 1;
            else // MBC1: lower 5 bits, bank 0 aliases to 1
            {
                rom_bank = (rom_bank & 0x60) | (val & 0x1F);
                if ((val & 0x1F) == 0)
                    rom_bank |= 1;
            }
        }
        else if (addr < 0x6000)
            ram_bank = val & 0x03; // RAM bank (MBC3 has no RTC on type 0x13)
        // 0x6000-0x7FFF: MBC1 mode / MBC3 RTC latch -- unused here
        return;
    }

    if (addr >= 0xA000 && addr < 0xC000) // external cartridge RAM
    {
        if (ram_enabled)
            ext_ram[(ram_bank * 0x2000 + (addr - 0xA000)) & 0x7FFF] = val;
        return;
    }

    // DIV (0xFF04): any CPU write resets the whole internal counter to 0.
    if (addr == 0xFF04)
    {
        div_counter = 0;
        memory[0xFF04] = 0;
        return;
    }
#endif

    memory[addr] = val;

#ifndef SST_TEST
    // OAM DMA (0xFF46): copy 0xXX00-0xXX9F into OAM (0xFE00-0xFE9F). Instant (HW takes 160 cycles).
    if (addr == 0xFF46)
    {
        uint16_t src = val << 8;
        for (int i = 0; i < 0xA0; i++)
            memory[0xFE00 + i] = read8(src + i); // source may be banked ROM/RAM
    }

    if (addr == 0xFF02 && (val & 0x81) == 0x81)
    {
        putchar(memory[0xFF01]);
        fflush(stdout);
        memory[0xFF01] = 0xFF;    // no link cable connected -> receive 0xFF
        memory[0xFF02] = val & ~0x80; // transfer complete (clear start bit)
        memory[0xFF0F] |= 0x08;   // request Serial interrupt
    }
#endif
}
