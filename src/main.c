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

    struct chip8 c8 = {0};
    if(!chipmunk_init(&c8, argv[1])) return 1;

    SDL_Window *w;
    SDL_Renderer *r;

    if(!sdl_init("chipmunk", 64 * 4, 32 * 4, &w, &r)) goto escape;

    // Set up timers and frame rate
    double seconds_per_tick = 1.0 / (double)SDL_GetPerformanceFrequency();
    double newtime = 0.0, oldtime = 0.0, time_accumulated = 0.0;
    double target_fps = 1.0 / 60.0;
    u64 time_count = SDL_GetPerformanceCounter();

    for(;;) {
        SDL_Event event;
        sdl_pump_events();

        if(sdl_event_exists(&event, SDL_QUIT)) goto escape;

        sdl_flush_events();

        chipmunk_run(&c8, 500);

        newtime = sys_float_time(&time_count, seconds_per_tick);
        time_accumulated += newtime - oldtime;
        oldtime = newtime;

        if(time_accumulated > target_fps) {
            if(c8.beep) {
                puts("BEEP");
                c8.beep = false;
            }
            
            time_accumulated -= target_fps;
        }
    }

escape:
    if(w) SDL_DestroyWindow(w);
    if(r) SDL_DestroyRenderer(r);
    SDL_Quit();

    return 0;
}
