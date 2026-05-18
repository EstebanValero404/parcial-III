#include <iostream>
#include "../include/tipos.h"
#include "../include/mapa.h"
#include "../include/utilidades.h"

Habitacion habitaciones[NUM_HABITACIONES];
Enemigo    enemigos[MAX_ENEMIGOS];
int        totalEnemigos = 0;

static void construirHabitacion(int idx) {
    Habitacion* hab = &habitaciones[idx];
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLS; j++)
            hab->mapa[i][j] = ' ';
    for (int j = 0; j < COLS; j++) {
        hab->mapa[0][j]       = '#';
        hab->mapa[FILAS-1][j] = '#';
    }
    for (int i = 0; i < FILAS; i++) {
        hab->mapa[i][0]      = '#';
        hab->mapa[i][COLS-1] = '#';
    }
    for (int d = 0; d < 4; d++)
        hab->conexiones[d] = -1;
    hab->tieneObjeto = false;
    hab->esSalida    = false;
}

static void agregarPuerta(int idx, int dir, int destino) {
    Habitacion* hab = &habitaciones[idx];
    hab->conexiones[dir] = destino;
    if (dir == DIR_ARRIBA) {
        hab->mapa[0][PUERTA_COL1] = ' ';
        hab->mapa[0][PUERTA_COL2] = ' ';
    } else if (dir == DIR_ABAJO) {
        hab->mapa[FILAS-1][PUERTA_COL1] = ' ';
        hab->mapa[FILAS-1][PUERTA_COL2] = ' ';
    } else if (dir == DIR_IZQUIERDA) {
        hab->mapa[PUERTA_FILA1][0] = ' ';
        hab->mapa[PUERTA_FILA2][0] = ' ';
    } else {
        hab->mapa[PUERTA_FILA1][COLS-1] = ' ';
        hab->mapa[PUERTA_FILA2][COLS-1] = ' ';
    }
}

void inicializarHabitaciones() {
    for (int i = 0; i < NUM_HABITACIONES; i++)
        construirHabitacion(i);
    agregarPuerta(0, DIR_DERECHA, 1);
    agregarPuerta(1, DIR_IZQUIERDA, 0);
    agregarPuerta(1, DIR_DERECHA,   2);
    agregarPuerta(2, DIR_IZQUIERDA, 1);
    agregarPuerta(2, DIR_ABAJO,     5);
    agregarPuerta(3, DIR_DERECHA, 4);
    habitaciones[3].esSalida  = true;
    habitaciones[3].posSalida = {PUERTA_FILA1, 2};
    agregarPuerta(4, DIR_IZQUIERDA, 3);
    agregarPuerta(4, DIR_DERECHA,   5);
    habitaciones[4].tieneObjeto = true;
    habitaciones[4].posObjeto   = {FILAS/2, COLS/2};
    agregarPuerta(5, DIR_IZQUIERDA, 4);
    agregarPuerta(5, DIR_ARRIBA,    2);
    habitaciones[1].mapa[3][8]  = '#';
    habitaciones[1].mapa[3][9]  = '#';
    habitaciones[1].mapa[8][13] = '#';
    habitaciones[1].mapa[8][14] = '#';
    for (int i = 2; i <= 5; i++)
        habitaciones[2].mapa[i][7] = '#';
    habitaciones[4].mapa[3][5]  = '#';
    habitaciones[4].mapa[8][16] = '#';
    habitaciones[5].mapa[4][11] = '#';
    habitaciones[5].mapa[4][12] = '#';
}

void inicializarEnemigos() {
    totalEnemigos = 0;
    enemigos[totalEnemigos++] = {{2, 18}, FANTASMA,  true, 0, 1};
    enemigos[totalEnemigos++] = {{8,  3}, FANTASMA,  true, 0, 1};
    enemigos[totalEnemigos++] = {{9, 15}, ESQUELETO, true, 0, 2};
    enemigos[totalEnemigos++] = {{2, 15}, ESQUELETO, true, 0, 2};
    enemigos[totalEnemigos++] = {{3,  5}, FANTASMA,  true, 0, 2};
    enemigos[totalEnemigos++] = {{7, 10}, ESQUELETO, true, 0, 3};
    enemigos[totalEnemigos++] = {{8, 10}, ESQUELETO, true, 0, 3};
    enemigos[totalEnemigos++] = {{10, 5}, FANTASMA, true, 0, 4};
    enemigos[totalEnemigos++] = {{2, 15}, FANTASMA, true, 0, 5};
    enemigos[totalEnemigos++] = {{10, 2}, ESQUELETO, true, 0, 5};
}

void dibujarHabitacion(int habitacionActual, Jugador* jugador) {
    Habitacion* hab = &habitaciones[habitacionActual];

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++) {
            int x = j * TILE_SIZE;
            int y = i * TILE_SIZE;
            char celda = hab->mapa[i][j];

            if (celda == '#') {
            
                dibujarRect(x, y, TILE_SIZE, TILE_SIZE, 80, 80, 80);
                
                dibujarRect(x+2, y+2, TILE_SIZE-4, TILE_SIZE-4, 60, 60, 60);
            } else {
                
                dibujarRect(x, y, TILE_SIZE, TILE_SIZE, 40, 25, 10);
            }
        }
    }

    
    if (hab->tieneObjeto) {
        int x = hab->posObjeto.col * TILE_SIZE;
        int y = hab->posObjeto.fila * TILE_SIZE;
        
        dibujarRect(x+8,  y+12, 16, 8,  255, 220, 0);
        dibujarRect(x+18, y+8,  6,  16, 255, 220, 0);
    }

    
    if (hab->esSalida) {
        int x = hab->posSalida.col * TILE_SIZE;
        int y = hab->posSalida.fila * TILE_SIZE;
        
        dibujarRect(x+4, y+4, TILE_SIZE-8, TILE_SIZE-8, 0, 200, 0);
        dibujarRect(x+10, y+10, TILE_SIZE-20, TILE_SIZE-20, 0, 255, 0);
    }

    
    for (int i = 0; i < totalEnemigos; i++) {
        Enemigo* e = &enemigos[i];
        if (!e->activo || e->habitacion != habitacionActual) continue;
        int x = e->pos.col  * TILE_SIZE;
        int y = e->pos.fila * TILE_SIZE;
        if (e->tipo == FANTASMA) {
            
            dibujarRect(x+4,  y+8,  24, 16, 100, 150, 255);
            dibujarRect(x+8,  y+4,  16, 8,  100, 150, 255);
            dibujarRect(x+10, y+10, 4,  4,  255, 255, 255);
            dibujarRect(x+18, y+10, 4,  4,  255, 255, 255);
        } else {
            
            dibujarRect(x+10, y+2,  12, 12, 220, 210, 190);
            dibujarRect(x+12, y+14, 8,  14, 220, 210, 190);
            dibujarRect(x+6,  y+16, 6,  10, 220, 210, 190);
            dibujarRect(x+20, y+16, 6,  10, 220, 210, 190);
        }
    }

    
    {
        int x = jugador->pos.col  * TILE_SIZE;
        int y = jugador->pos.fila * TILE_SIZE;
        dibujarRect(x+8,  y+4,  16, 16, 255, 50, 50);
        dibujarRect(x+10, y+20, 12, 8,  255, 50, 50);
        dibujarRect(x+6,  y+22, 6,  8,  255, 50, 50);
        dibujarRect(x+20, y+22, 6,  8,  255, 50, 50);
    }

    SDL_RenderPresent(gRenderer);
}
