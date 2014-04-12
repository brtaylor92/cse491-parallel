#include <errno.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdl.h"
#include "gol.h"

int main(int argc, char *argv[])
{
  if(argc != 2) {
      printf("format: %s [fileName]\n", argv[0]);
      return 1;
  }

  FILE *fp = fopen(argv[1], "r");
  if(!fp) {
    printf("Could not open file\n");
    return 1;
  }

  const int rows = 8;
  const int cols = 8;
  const int height  = 16 * rows + 1;
  const int width = 16 * cols + 1;
  square_t *gridA = (square_t*) malloc(sizeof(square_t) * rows * cols);
  square_t *gridB = (square_t*) malloc(sizeof(square_t) * rows * cols);

  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    logSDLError("SDL_Init");
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, width,
                                        height, SDL_WINDOW_SHOWN
                                       );
  if(!window) {
    logSDLError("CreateWindow");
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                              SDL_RENDERER_ACCELERATED | 
                                              SDL_RENDERER_PRESENTVSYNC
                                             );
  if(!renderer) {
    logSDLError("CreateRenderer");
    return 1;
  }
  
  readGrid(gridA, rows, cols, fp);
  fclose(fp);

  long numSteps = 0;
  int quit = 0;
  SDL_Event e;
  printGrid(gridA, rows, cols);
  drawGrid(gridA, rows, cols, width, height, renderer);
  SDL_Delay(500);
  //for(long i = 0; i < numSteps; i++) {
  while(!quit) {
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT){
          quit = 1;
      }
    }
    step(gridA, gridB, rows, cols);
    numSteps++;
    square_t *temp = gridA;
    gridA = gridB;
    gridB = temp;
    printGrid(gridA, rows, cols);
    drawGrid(gridA, rows, cols, width, height, renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(250);
  }

  printf("Iterated for %ld steps\n", numSteps);
  free(gridA);
  free(gridB);

  return 0;
}
