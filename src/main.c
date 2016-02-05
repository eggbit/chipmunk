#include "chipdef.h"
#include "chipmunk.h"
#include <unistd.h> // for sleep.

int
main(int argc, char const *argv[]) {
    // TODO: Init SDL.

    if(argc < 2) return 1;  // TODO: Show usage.

    puts("Chipmunk: CHIP-8 Emulator");

    struct chip8 c8 = {0};
    if(!chipmunk_init(&c8, argv[1])) return 1;

    for(;;) {
        chipmunk_run(&c8);

        // TODO: Draw frame every 16.6ms
        sleep(1);
    }

    return 0;
}
