#ifndef SDL_H
#define SDL_H

#include <stdio.h>
#include <SDL2/SDL.h>

void logSDLError(const char *msg){
    printf("%s error: %s\n", msg, SDL_GetError());
}

/*SDL_Texture* loadTexture(const char *file, SDL_Renderer *ren){
    //Initialize to NULL to avoid dangling pointer issues
    SDL_Texture *texture = NULL;
    //Load the image
    SDL_Surface *loadedImage = SDL_LoadBMP(file);
    //If the loading went ok, convert to texture and return the texture
    if (loadedImage != NULL){
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        //Make sure converting went ok too
        if (texture == NULL)
            logSDLError("CreateTextureFromSurface");
    }
    else
        logSDLError("LoadBMP");

    return texture;
}*/

/*SDL_Texture* loadTexture(const char *file, SDL_Renderer *ren) {
    SDL_Texture *texture = IMG_LoadTexture(ren, file);
    if (texture == NULL)     
        logSDLError("LoadTexture");
    return texture;
}*/

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

void drawGrid(short **grid, const int rows, const int cols, \
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
            else if(i/16 < rows && j/16 < cols && grid[i/16][j/16]) {
                SDL_RenderDrawPoint(renderer, j, i);
            }
        }
    }
}

#endif
