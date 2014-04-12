#ifndef SDL_H
#define SDL_H

#include <stdio.h>
#include <SDL2/SDL.h>

#include "gol.h"

void logSDLError(const char *msg){
  printf("%s error: %s\n", msg, SDL_GetError());
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, 
                  int x, int y, int w, int h) {
  //Setup the destination rectangle to be at the position we want
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTextureNoScale(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  renderTexture(tex, ren, x, y, w, h);
}

void drawGrid(square_t *grid, const int rows, const int cols, \
              const int width, const int height, \
              SDL_Renderer *renderer) {
  (void) rows;
  (void) cols;
  //Clear screen
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  for(int i = 0; i < height; i++) {
    if(i % 16 == 0 || i % 4 == 15) {
      SDL_RenderDrawLine(renderer, 0, i, width, i);
    }
    for(int j = 0; j < width; j++) {
      if(j % 16 == 0 || j % 16 == 15) {
        SDL_RenderDrawPoint(renderer, j, i);
      }
      else if(i/16 < rows && j/16 < cols && grid(i/16, j/16)) {
        SDL_RenderDrawPoint(renderer, j, i);
      }
    }
  }
}

#endif