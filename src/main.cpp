#include "../include/mapa.h"
#include "../include/juego.h"
#include "../include/utilidades.h"

int main(int argc, char* argv[]) {
    if (!iniciarSDL()) return 1;
    inicializarHabitaciones();
    inicializarEnemigos();
    correrJuego();
    cerrarSDL();
    return 0;
}
