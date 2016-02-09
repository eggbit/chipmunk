#include "chipdef.h"
#include "chipmunk.h"

double
sys_float_time(u64 *time_counter, double seconds_per_tick) {
    static double time_passed = 0;

    u64 counter = SDL_GetPerformanceCounter();
    u64 interval = counter - *time_counter;

    *time_counter = counter;

    return time_passed += (double)interval * seconds_per_tick;
}

int
main(int argc, char const *argv[]) {
    if(argc < 2) return 1;  // TODO: Show usage.

    puts("Chipmunk: CHIP-8 Emulator");

    // NOTE: Chip8 initialization.
    struct chip8 c8 = {0};
    if(!chipmunk_init(&c8, argv[1])) return 1;

    // NOTE: SDL setup.
    SDL_Window *w = NULL;
    SDL_Renderer *r = NULL;
    SDL_Texture *t = NULL;

    // NOTE: SDL initialization.
    if(!sdl_init("chipmunk", 64 * 14, 32 * 14, &w, &r)) goto escape;
    t = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    // NOTE: Timer and frame rate set up.
    double seconds_per_tick = 1.0 / (double)SDL_GetPerformanceFrequency();
    double newtime = 0.0, oldtime = 0.0, time_accumulated = 0.0;
    double target_fps = 1.0 / 60.0;
    u64 time_count = SDL_GetPerformanceCounter();

    for(;;) {
        // NOTE: SDL event processing
        SDL_Event event;
        sdl_pump_events();

        if(sdl_event_exists(&event, SDL_QUIT)) goto escape;

        sdl_flush_events();

        // NOTE: Run the emulator.
        chipmunk_run(&c8, 2000);

        // NOTE: Timing update
        newtime = sys_float_time(&time_count, seconds_per_tick);
        time_accumulated += newtime - oldtime;
        oldtime = newtime;

        // NOTE: Do sound and rendering every 60th of a second.
        if(time_accumulated > target_fps) {
            if(c8.beep) {
                puts("BEEP");
                c8.beep = false;
            }

            // TODO: Has to be a better way to do this...
            for(u32 i = sizeof(c8.display); i--;) {
                if(c8.display[i]) c8.display[i] = 255;
            }

            SDL_UpdateTexture(t, NULL, c8.display, 64 * sizeof(u8));
            SDL_RenderClear(r);
            SDL_RenderCopy(r, t, NULL, NULL);
            SDL_RenderPresent(r);

            time_accumulated -= target_fps;
        }
    }

escape:
    if(t) SDL_DestroyTexture(t);
    if(r) SDL_DestroyRenderer(r);
    if(w) SDL_DestroyWindow(w);
    SDL_Quit();

    return 0;
}
