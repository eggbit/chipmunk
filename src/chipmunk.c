#include "chipmunk.h"

static bool chipmunk_load_rom(struct chip8 *c8, const char *rom_path);
static void chipmunk_load_font(struct chip8 *c8);

bool
chipmunk_init(struct chip8 *c8, const char *rom_path) {
    c8->pc = 0x200;
    chipmunk_load_font(c8);

    if(!chipmunk_load_rom(c8, rom_path)) {
        printf("%s: Error opening ROM.\n", rom_path);
        return false;
    }
    else {
        printf("%s: ROM loaded successfully.\n", rom_path);
        return true;
    }
}

void
chipmunk_run(struct chip8 *c8) {
    // TODO: Execute instructions.
}

bool
chipmunk_load_rom(struct chip8 *c8, const char *rom_path) {
    FILE *rom = fopen(rom_path, "rb");
    if(!rom) goto failure;

    // Get the file size.
    fseek(rom, 0, SEEK_END);
    size_t rom_size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    // Read rom into memory starting at 0x200 and verify.
    size_t amount_read = fread(c8->memory + 0x200, 1, rom_size, rom);
    if(amount_read != rom_size) goto failure;

    fclose(rom);
    return true;

failure:
    if(rom) fclose(rom);
    return false;
}

// Load font data into memory
void
chipmunk_load_font(struct chip8 *c8) {
    u8 font_data[] = {
        0xf0, 0x90, 0x90, 0x90, 0xf0,   // 0
        0x20, 0x60, 0x20, 0x20, 0x70,   // 1
        0xf0, 0x10, 0xf0, 0x80, 0xf0,   // 2
        0xf0, 0x10, 0xf0, 0x10, 0xf0,   // 3
        0x90, 0x90, 0xf0, 0x10, 0x10,   // 4
        0xf0, 0x80, 0xf0, 0x10, 0xf0,   // 5
        0xf0, 0x80, 0xf0, 0x90, 0xf0,   // 6
        0xf0, 0x10, 0x20, 0x40, 0x40,   // 7
        0xf0, 0x90, 0xf0, 0x90, 0xf0,   // 8
        0xf0, 0x90, 0xf0, 0x10, 0xf0,   // 9
        0xf0, 0x90, 0xf0, 0x90, 0x90,   // a
        0xe0, 0x90, 0xe0, 0x90, 0xe0,   // b
        0xf0, 0x80, 0x80, 0x80, 0xf0,   // c
        0xe0, 0x90, 0x90, 0x90, 0xe0,   // d
        0xf0, 0x80, 0xf0, 0x80, 0xf0,   // e
        0xf0, 0x80, 0xf0, 0x80, 0x80    // f
    };

    memcpy(c8->memory, font_data, sizeof(font_data));
}
