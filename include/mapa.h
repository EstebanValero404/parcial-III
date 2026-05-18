#pragma once
#include "tipos.h"
extern Habitacion habitaciones[NUM_HABITACIONES];
extern Enemigo enemigos[MAX_ENEMIGOS];
extern int totalEnemigos;
void inicializarHabitaciones();
void inicializarEnemigos();
void dibujarHabitacion(int habitacionActual, Jugador* jugador);
