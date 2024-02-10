

#include "display.h"

#define DISPLAY_TITLE "Space Invaders"



Display* display_init(int width, int height){
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
        perror("Failed to initialize video subsytem for SDL\n");
        exit(EXIT_FAILURE);
    }
    Display* display = calloc(1, sizeof(Display));

    display -> window = SDL_CreateWindow(DISPLAY_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    display -> renderer = SDL_CreateRenderer(display -> window, -1, SDL_RENDERER_ACCELERATED);

    return display;

}

void display_cleanup(Display* display){
    SDL_Quit();
    free(display);
}
