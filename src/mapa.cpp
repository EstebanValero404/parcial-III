#include <iostream>
#include "../include/tipos.h"
#include "../include/mapa.h"
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
    hab->esSalida = false;
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
    enemigos[totalEnemigos++] = {{8,  3}, FANTASMA,  true, 0, 5};
    enemigos[totalEnemigos++] = {{9, 15}, ESQUELETO, true, 0, 2};
    enemigos[totalEnemigos++] = {{2, 15}, ESQUELETO, true, 0, 4};
}

void dibujarHabitacion(int habitacionActual, Jugador* jugador) {
    Habitacion* hab = &habitaciones[habitacionActual];
    char display[FILAS][COLS];
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLS; j++)
            display[i][j] = hab->mapa[i][j];
    if (hab->tieneObjeto)
        display[hab->posObjeto.fila][hab->posObjeto.col] = 'K';
    if (hab->esSalida)
        display[hab->posSalida.fila][hab->posSalida.col] = 'E';
    for (int i = 0; i < totalEnemigos; i++) {
        Enemigo* e = &enemigos[i];
        if (e->activo && e->habitacion == habitacionActual) {
            char simbolo = (e->tipo == FANTASMA) ? 'G' : 'S';
            display[e->pos.fila][e->pos.col] = simbolo;
        }
    }
    display[jugador->pos.fila][jugador->pos.col] = '@';
    std::cout << " CALABOZO OSCURO | Habitacion "
              << (habitacionActual + 1) << " de " << NUM_HABITACIONES << "\n";
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLS; j++)
            std::cout << display[i][j];
        std::cout << "\n";
    }
}
