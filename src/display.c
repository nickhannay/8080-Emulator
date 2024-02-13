

#include "display.h"

const char* DISPLAY_TITLE = "Space Invaders";

const size_t SCREEN_HEIGHT = 224;
const size_t SCREEN_WIDTH = 256;

const size_t TEXTURE_HEIGHT = 224;
const size_t TEXTURE_WIDTH = 256;

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
    display -> converted_bitmap = calloc((VRAM_END - VRAM_START + 1) * 8, sizeof(uint32_t));
    return display;

}


/*
    Traverses VRAM, converting each pixel from 1 bit to 32 bit.
    This is needed to allow SDL to handle rendering
*/
int display_convertBitmap(CPUState* p_state, uint32_t *bitmap){
    int bitmap_index = 0;
    for(uint32_t v_index = VRAM_START; v_index <= VRAM_END; v_index += 1){
        byte b = p_state -> memory[v_index];
        for(int i = 0; i < 8; i += 1){
            byte px = (b >> i) & 0x01;
            if(px == 0x01){
                bitmap[bitmap_index] = 0xffffffff;
            } else{
                bitmap[bitmap_index] = 0x000000ff;
            }
            bitmap_index += 1;

        }
    }


    return 0;
}





int display_dumpBitmap(Display* d){
    int max = 256 * 224;
    for(int i = 0; i < max; i+= 1){
        printf("index [%04x] - decimal=%d, hex=%08x\n", i, d->converted_bitmap[i], d->converted_bitmap[i]);
    }

    return 0;
}


int display_renderFrame(Display* display){
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((uint32_t *)display -> converted_bitmap, TEXTURE_WIDTH,
                        TEXTURE_HEIGHT,
                        32,
                        TEXTURE_WIDTH * 4,
                        0xff000000,
                        0x00ff0000,
                        0x0000ff00,
                        0x000000ff
                        );

    if(!surface){
        printf("%s\n", SDL_GetError());
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(display -> renderer, surface);
    if(!texture){
        SDL_GetError();
    }
    


    SDL_RenderClear(display -> renderer);
    SDL_RenderCopyEx(display -> renderer, texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(display -> renderer);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    return 0;
}


void display_cleanup(Display* display){
    SDL_Quit();
    free(display -> converted_bitmap);
    free(display);
}
