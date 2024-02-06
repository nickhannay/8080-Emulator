
#include <SDL2/SDL.h>
#include <unistd.h>

int main(){

    if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
        perror("Failed to initialize video subsytem for SDL\n");
        exit(EXIT_FAILURE);
    }

    SDL_Window* window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 448, 512, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    sleep(10);

    SDL_Quit();


    return 0;
}
