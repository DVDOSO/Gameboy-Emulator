enum PPUMode{
    OAM_SCAN = 2,
    DRAWING_PIXELS = 3,
    H_BLANK = 0,
    V_BLANK = 1
};

struct PPU{
    uint8_t* memory;
    int cycles = 0;
    int line = 0;
    PPUMode mode = OAM_SCAN;
};

void ppu_step(PPU *ppu, int cpu_cycles);