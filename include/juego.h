#pragma once
#include "tipos.h"

bool moverJugador(Jugador* jugador, char tecla);

void moverEnemigos(Jugador* jugador);

bool hayColisionEnemigo(Jugador* jugador);

void recogerObjeto(Jugador* jugador);

bool verificarVictoria(Jugador* jugador);
