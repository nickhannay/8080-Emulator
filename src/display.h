#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <SDL2/SDL.h>
#include <unistd.h>
#include "cpu.h"

typedef struct Display{
    SDL_Window* window;
    SDL_Renderer* renderer;
    uint32_t *converted_bitmap;

} Display;


Display* display_init(void);

void display_cleanup(Display* display);


int display_convertBitmap(CPUState* p_state, uint32_t *bitmap);

int display_renderFrame(Display *display);

int display_dumpBitmap(Display* d);

#endif


