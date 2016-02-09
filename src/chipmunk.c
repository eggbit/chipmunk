#include "chipmunk.h"
#include <unistd.h> // for sleep.

static bool chipmunk_load_rom(struct chip8 *c8, const char *rom_path);
static void chipmunk_load_font(struct chip8 *c8);

void
chipmunk_run(struct chip8 *c8, u32 cycles) {
    // Timing
    float seconds_per_cycle = 60.0f / cycles;

    // Opcode to get == ac3f
    //
    // memory[pc] == ac
    // memory[pc + 1] == 3f
    // memory[pc] << 8 == ac00 (logical or'd with) memory[pc + 1] == ac3f
    u16 opcode = c8->memory[c8->pc] << 8 | c8->memory[c8->pc + 1];

    // Get potential VX and VY values.
    u8 op_x = (opcode & 0x0f00) >> 8;
    u8 op_y = (opcode & 0x00f0) >> 4;

    // NOTE: Debug
    // printf("pc: %x - op: %x\n", c8->pc, opcode);

    // TODO: Handle opcodes
    switch((opcode & 0xf000) >> 12) {
        case 0x0:
            switch(opcode & 0x00ff) {
                case 0x00e0:
                    memset(c8->display, 0, sizeof(c8->display));
                    puts("display clear.");
                break;

                case 0x00ee:
                    c8->pc = c8->stack[c8->sp];
                    c8->sp--;
                break;

                // TODO: 0NNN
                default:
                break;
            }
        break;

        case 0x1:
            c8->pc = (opcode & 0x0fff);
            return;
        break;

        case 0x2:
            c8->sp++;
            c8->stack[c8->sp] = c8->pc;
            c8->pc = (opcode & 0x0fff);
            return;
        break;

        case 0x3:
            if(c8->v[op_x] == (opcode & 0x00ff)) c8->pc += 2;
        break;

        case 0x4:
            if(c8->v[op_x] != (opcode & 0x00ff)) c8->pc += 2;
        break;

        case 0x5: {
            if(c8->v[op_x] == c8->v[op_y]) c8->pc += 2;
        }
        break;

        case 0x6:
            c8->v[op_x] = (opcode & 0x00ff);
        break;

        case 0x7:
            c8->v[op_x] += (opcode & 0x00ff);
        break;

        case 0x8: {
            switch(opcode & 0x000f) {
                case 0x0:
                    c8->v[op_x] = c8->v[op_y];
                break;

                case 0x1:
                    c8->v[op_x] |= c8->v[op_y];
                break;

                case 0x2:
                    c8->v[op_x] &= c8->v[op_y];
                break;

                case 0x3:
                    c8->v[op_x] ^= c8->v[op_y];
                break;

                case 0x4:
                    c8->v[0xf] = (c8->v[op_x] + c8->v[op_y] > 255 ? 1 : 0);
                    c8->v[op_x] += c8->v[op_y];
                break;

                case 0x5:
                    c8->v[0xf] = (c8->v[op_x] > c8->v[op_y] ? 1 : 0);
                    c8->v[op_x] -= c8->v[op_y];
                break;

                case 0x6:
                    c8->v[0xf] = (c8->v[op_x] & 1 ? 1 : 0);
                    c8->v[op_x] /= 2;
                break;

                case 0x7:
                    c8->v[0xf] = (c8->v[op_y] > c8->v[op_x] ? 1 : 0);
                    c8->v[op_x] = c8->v[op_y] - c8->v[op_x];
                break;

                case 0xe:
                    c8->v[0xf] = (c8->v[op_x] & 1 << 7 ? 1 : 0);
                    c8->v[op_x] *= 2;
                break;
            }
        }
        break;

        case 0x9:
            if(c8->v[op_x] != c8->v[op_y]) c8->pc += 2;
        break;

        case 0xa:
            c8->i = (opcode & 0x0fff);
        break;

        case 0xb:
            c8->pc = (opcode & 0x0fff) + c8->v[0];
        break;

        case 0xc:
            c8->v[op_x] = (rand() % 254) & (opcode & 0x00ff);
        break;

        case 0xd: {
            // NOTE: http://chip8.wikia.com/wiki/Instruction_Draw
            // NOTE: http://stackoverflow.com/questions/17568851/understanding-a-piece-of-code-in-chip8-emulator
            u8 height = (opcode & 0x000f);
            u8 x_pos = c8->v[op_x];
            u8 y_pos = c8->v[op_y];
            c8->v[0xf] = 0;

            for(u32 i = 0; i < 8; i++) {
                u16 data = c8->memory[c8->i + i];

                for(u32 j = 0; j < height; j++) {
                    if(data & (0x80 >> j))
                        c8->display[x_pos + i + (y_pos + j) * 64] ^= 1;
                }
            }
        }
        break;

        case 0xe:
            // TODO: EX9E, EXA1
        break;

        case 0xf: {
            switch(opcode & 0x00ff) {
                case 0x07:
                    c8->v[op_x] = c8->d_timer;
                break;

                case 0x0a:
                    // TODO: FX0A
                break;

                case 0x15:
                    c8->d_timer = c8->v[op_x];
                break;

                case 0x18:
                    c8->s_timer = c8->v[op_x];
                break;

                case 0x1e:
                    c8->i += c8->v[op_x];
                break;

                case 0x29:
                    // TODO: FX29
                    c8->i = c8->memory[c8->v[op_x]];
                break;

                case 0x33: {
                    u8 dec = c8->v[op_x];
                    c8->memory[c8->i] = dec / 100;
                    c8->memory[c8->i + 1] = dec % 100 / 10;
                    c8->memory[c8->i + 2] = dec % 10;
                }
                break;

                case 0x55:
                    for(int i = 0; i <= op_x; i++)
                        c8->memory[c8->i + i] = c8->v[i];
                break;

                case 0x65:
                    for(int i = 0; i <= op_x; i++)
                        c8->v[i] = c8->memory[c8->i + i];
                break;
            }
        }
        break;

        default:
            printf("%x: Unknown opcode\n", opcode);
        break;
    }

    // NOTE: Update timers.
    if(c8->d_timer > 0) c8->d_timer--;

    if(c8->s_timer > 0) {
        if(c8->s_timer == 1) c8->beep = true;
        c8->s_timer--;
    }

    // NOTE: Move to next instruction.
    c8->pc += 2;
    usleep(seconds_per_cycle / 60 * 1000000);
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
