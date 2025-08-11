#include <iostream>
#include <fstream>
#include <string>
#include "constants.hpp"

using namespace std;

struct rom_header{
    uint8_t entry[0x4]; //0100-0103
    uint8_t logo[0x30]; //0104-0133
    char title[0x10]; //0134-0143
    uint8_t new_lic_code[0x2]; //0144-0145
    uint8_t sgb_flag; //0146
    uint8_t type; //0147
    uint8_t rom_size; //0148
    uint8_t ram_size; //0149
    uint8_t dest_code; //014A
    uint8_t old_lic_code; //014B
    uint8_t mask_rom_version; //014C
    uint8_t header_check_sum; //014D
    uint16_t global_check_sum; //014E-014F
};

struct{
    string filename;
    uint32_t rom_size;
    char *rom_data;
    rom_header *header;
} cartridge;

int main(){
    cartridge.filename = "Pokemon Red.gb";

    ifstream file(cartridge.filename, ios::in|ios::binary|ios::ate);
    if(file.is_open()){
        cartridge.rom_size = file.tellg();
        cartridge.rom_data = new char[cartridge.rom_size];
        file.seekg(0, ios::beg);
        file.read(cartridge.rom_data, cartridge.rom_size);
        file.close();

        cartridge.header = (rom_header*)(cartridge.rom_data + 0x100);

        cout << "---Cartridge Loaded---\n";
        cout << "Title:\t\t" << cartridge.header->title << '\n';
        cout << "Type:\t\t" << rom_types(cartridge.header->type) << '\n';
        cout << "ROM size:\t" << 32*(1 << cartridge.header->rom_size) << " KiB\n";
        cout << "RAM size:\t" << ram_size_value(cartridge.header->ram_size) << '\n';
        cout << "License Code: \t" << old_lic_name(cartridge.header->old_lic_code, cartridge.header->new_lic_code) << '\n';
        cout << "ROM version: \t" << (int)cartridge.header->mask_rom_version << '\n';

        uint16_t x = 0;
        for(uint16_t i = 0x0134; i <= 0x014C; i++){
            x = x - cartridge.rom_data[i] - 1;
        }
        cout << "Checksum: \t" << (int)cartridge.header->header_check_sum << ((x & 0xFF) ? " PASSED\n" : " FAILED\n");

        delete[] cartridge.rom_data;
    }
    else cout << "Unable to open file\n";
    return 0;
}