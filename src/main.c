#include "chipdef.h"

struct chip8 {
    u8 memory[0xfff];       // 4KB of memory.
    u8 display[64 * 32];    // Screen with a resolution of 64x32.
    u8 v[0xf];              // 16 8-bit registers, v0 - vf.
    u8 d_timer;             // 8-bit delay timer.
    u8 s_timer;             // 8-bit sound timer.
    u8 sp;                  // 8-bit stack pointer.
    u16 pc;                 // 16-bit program counter.
    u16 i;                  // 16-bit address register.
    u16 stack[0xf];         // 16-bit, 16-level stack.
};

bool chipmunk_init(struct chip8 *c8);
bool chipmunk_reset(struct chip8 *c8);
bool chipmunk_load(struct chip8 *c8, const char *rom_path) ;

int
main(int argc, char const *argv[]) {
    // TODO: Init SDL.
    // TODO: Init chip8.
    // TODO: Run.

    struct chip8 c8 = {0};
    const char *rom_path = "demo.ch8";

    puts("Chipmunk: CHIP-8 Emulator");

    if(chipmunk_load(&c8, rom_path)) printf("%s: Load successful.\n", rom_path);
    else printf("%s: Error opening ROM.\n", rom_path);

    return 0;
}

bool
chipmunk_init(struct chip8 *c8) {
    return true;
}

bool
chipmunk_reset(struct chip8 *c8) {
    return true;
}

bool
chipmunk_load(struct chip8 *c8, const char *rom_path) {
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
