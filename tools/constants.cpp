#include <string>
#include "constants.hpp"
#include <iostream>

using namespace std;

const string rom_types(uint8_t type_value){
    switch(type_value){
    case 0x00:
        return "ROM ONLY";
    case 0x01:
        return "MBC1";
    case 0x02:
        return "MBC1+RAM";
    case 0x03:
        return "MBC1+RAM+BATTERY";
    case 0x05:
        return "MBC2";
    case 0x06:
        return "MBC2+BATTERY";
    case 0x08:
        return "ROM+RAM";
    case 0x09:
        return "ROM+RAM+BATTERY";
    case 0x0B:
        return "MMM01";
    case 0x0C:
        return "MMM01+RAM";
    case 0x0D:
        return "MMM01+RAM+BATTERY";
    case 0x0F:
        return "MBC3+TIMER+BATTERY";
    case 0x10:
        return "MBC3+TIMER+RAM+BATTERY";
    case 0x11:
        return "MBC3";
    case 0x12:
        return "MBC3+RAM";
    case 0x13:
        return "MBC3+RAM+BATTERY";
    case 0x19:
        return "MBC5";
    case 0x1A:
        return "MBC5+RAM";
    case 0x1B:
        return "MBC5+RAM+BATTERY";
    case 0x1C:
        return "MBC5+RUMBLE";
    case 0x1D:
        return "MBC5+RUMBLE+RAM";
    case 0x1E:
        return "MBC5+RUMBLE+RAM+BATTERY";
    case 0x20:
        return "MBC6";
    case 0x22:
        return "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
    case 0xFC:
        return "POCKET CAMERA";
    case 0xFD:
        return "BANDAI TAMA5";
    case 0xFE:
        return "HuC3";
    case 0xFF:
        return "HuC1+RAM+BATTERY";
    default:
        return "No ROM type matches this value";
    }
}

const string ram_size_value(uint8_t size_value){
    switch(size_value){
    case 0x00:
        return "0";
    case 0x02:
        return "8 KiB";
    case 0x03:
        return "32 KiB";
    case 0x04:
        return "128 KiB";
    case 0x05:
        return "64 KiB";
    default:
        return "No RAM size matches this value";
    }
}

const string new_lic_name(uint8_t new_lic_code[]){
    string code;
    code.push_back(new_lic_code[0]);
    code.push_back(new_lic_code[1]);
    
    if(code == "00") {
        return "None";
    }
    else if(code == "01") {
        return "Nintendo Research & Development 1";
    }
    else if(code == "08") {
        return "Capcom";
    }
    else if(code == "13") {
        return "EA (Electronic Arts)";
    }
    else if(code == "18") {
        return "Hudson Soft";
    }
    else if(code == "19") {
        return "B-AI";
    }
    else if(code == "20") {
        return "KSS";
    }
    else if(code == "22") {
        return "Planning Office WADA";
    }
    else if(code == "24") {
        return "PCM Complete";
    }
    else if(code == "25") {
        return "San-X";
    }
    else if(code == "28") {
        return "Kemco";
    }
    else if(code == "29") {
        return "SETA Corporation";
    }
    else if(code == "30") {
        return "Viacom";
    }
    else if(code == "31") {
        return "Nintendo";
    }
    else if(code == "32") {
        return "Bandai";
    }
    else if(code == "33") {
        return "Ocean Software/Acclaim Entertainment";
    }
    else if(code == "34") {
        return "Konami";
    }
    else if(code == "35") {
        return "HectorSoft";
    }
    else if(code == "37") {
        return "Taito";
    }
    else if(code == "38") {
        return "Hudson Soft";
    }
    else if(code == "39") {
        return "Banpresto";
    }
    else if(code == "41") {
        return "Ubi Soft";
    }
    else if(code == "42") {
        return "Atlus";
    }
    else if(code == "44") {
        return "Malibu Interactive";
    }
    else if(code == "46") {
        return "Angel";
    }
    else if(code == "47") {
        return "Bullet-Proof Software";
    }
    else if(code == "49") {
        return "Irem";
    }
    else if(code == "50") {
        return "Absolute";
    }
    else if(code == "51") {
        return "Acclaim Entertainment";
    }
    else if(code == "52") {
        return "Activision";
    }
    else if(code == "53") {
        return "Sammy USA Corporation";
    }
    else if(code == "54") {
        return "Konami";
    }
    else if(code == "55") {
        return "Hi Tech Expressions";
    }
    else if(code == "56") {
        return "LJN";
    }
    else if(code == "57") {
        return "Matchbox";
    }
    else if(code == "58") {
        return "Mattel";
    }
    else if(code == "59") {
        return "Milton Bradley Company";
    }
    else if(code == "60") {
        return "Titus Interactive";
    }
    else if(code == "61") {
        return "Virgin Games Ltd.";
    }
    else if(code == "64") {
        return "Lucasfilm Games";
    }
    else if(code == "67") {
        return "Ocean Software";
    }
    else if(code == "69") {
        return "EA (Electronic Arts)";
    }
    else if(code == "70") {
        return "Infogrames";
    }
    else if(code == "71") {
        return "Interplay Entertainment";
    }
    else if(code == "72") {
        return "Broderbund";
    }
    else if(code == "73") {
        return "Sculptured Software";
    }
    else if(code == "75") {
        return "The Sales Curve Limited";
    }
    else if(code == "77") {
        return "THQ";
    }
    else if(code == "79") {
        return "Accolade";
    }
    else if(code == "80") {
        return "Misawa Entertainment";
    }
    else if(code == "83") {
        return "lozc";
    }
    else if(code == "86") {
        return "Tokuma Shoten";
    }
    else if(code == "87") {
        return "Tsukuda Original";
    }
    else if(code == "91") {
        return "Chunsoft Co.";
    }
    else if(code == "92") {
        return "Video System";
    }
    else if(code == "93") {
        return "Ocean Software/Acclaim Entertainment";
    }
    else if(code == "95") {
        return "Varie";
    }
    else if(code == "96") {
        return "Yonezawa/s’pal";
    }
    else if(code == "97") {
        return "Kaneko";
    }
    else if(code == "99") {
        return "Pack-In-Video";
    }
    else if(code == "9H") {
        return "Bottom Up";
    }
    else if(code == "A4") {
        return "Konami (Yu-Gi-Oh!)";
    }
    else if(code == "BL") {
        return "MTO";
    }
    else if(code == "DK") {
        return "Kodansha";
    }
    else{
        return "No licensee matches this code";
    }
}

const string old_lic_name(uint8_t old_lic_code, uint8_t new_lic_code[]){
    switch(old_lic_code){
    case 0x00:
        return "None";
    case 0x01:
        return "Nintendo";
    case 0x08:
        return "Capcom";
    case 0x09:
        return "HOT-B";
    case 0x0A:
        return "Jaleco";
    case 0x0B:
        return "Coconuts Japan";
    case 0x0C:
        return "Elite Systems";
    case 0x13:
        return "EA (Electronic Arts)";
    case 0x18:
        return "Hudson Soft";
    case 0x19:
        return "ITC Entertainment";
    case 0x1A:
        return "Yanoman";
    case 0x1D:
        return "Japan Clary";
    case 0x1F:
        return "Virgin Games Ltd.";
    case 0x24:
        return "PCM Complete";
    case 0x25:
        return "San-X";
    case 0x28:
        return "Kemco";
    case 0x29:
        return "SETA Corporation";
    case 0x30:
        return "Infogrames";
    case 0x31:
        return "Nintendo";
    case 0x32:
        return "Bandai";
    case 0x33:
        return new_lic_name(new_lic_code);
    case 0x34:
        return "Konami";
    case 0x35:
        return "HectorSoft";
    case 0x38:
        return "Capcom";
    case 0x39:
        return "Banpresto";
    case 0x3C:
        return "Entertainment Interactive (stub)";
    case 0x3E:
        return "Gremlin";
    case 0x41:
        return "Ubi Soft";
    case 0x42:
        return "Atlus";
    case 0x44:
        return "Malibu Interactive";
    case 0x46:
        return "Angel";
    case 0x47:
        return "Spectrum HoloByte";
    case 0x49:
        return "Irem";
    case 0x4A:
        return "Virgin Games Ltd.";
    case 0x4D:
        return "Malibu Interactive";
    case 0x4F:
        return "U.S. Gold";
    case 0x50:
        return "Absolute";
    case 0x51:
        return "Acclaim Entertainment";
    case 0x52:
        return "Activision";
    case 0x53:
        return "Sammy USA Corporation";
    case 0x54:
        return "GameTek";
    case 0x55:
        return "Park Place";
    case 0x56:
        return "LJN";
    case 0x57:
        return "Matchbox";
    case 0x59:
        return "Milton Bradley Company";
    case 0x5A:
        return "Mindscape";
    case 0x5B:
        return "Romstar";
    case 0x5C:
        return "Naxat Soft";
    case 0x5D:
        return "Tradewest";
    case 0x60:
        return "Titus Interactive";
    case 0x61:
        return "Virgin Games Ltd.";
    case 0x67:
        return "Ocean Software";
    case 0x69:
        return "EA (Electronic Arts)";
    case 0x6E:
        return "Elite Systems";
    case 0x6F:
        return "Electro Brain";
    case 0x70:
        return "Infogrames";
    case 0x71:
        return "Interplay Entertainment";
    case 0x72:
        return "Broderbund";
    case 0x73:
        return "Sculptured Software";
    case 0x75:
        return "The Sales Curve Limited";
    case 0x78:
        return "THQ";
    case 0x79:
        return "Accolade";
    case 0x7A:
        return "Triffix Entertainment";
    case 0x7C:
        return "MicroProse";
    case 0x7F:
        return "Kemco";
    case 0x80:
        return "Misawa Entertainment";
    case 0x83:
        return "LOZC G.";
    case 0x86:
        return "Tokuma Shoten";
    case 0x8B:
        return "Bullet-Proof Software";
    case 0x8C:
        return "Vic Tokai Corp.";
    case 0x8E:
        return "Ape Inc.";
    case 0x8F:
        return "I’Max";
    case 0x91:
        return "Chunsoft Co.";
    case 0x92:
        return "Video System";
    case 0x93:
        return "Tsubaraya Productions";
    case 0x95:
        return "Varie";
    case 0x96:
        return "Yonezawa/S’Pal";
    case 0x97:
        return "Kemco";
    case 0x99:
        return "Arc";
    case 0x9A:
        return "Nihon Bussan";
    case 0x9B:
        return "Tecmo";
    case 0x9C:
        return "Imagineer";
    case 0x9D:
        return "Banpresto";
    case 0x9F:
        return "Nova";
    case 0xA1:
        return "Hori Electric";
    case 0xA2:
        return "Bandai";
    case 0xA4:
        return "Konami";
    case 0xA6:
        return "Kawada";
    case 0xA7:
        return "Takara";
    case 0xA9:
        return "Technos Japan";
    case 0xAA:
        return "Broderbund";
    case 0xAC:
        return "Toei Animation";
    case 0xAD:
        return "Toho";
    case 0xAF:
        return "Namco";
    case 0xB0:
        return "Acclaim Entertainment";
    case 0xB1:
        return "ASCII Corporation or Nexsoft";
    case 0xB2:
        return "Bandai";
    case 0xB4:
        return "Square Enix";
    case 0xB6:
        return "HAL Laboratory";
    case 0xB7:
        return "SNK";
    case 0xB9:
        return "Pony Canyon";
    case 0xBA:
        return "Culture Brain";
    case 0xBB:
        return "Sunsoft";
    case 0xBD:
        return "Sony Imagesoft";
    case 0xBF:
        return "Sammy Corporation";
    case 0xC0:
        return "Taito";
    case 0xC2:
        return "Kemco";
    case 0xC3:
        return "Square";
    case 0xC4:
        return "Tokuma Shoten";
    case 0xC5:
        return "Data East";
    case 0xC6:
        return "Tonkin House";
    case 0xC8:
        return "Koei";
    case 0xC9:
        return "UFL";
    case 0xCA:
        return "Ultra Games";
    case 0xCB:
        return "VAP, Inc.";
    case 0xCC:
        return "Use Corporation";
    case 0xCD:
        return "Meldac";
    case 0xCE:
        return "Pony Canyon";
    case 0xCF:
        return "Angel";
    case 0xD0:
        return "Taito";
    case 0xD1:
        return "SOFEL (Software Engineering Lab)";
    case 0xD2:
        return "Quest";
    case 0xD3:
        return "Sigma Enterprises";
    case 0xD4:
        return "ASK Kodansha Co.";
    case 0xD6:
        return "Naxat Soft";
    case 0xD7:
        return "Copya System";
    case 0xD9:
        return "Banpresto";
    case 0xDA:
        return "Tomy";
    case 0xDB:
        return "LJN";
    case 0xDD:
        return "Nippon Computer Systems";
    case 0xDE:
        return "Human Ent.";
    case 0xDF:
        return "Altron";
    case 0xE0:
        return "Jaleco";
    case 0xE1:
        return "Towa Chiki";
    case 0xE2:
        return "Yutaka";
    case 0xE3:
        return "Varie";
    case 0xE5:
        return "Epoch";
    case 0xE7:
        return "Athena";
    case 0xE8:
        return "Asmik Ace Entertainment";
    case 0xE9:
        return "Natsume";
    case 0xEA:
        return "King Records";
    case 0xEB:
        return "Atlus";
    case 0xEC:
        return "Epic/Sony Records";
    case 0xEE:
        return "IGS";
    case 0xF0:
        return "A Wave";
    case 0xF3:
        return "Extreme Entertainment";
    case 0xFF:
        return "LJN";
    default:
        return "No licensee matches this code";
    }
}