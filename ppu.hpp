#include <array>
#include <SDL2/SDL.h>

using namespace std;

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

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
    array<array<uint32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> frame_buffer;
};

void ppu_step(PPU *ppu, int cpu_cycles, SDL_Renderer *renderer, SDL_Texture *texture);