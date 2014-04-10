CC = gcc
CCFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic-errors
LDLIBS = 

gol: main.c gol.h
	$(CC) $(CCFLAGS) $(LDLIBS) $< -o $@

clean:
	rm -rf gol
