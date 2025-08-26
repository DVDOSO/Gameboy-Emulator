#include <iostream>
#include <stdio.h>
#include <fstream>
#include <array>

using namespace std;

enum PPUMode{
    OAM_SCAN = 2,
    DRAWING_PIXELS = 3,
    H_BLANK = 0,
    V_BLANK = 1
};

struct PPU{
    uint8_t *memory;
    int cycles;
    int line;
    PPUMode mode = OAM_SCAN;
};

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

array<array<uint32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> frame_buffer;

const array<uint32_t, 4> GB_COLOR_PALETTE = {
    0xFFFFFFFF,
    0xFFAAAAAA,
    0xFF555555,
    0xFF000000
};

void requestInterrupt(PPU *ppu, uint8_t interrupt_flag){
    ppu->memory[0xFF0F] |= interrupt_flag;
}

void drawScanline(PPU *ppu){
    uint8_t lcdc = ppu->memory[0xFF40];
    uint8_t scy = ppu->memory[0xFF42];
    uint8_t scx = ppu->memory[0xFF43];
    uint8_t bgp = ppu->memory[0xFF47];

    bool bg_enabled = (lcdc & 0x01);
    if(!bg_enabled){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            frame_buffer[ppu->line][x] = GB_COLOR_PALETTE[0];
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

        frame_buffer[ppu->line][x] = final_pixel_color;
    }
}

void debugDraw(){
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            if(frame_buffer[x][y] == 0xFFFFFFFF) printf(".");
            else if(frame_buffer[x][y] == 0xFFAAAAAA) printf("|");
            else if(frame_buffer[x][y] == 0xFF555555) printf("/");
            else if(frame_buffer[x][y] == 0xFF000000) printf("X");
        }
        printf("\n");
    }
    printf("-----------------------------------------------------\n");
}

void ppu_step(PPU *ppu, int cpu_cycles){
    ppu->cycles += cpu_cycles;

    uint8_t lcdc = ppu->memory[0xFF40];
    if((lcdc & 0x80) == 0){
        ppu->line = 0;
        ppu->cycles = 0;
        ppu->memory[0xFF44] = 0;
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
                    ppu->line = 0;
                    ppu->memory[0xFF44] = 0;
                    ppu->mode = OAM_SCAN;
                }
            }
            break;
    }
}