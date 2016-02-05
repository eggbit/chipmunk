#include "chipmunk.h"

static bool chipmunk_load_rom(struct chip8 *c8, const char *rom_path);
static void chipmunk_load_font(struct chip8 *c8);

void
chipmunk_run(struct chip8 *c8) {
    // Opcode to get == ac3f
    //
    // memory[pc] == ac
    // memory[pc + 1] == 3f
    // memory[pc] << 8 == ac00 (logical or'd with) memory[pc + 1] == ac3f

    u16 opcode = c8->memory[c8->pc] << 8 | c8->memory[c8->pc + 1];

    // NOTE: Debug
    // printf("pc: %x - opcode: %x (%x)\n", c8->pc, opcode, (opcode & 0xf000) >> 12);

    // TODO: Handle opcodes
    switch ((opcode & 0xf000) >> 12) {
        case 0x0:
            // TODO: 0NNN, 00E0, 00EE
        break;

        case 0x1:
            // TODO: 1NNN
        break;

        case 0x2:
            // TODO: 2NNN
        break;

        case 0x3:
            // TODO: 3XNN
        break;

        case 0x4:
            // TODO: 4XNN
        break;

        case 0x5:
            // TODO: 5XY0
        break;

        case 0x6:
            // TODO: 6XNN
        break;

        case 0x7:
            // TODO: 7XNN
        break;

        case 0x8:
            // TODO: 8XY0, 8XY1, 8XY2, 8XY3, 8XY4, 8XY5, 8XY6, 8XY7, 8XYE
        break;

        case 0x9:
            // TODO: 9XY0
        break;

        case 0xa:
            // TODO: ANNN
        break;

        case 0xb:
            // TODO: BNNN
        break;

        case 0xc:
            // TODO: CXNN
        break;

        case 0xd:
            // TODO: DXYN
        break;

        case 0xe:
            // TODO: EX9E, EXA1
        break;

        case 0xf:
            // TODO: FX07, FX0A, FX15, FX18, FX19, FX29, FX33, FX55, FX65
        break;

        default:
            printf("%x: Unknown opcode\n", opcode);
        break;
    }

    // NOTE: Move to next instruction.
    c8->pc += 2;
}

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
