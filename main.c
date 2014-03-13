#include <errno.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdl.h"
#include "gol.h"

int main(int argc, char const *argv[])
{
	if(argc < 3) {
		printf("format: %s [fileName] [numSteps]\n", argv[0]);
		return 1;
	}

	FILE *fp = fopen(argv[1], "r");
	if(!fp) {
		printf("Could not open file\n");
		return 1;
	}

	long numSteps = strtol(argv[2], NULL, 10);
	if (errno != 0) {
    printf("Unable to process argument as int\n");
    return 1;
	}
	(void) numSteps;

	short rules[2][9] = {{0,0,0,1,0,0,0,0,0},{0,0,1,1,0,0,0,0,0}};
	const int rows = 8;
	const int cols = 8;
	const int height  = 16 * rows + 1;
	const int width = 16 * cols + 1;
	short **grid = (short**) malloc(sizeof(short*) * rows);
	for(int i = 0; i < rows; i++) {
		grid[i] = (short*) malloc(sizeof(short) * cols);
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError("SDL_Init");
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, width,
		height, SDL_WINDOW_SHOWN);
	if(!window) {
		logSDLError("CreateWindow");
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!renderer) {
		logSDLError("CreateRenderer");
		return 1;
	}
	
	readGrid(grid, rows, cols, fp);
	fclose(fp);

	int quit = 0;
	SDL_Event e;
	printGrid(grid, rows, cols);
	drawGrid(grid, rows, cols, width, height, renderer);
	SDL_Delay(500);
	//for(long i = 0; i < numSteps; i++) {
	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT){
				quit = 1;
			}
		}
		step(grid, rows, cols, rules);
		printGrid(grid, rows, cols);
		drawGrid(grid, rows, cols, width, height, renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(250);
	}
	
	/*int quit = 0;
	SDL_Event e;
	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT){
				quit = 1;
			}
		}
		//SDL_RenderPresent(renderer);
	}*/

	for(int i = 0; i < rows; i++) {
		free(grid[i]);
	}
	free(grid);

	return 0;
}
