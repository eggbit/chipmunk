CC = clang
CFLAGS = -std=c11 -g -O0 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-missing-braces -Wno-missing-field-initializers
IFLAGS = -I/usr/local/include/SDL2 -D_THREAD_SAFE
LFLAGS = -L/usr/local/lib -lSDL2
FILES = src/main.c src/sdl_helper.c src/chipmunk.c

all: chipmunk

chipmunk: $(FILES)
	@$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(FILES) -o chipmunk

clean:
	rm -rf *.o *.dSYM chipmunk
