#include "chipdef.h"

struct chip8 {
    u8 memory[0xfff];   // 4KB of memory.
    u8 v_reg[0xf];      // 16 8-bit registers, v0 - vf.
    u8 d_timer;         // 8-bit delay timer.
    u8 s_timer;         // 8-bit sound timer.
    u8 sp;              // 8-bit stack pointer.
    u16 pc;             // 16-bit program counter.
    u16 i_reg;          // 16-bit address register.
    u16 stack[0xf];     // 16-bit, 16-level stack.
};

void chipmunk_init(struct chip8 *c8);
void chipmunk_reset(struct chip8 *c8);

int
main(int argc, char const *argv[]) {
    // TODO: Init SDL.
    // TODO: Init chip8.
    // TODO: Load ROM.
    // TODO: Run.

    puts("Chipmunk: CHIP-8 Emulator");
    return 0;
}

void
chipmunk_init(struct chip8 *c8) {
}

void
chipmunk_reset(struct chip8 *c8) {
}
