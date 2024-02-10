#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <SDL2/SDL.h>
#include <unistd.h>

typedef struct Display{
    SDL_Window* window;
    SDL_Renderer* renderer;
    uint32_t *converted_bitmap;

} Display;


Display* display_init(void);

void display_cleanup(Display* display);


#endif


