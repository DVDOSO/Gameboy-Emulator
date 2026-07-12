#include <iostream>
#include <stdio.h>
#include <fstream>
#include <array>
#include <SDL2/SDL.h>

using namespace std;

enum PPUMode{
    OAM_SCAN = 2,
    DRAWING_PIXELS = 3,
    H_BLANK = 0,
    V_BLANK = 1
};

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;
const int TILE_SIZE = 8;

struct PPU{
    uint8_t *memory;
    int cycles;
    int line;
    PPUMode mode = OAM_SCAN;
    array<array<uint32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> frame_buffer;
};

const array<uint32_t, 4> GB_COLOR_PALETTE = {
    0xFFFFFFFF,
    0xFFAAAAAA,
    0xFF555555,
    0xFF000000
};

void requestInterrupt(PPU *ppu, uint8_t interrupt_flag){
    ppu->memory[0xFF0F] |= interrupt_flag;
}

// Raw BG/window colour index (0-3, pre-palette) for the scanline currently being
// drawn. Sprite rendering reads it to resolve the BG-over-OBJ priority bit.
static uint8_t bg_color_index_line[SCREEN_WIDTH];

// The window has its own line counter that only advances on lines where the window
// is actually rendered (not the same as LY). Reset to 0 at the top of each frame.
int window_line_counter = 0;

void drawScanline(PPU *ppu){
    uint8_t lcdc = ppu->memory[0xFF40];
    uint8_t scy = ppu->memory[0xFF42];
    uint8_t scx = ppu->memory[0xFF43];
    uint8_t bgp = ppu->memory[0xFF47];

    bool bg_enabled = (lcdc & 0x01);
    if(!bg_enabled){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            (ppu->frame_buffer)[ppu->line][x] = GB_COLOR_PALETTE[0];
            bg_color_index_line[x] = 0;
        }
        return;
    }

    uint16_t bg_map_addr = (lcdc & 0x08) ? 0x9C00 : 0x9800;
    uint16_t tile_data_addr = (lcdc & 0x10) ? 0x8000 : 0x8800;
    bool signed_tile_addressing = !(lcdc & 0x10);

    uint8_t bg_y = (ppu->line + scy) & 0xFF;
    uint8_t tile_y_in_row = bg_y % 8;
    uint8_t tile_row_in_map = bg_y / 8;

    for(int x = 0; x < SCREEN_WIDTH; x++){
        uint8_t bg_x = (x + scx) & 0xFF;
        uint8_t tile_x_in_row = bg_x % 8;
        uint8_t tile_col_in_map = bg_x / 8;
        uint16_t tile_map_index = (tile_row_in_map * 32) + tile_col_in_map;
        uint8_t tile_id = ppu->memory[bg_map_addr + tile_map_index];

        uint16_t current_tile_data_base_addr;
        if(signed_tile_addressing){
            current_tile_data_base_addr = tile_data_addr + (int8_t)tile_id * 16;
        }
        else{
            current_tile_data_base_addr = tile_data_addr + tile_id * 16;
        }

        uint16_t tile_row_data_addr = current_tile_data_base_addr + (tile_y_in_row * 2);

        uint8_t byte1 = ppu->memory[tile_row_data_addr];
        uint8_t byte2 = ppu->memory[tile_row_data_addr + 1];

        int bit_pos = 7 - tile_x_in_row;
        uint8_t color_index = 0;
        color_index |= ((byte1 >> bit_pos) & 0x01);
        color_index |= (((byte2 >> bit_pos) & 0x01) << 1);

        uint8_t final_gb_color_index = (bgp >> (color_index * 2)) & 0x03;
        uint32_t final_pixel_color = GB_COLOR_PALETTE[final_gb_color_index];

        (ppu->frame_buffer)[ppu->line][x] = final_pixel_color;
        bg_color_index_line[x] = color_index;
    }

    // --- Window layer ---
    // Drawn over the BG when enabled (LCDC.5), once LY has reached WY. The window's
    // top-left pixel maps to screen (WX-7, WY). It uses its own line counter that only
    // advances on lines where the window is drawn.
    uint8_t wy = ppu->memory[0xFF4A];
    uint8_t wx = ppu->memory[0xFF4B];
    if((lcdc & 0x20) && ppu->line >= wy && wx <= 166){
        uint16_t win_map_addr = (lcdc & 0x40) ? 0x9C00 : 0x9800;
        uint8_t win_tile_y = window_line_counter % 8;
        uint8_t win_tile_row = window_line_counter / 8;
        int win_start = (int)wx - 7;

        for(int x = (win_start < 0 ? 0 : win_start); x < SCREEN_WIDTH; x++){
            int win_x = x - win_start;
            uint8_t win_tile_col = win_x / 8;
            uint8_t win_pix = win_x % 8;
            uint16_t map_idx = (win_tile_row * 32) + win_tile_col;
            uint8_t tile_id = ppu->memory[win_map_addr + map_idx];

            uint16_t base = signed_tile_addressing
                ? tile_data_addr + (int8_t)tile_id * 16
                : tile_data_addr + tile_id * 16;
            uint16_t row_addr = base + win_tile_y * 2;
            uint8_t b1 = ppu->memory[row_addr];
            uint8_t b2 = ppu->memory[row_addr + 1];

            int bp = 7 - win_pix;
            uint8_t ci = ((b1 >> bp) & 1) | (((b2 >> bp) & 1) << 1);
            uint8_t shade = (bgp >> (ci * 2)) & 0x03;
            (ppu->frame_buffer)[ppu->line][x] = GB_COLOR_PALETTE[shade];
            bg_color_index_line[x] = ci; // window counts as BG for OBJ priority
        }
        window_line_counter++;
    }
}

// Draw sprites (OBJ) over the background for the current scanline.
// Handles 8x8/8x16, X/Y flip, OBP0/OBP1 palettes, BG-over-OBJ priority, the 10-per-
// line limit, and DMG X-coordinate priority.
void drawSprites(PPU *ppu){
    uint8_t lcdc = ppu->memory[0xFF40];
    if(!(lcdc & 0x02)) return; // OBJ disabled

    int height = (lcdc & 0x04) ? 16 : 8;
    int line = ppu->line;

    // Gather up to 10 sprites covering this scanline, in OAM order.
    int idx[10], count = 0;
    for(int i = 0; i < 40 && count < 10; i++){
        int sy = (int)ppu->memory[0xFE00 + i*4] - 16;
        if(line >= sy && line < sy + height) idx[count++] = i;
    }

    // DMG priority: lower X wins; ties broken by lower OAM index. Draw lowest priority
    // first so the highest-priority sprite ends up on top. Sort indices so the array is
    // ordered highest-priority-first, then iterate in reverse when drawing.
    for(int a = 0; a < count; a++)
        for(int b = a+1; b < count; b++){
            int xa = ppu->memory[0xFE00 + idx[a]*4 + 1];
            int xb = ppu->memory[0xFE00 + idx[b]*4 + 1];
            if(xb < xa || (xb == xa && idx[b] < idx[a])){ int t = idx[a]; idx[a] = idx[b]; idx[b] = t; }
        }

    for(int s = count - 1; s >= 0; s--){
        int i = idx[s];
        int sy   = (int)ppu->memory[0xFE00 + i*4] - 16;
        int sx   = (int)ppu->memory[0xFE00 + i*4 + 1] - 8;
        uint8_t tile = ppu->memory[0xFE00 + i*4 + 2];
        uint8_t attr = ppu->memory[0xFE00 + i*4 + 3];

        bool flip_y   = attr & 0x40;
        bool flip_x   = attr & 0x20;
        bool bg_over  = attr & 0x80;
        uint8_t obp   = (attr & 0x10) ? ppu->memory[0xFF49] : ppu->memory[0xFF48];

        int row = line - sy;
        if(flip_y) row = (height - 1) - row;

        if(height == 16) tile &= 0xFE; // 8x16: LSB ignored, low tile = top
        uint16_t tile_addr = 0x8000 + tile * 16 + row * 2; // OBJ always uses 0x8000 base
        uint8_t byte1 = ppu->memory[tile_addr];
        uint8_t byte2 = ppu->memory[tile_addr + 1];

        for(int px = 0; px < 8; px++){
            int screen_x = sx + px;
            if(screen_x < 0 || screen_x >= SCREEN_WIDTH) continue;

            int bit = flip_x ? px : (7 - px);
            uint8_t color_index = ((byte1 >> bit) & 1) | (((byte2 >> bit) & 1) << 1);
            if(color_index == 0) continue; // transparent

            if(bg_over && bg_color_index_line[screen_x] != 0) continue; // BG has priority

            uint8_t shade = (obp >> (color_index * 2)) & 0x03;
            (ppu->frame_buffer)[line][screen_x] = GB_COLOR_PALETTE[shade];
        }
    }

}

// Tracks the STAT interrupt "line"; a STAT interrupt fires only on its rising edge.
static bool stat_irq_line = false;

// Refresh the STAT register (0xFF41): mode bits (0-1), LY==LYC coincidence (bit 2),
// and request a STAT interrupt (IF bit 1) when an enabled source rises.
void update_stat(PPU *ppu){
    uint8_t stat = ppu->memory[0xFF41];
    uint8_t ly = ppu->memory[0xFF44];
    uint8_t lyc = ppu->memory[0xFF45];

    bool coincidence = (ly == lyc);

    stat = (stat & ~0x07) | (ppu->mode & 0x03) | (coincidence ? 0x04 : 0x00);
    stat |= 0x80; // bit 7 is always set
    ppu->memory[0xFF41] = stat;

    bool line =
        ((ppu->mode == H_BLANK) && (stat & 0x08)) ||
        ((ppu->mode == V_BLANK) && (stat & 0x10)) ||
        ((ppu->mode == OAM_SCAN) && (stat & 0x20)) ||
        (coincidence && (stat & 0x40));

    if(line && !stat_irq_line){
        requestInterrupt(ppu, 0x02);
    }
    stat_irq_line = line;
}

void debugDraw(PPU *ppu, SDL_Renderer *renderer, SDL_Texture *texture){
    SDL_UpdateTexture(texture, NULL, (ppu->frame_buffer).data(), SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ppu_step(PPU *ppu, int cpu_cycles, SDL_Renderer *renderer, SDL_Texture *texture){
    ppu->cycles += cpu_cycles;

    uint8_t lcdc = ppu->memory[0xFF40];
    if((lcdc & 0x80) == 0){
        // LCD off: PPU resets to line 0, mode 0; STAT mode bits read 0.
        ppu->line = 0;
        ppu->cycles = 0;
        ppu->mode = H_BLANK;
        ppu->memory[0xFF44] = 0;
        ppu->memory[0xFF41] = (ppu->memory[0xFF41] & ~0x03) | 0x80;
        stat_irq_line = false;
        window_line_counter = 0;
        return;
    }

    switch(ppu->mode){
        case OAM_SCAN:
            if(ppu->cycles >= 20){
                ppu->cycles -= 20;
                ppu->mode = DRAWING_PIXELS;
            }
            break;

        case DRAWING_PIXELS:
            if(ppu->cycles >= 43){
                ppu->cycles -= 43;
                ppu->mode = H_BLANK;
                drawScanline(ppu);
                drawSprites(ppu);
            }
            break;

        case H_BLANK:
            if(ppu->cycles >= 51){
                ppu->cycles -= 51;
                ppu->line++;
                ppu->memory[0xFF44] = ppu->line;

                if(ppu->line == 144){
                    ppu->mode = V_BLANK;
                    requestInterrupt(ppu, 0x01);
                }
                else{
                    ppu->mode = OAM_SCAN;
                }
            }
            break;

        case V_BLANK:
            if(ppu->cycles >= 114){
                ppu->cycles -= 114;
                ppu->line++;
                ppu->memory[0xFF44] = ppu->line;

                if(ppu->line > 153){
                    debugDraw(ppu, renderer, texture);
                    ppu->line = 0;
                    ppu->memory[0xFF44] = 0;
                    ppu->mode = OAM_SCAN;
                    window_line_counter = 0; // new frame: reset window line counter
                }
            }
            break;
    }

    update_stat(ppu);
}