#include "chipdef.h"
#include "chipmunk.h"
#include <unistd.h> // for sleep.

int
main(int argc, char const *argv[]) {
    if(argc < 2) return 1;  // TODO: Show usage.

    puts("Chipmunk: CHIP-8 Emulator");

    struct chip8 c8 = {0};
    if(!chipmunk_init(&c8, argv[1])) return 1;

    SDL_Window *w;
    SDL_Renderer *r;

    if(!sdl_init("chipmunk", 64 * 4, 32 * 4, &w, &r)) goto escape;

    for(;;) {
        SDL_Event event;
        sdl_pump_events();

        if(sdl_event_exists(&event, SDL_QUIT)) goto escape;

        sdl_flush_events();

        chipmunk_run(&c8);

        // TODO: Draw frame every 16.6ms
        usleep(250 * 1000);
    }

escape:
    if(w) SDL_DestroyWindow(w);
    if(r) SDL_DestroyRenderer(r);
    SDL_Quit();

    return 0;
}
