#pragma once
#define FILAS 12
#define COLS 22
#define MAX_ENEMIGOS 10
#define NUM_HABITACIONES 6
#define DIR_ARRIBA 0
#define DIR_ABAJO 1
#define DIR_IZQUIERDA 2
#define DIR_DERECHA 3
#define PUERTA_FILA1 5
#define PUERTA_FILA2 6
#define PUERTA_COL1 10
#define PUERTA_COL2 11

enum TipoEnemigo {
    FANTASMA, ESQUELETO
};
struct Posicion {
    int fila;
    int col;
};
struct Jugador {
    Posicion pos;
    bool tieneObjeto;
    int vida;
    int habitacionActual;
};
struct Enemigo {
    Posicion pos;
    TipoEnemigo tipo;
    bool activo;
    int contadorTurno;
    int habitacion;
};
struct Habitacion {
    char mapa[FILAS][COLS];
    int conexiones[4];
    bool tieneObjeto;
    Posicion posObjeto;
    bool esSalida;
    Posicion posSalida;
};
