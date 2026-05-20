#include "../include/utilidades.h"
SDL_Window*   gVentana  = nullptr;
SDL_Renderer* gRenderer = nullptr;

bool iniciarSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    gVentana = SDL_CreateWindow(
        "Calabozo Oscuro",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ANCHO_VENTANA, ALTO_VENTANA, 0
    );
    if (!gVentana) return false;

    gRenderer = SDL_CreateRenderer(gVentana, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) return false;

    return true;
}

void cerrarSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gVentana);
    SDL_Quit();
}

void dibujarRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(gRenderer, r, g, b, 255);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(gRenderer, &rect);
}

char leerTecla() {
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) return 'q';
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_w: case SDLK_UP:    return 'w';
                case SDLK_s: case SDLK_DOWN:  return 's';
                case SDLK_a: case SDLK_LEFT:  return 'a';
                case SDLK_d: case SDLK_RIGHT: return 'd';
                case SDLK_q: case SDLK_ESCAPE: return 'q';
                case SDLK_k: case SDLK_RETURN: return 'K';
                case SDLK_e: return 'e';
                case SDLK_f: return 'f';
                default: return ' ';
            }
        }
    }
    return ' ';
}
