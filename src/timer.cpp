#include "emulator.hpp"

// Free-running 16-bit counter whose high byte is exposed as DIV (0xFF04).
uint16_t div_counter = 0;
int tima_counter = 0;

void timer_step(int m_cycles_elapsed)
{
    int t = m_cycles_elapsed * 4;

    div_counter += t;
    memory[0xFF04] = (div_counter >> 8) & 0xFF;

    uint8_t tac = memory[0xFF07];
    if (tac & 0x04) // timer enabled
    {
        static const int periods[4] = {1024, 16, 64, 256}; // T-cycles per TIMA tick
        int period = periods[tac & 0x03];
        tima_counter += t;
        while (tima_counter >= period)
        {
            tima_counter -= period;
            if (memory[0xFF05] == 0xFF)
            {
                memory[0xFF05] = memory[0xFF06]; // reload TIMA from TMA
                memory[0xFF0F] |= 0x04;          // request Timer interrupt
            }
            else
            {
                memory[0xFF05]++;
            }
        }
    }
}
