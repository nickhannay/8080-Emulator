

#include "display.h"

const char* DISPLAY_TITLE = "Space Invaders";

const size_t SCREEN_HEIGHT = 224;
const size_t SCREEN_WIDTH = 256;

const uint16_t VRAM_START = 0x2400;
const uint16_t VRAM_END = 0x3fff;

Display* display_init(void){
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
        perror("Failed to initialize video subsytem for SDL\n");
        exit(EXIT_FAILURE);
    }
    Display* display = calloc(1, sizeof(Display));

    display -> window = SDL_CreateWindow(DISPLAY_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    display -> renderer = SDL_CreateRenderer(display -> window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(display -> renderer, 0, 0, 0, 0xff);

    // initialize bitmap used to draw to screen 
    display -> converted_bitmap = calloc(VRAM_END - VRAM_START + 1, sizeof(uint32_t));
    return display;

}

void display_cleanup(Display* display){
    SDL_Quit();
    free(display -> converted_bitmap);
    free(display);
}
