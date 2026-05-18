#pragma once
#include <SDL.h>
#define TILE_SIZE 32
#define ANCHO_VENTANA (22 * TILE_SIZE)
#define ALTO_VENTANA  (12 * TILE_SIZE)

extern SDL_Window*   gVentana;
extern SDL_Renderer* gRenderer;

bool iniciarSDL();
void cerrarSDL();
char leerTecla();

void dibujarRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);
