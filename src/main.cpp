#include "emulator.hpp"
#include "ppu.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *args[])
{
    registers.pc = 0x100;

    // ROM path: first CLI argument if given, else default to Tetris.
    const char *rom_path = (argc > 1) ? args[1] : "./test-cartridges/Tetris (World) (Rev 1).gb";
    cout << "Loading ROM: " << rom_path << '\n';

    ifstream file(rom_path, ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        streamsize size = file.tellg();
        rom_size = (uint32_t)size;
        rom_data = new uint8_t[rom_size](); // full cartridge ROM (all banks)
        memory = new uint8_t[0x10000]();     // 0x8000-0xFFFF working memory
        file.seekg(0, ios::beg);
        if (file.read(reinterpret_cast<char *>(rom_data), size))
            cout << "File read successfully." << '\n';
        else
            cerr << "Error reading file." << '\n';
        file.close();

        // Pick the MBC from the cartridge type byte (0x0147).
        uint8_t cart_type = rom_data[0x0147];
        if (cart_type == 0x00)
            mbc_type = 0; // ROM only
        else if (cart_type <= 0x03)
            mbc_type = 1; // MBC1 family
        else if (cart_type >= 0x0F && cart_type <= 0x13)
            mbc_type = 3; // MBC3 family
        else
            mbc_type = 1; // best-effort fallback

        printf("Title: %.15s\n", rom_data + 0x0134);
        printf("Cart type 0x%02X -> MBC%d, ROM %u KiB, logo %s\n",
               cart_type, mbc_type, rom_size / 1024,
               (rom_data[0x0104] == 0xCE) ? "OK" : "MISMATCH");
        fflush(stdout);

        set_post_boot_state(); // emulate the boot ROM handoff (registers + IO regs)

        PPU *ppu = new PPU();
        ppu->memory = memory;
        ppu->cycles = 0;
        ppu->line = 0;
        ppu->mode = OAM_SCAN;

        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Window *window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                              SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, SDL_WINDOW_ALLOW_HIGHDPI);
        if (window == NULL)
        {
            cerr << "Could not create window: " << SDL_GetError() << '\n';
            return 1;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
        {
            cerr << "Could not create renderer: " << SDL_GetError() << '\n';
            return 1;
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                 SCREEN_WIDTH, SCREEN_HEIGHT);
        if (texture == NULL)
        {
            cerr << "Could not create texture: " << SDL_GetError() << '\n';
            return 1;
        }

        // Keyboard -> button bit: arrows = d-pad, Z=A, X=B, Enter=Start, Right-Shift=Select.
        auto keyToBit = [](SDL_Keycode k) -> int {
            switch (k)
            {
            case SDLK_z: return 0;      // A
            case SDLK_x: return 1;      // B
            case SDLK_RSHIFT: return 2; // Select
            case SDLK_RETURN: return 3; // Start
            case SDLK_RIGHT: return 4;
            case SDLK_LEFT: return 5;
            case SDLK_UP: return 6;
            case SDLK_DOWN: return 7;
            default: return -1;
            }
        };

        // One DMG frame = 17556 M-cycles (~59.7 Hz). Poll input and pace once per frame.
        const int CYCLES_PER_FRAME = 17556;
        const double FRAME_MS = 1000.0 / 59.73;

        bool quit = false;
        SDL_Event windowEvent;
        while (!quit)
        {
            uint64_t frame_start = SDL_GetPerformanceCounter();

            while (SDL_PollEvent(&windowEvent))
            {
                if (windowEvent.type == SDL_QUIT)
                    quit = true;
                else if (windowEvent.type == SDL_KEYDOWN && !windowEvent.key.repeat)
                {
                    int bit = keyToBit(windowEvent.key.keysym.sym);
                    if (bit >= 0)
                    {
                        joypad_buttons &= ~(1 << bit); // 0 = pressed
                        memory[0xFF0F] |= 0x10;        // request Joypad interrupt
                    }
                }
                else if (windowEvent.type == SDL_KEYUP)
                {
                    int bit = keyToBit(windowEvent.key.keysym.sym);
                    if (bit >= 0)
                        joypad_buttons |= (1 << bit); // 1 = released
                }
            }
            if (quit)
                break;

            // Emulate one frame's worth of cycles.
            int frame_cycles = 0;
            while (frame_cycles < CYCLES_PER_FRAME)
            {
                int before = m_cycles;

                handle_interrupts();

                if (halted)
                    m_cycles += 1; // idle a cycle so the PPU/timer can advance and wake us
                else
                    execute_instruction();

                int elapsed = m_cycles - before;
                timer_step(elapsed);
                ppu_step(ppu, elapsed, renderer, texture);
                frame_cycles += elapsed;
            }

            // Pace to ~59.7 fps: SDL_Delay for the bulk, then a short busy-wait for precision.
            double freq = (double)SDL_GetPerformanceFrequency();
            double emu_ms = (double)(SDL_GetPerformanceCounter() - frame_start) * 1000.0 / freq;
            if (emu_ms < FRAME_MS - 2.0)
                SDL_Delay((Uint32)(FRAME_MS - emu_ms - 2.0));
            while ((double)(SDL_GetPerformanceCounter() - frame_start) * 1000.0 / freq < FRAME_MS)
                ; // spin the last bit
        }

        SDL_DestroyWindow(window);
        SDL_Quit();

        delete[] memory;
        delete[] rom_data;
    }
    else
    {
        cerr << "Unable to open ROM: " << rom_path << '\n';
        return 1;
    }

    return 0;
}
