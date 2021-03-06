#include "chipdef.h"

struct chip8 {
    bool beep;              // Let's us know if it's time to beep.
    u8 memory[0x1000];      // 4KB of memory.
    u8 display[64 * 32];    // Screen with a resolution of 64x32.
    u8 v[0x10];             // 16 8-bit registers, v0 - vf.
    u8 d_timer;             // 8-bit delay timer.
    u8 s_timer;             // 8-bit sound timer.
    u8 sp;                  // 8-bit stack pointer.
    u16 pc;                 // 16-bit program counter.
    u16 i;                  // 16-bit address register.
    u16 stack[0x10];        // 16-bit, 16-level stack.
};

bool chipmunk_init(struct chip8 *c8, const char *rom_path);
void chipmunk_run(struct chip8 *c8, u32 cycles);
