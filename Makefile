CC = gcc
CCFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic-errors
LDLIBS = -I/Library/Frameworks/SDL.framework/Headers
SDL = -framework SDL2 -framework SDL2_image
LDFLAGS = $(SDL)

gol: main.c sdl.h gol.h
	$(CC) $(CCFLAGS) $(LDLIBS) $(SDL) -g $^ -o $@

clean:
	rm -rf gol