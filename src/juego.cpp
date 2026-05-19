#include "../include/tipos.h"
#include "../include/mapa.h"
#include "../include/juego.h"
#include "../include/utilidades.h"
static bool posValidaJugador(int hab, int fila, int col) {
    if (fila < 0 || fila >= FILAS || col < 0 || col >= COLS) return false;
    return habitaciones[hab].mapa[fila][col] != '#';
}

static bool posValidaEnemigo(int hab, int fila, int col) {
    if (fila <= 0 || fila >= FILAS-1) return false;
    if (col  <= 0 || col  >= COLS-1)  return false;
    return habitaciones[hab].mapa[fila][col] != '#';
}

bool moverJugador(Jugador* jugador, char tecla) {
    int nf = jugador->pos.fila;
    int nc = jugador->pos.col;

    if      (tecla == 'w' || tecla == 'W') nf--;
    else if (tecla == 's' || tecla == 'S') nf++;
    else if (tecla == 'a' || tecla == 'A') nc--;
    else if (tecla == 'd' || tecla == 'D') nc++;
    else return false;

    int hab = jugador->habitacionActual;

    if (!posValidaJugador(hab, nf, nc)) return false;

    jugador->pos.fila = nf;
    jugador->pos.col  = nc;

    Habitacion* h = &habitaciones[hab];
    int destino = -1;

    if (nc == 0 && (nf == PUERTA_FILA1 || nf == PUERTA_FILA2)) {
        destino = h->conexiones[DIR_IZQUIERDA];
        if (destino != -1) {
            jugador->habitacionActual = destino;
            jugador->pos.col = COLS - 2;
        }
    } else if (nc == COLS-1 && (nf == PUERTA_FILA1 || nf == PUERTA_FILA2)) {
        destino = h->conexiones[DIR_DERECHA];
        if (destino != -1) {
            jugador->habitacionActual = destino;
            jugador->pos.col = 1;
        }
    } else if (nf == 0 && (nc == PUERTA_COL1 || nc == PUERTA_COL2)) {
        destino = h->conexiones[DIR_ARRIBA];
        if (destino != -1) {
            jugador->habitacionActual = destino;
            jugador->pos.fila = FILAS - 2;
        }
    } else if (nf == FILAS-1 && (nc == PUERTA_COL1 || nc == PUERTA_COL2)) {
        destino = h->conexiones[DIR_ABAJO];
        if (destino != -1) {
            jugador->habitacionActual = destino;
            jugador->pos.fila = 1;
        }
    }

    return (destino != -1);
}
static bool celdaOcupadaPorEnemigo(int fila, int col, int ignorarIdx) {
    for (int i = 0; i < totalEnemigos; i++) {
        if (i == ignorarIdx) continue;
        if (!enemigos[i].activo) continue;
        if (enemigos[i].pos.fila == fila && enemigos[i].pos.col == col)
            return true;
    }
    return false;
}
void moverEnemigos(Jugador* jugador) {
    int hab = jugador->habitacionActual;

    for (int i = 0; i < totalEnemigos; i++) {
        Enemigo* e = &enemigos[i];

        if (!e->activo || e->habitacion != hab) continue;

        if (e->tipo == FANTASMA) {
            e->contadorTurno++;
        if (e->contadorTurno % 2 != 0) continue;
    }
    if (e->tipo == ESQUELETO) {
        e->contadorTurno++;
    if (e->contadorTurno % 4 != 0) continue;
    }

        int dr = 0, dc = 0;
        if (e->pos.fila < jugador->pos.fila) dr =  1;
        if (e->pos.fila > jugador->pos.fila) dr = -1;
        if (e->pos.col  < jugador->pos.col)  dc =  1;
        if (e->pos.col  > jugador->pos.col)  dc = -1;

        int nf = e->pos.fila + dr;
        int nc = e->pos.col  + dc;

        if (posValidaEnemigo(hab, nf, nc) && !celdaOcupadaPorEnemigo(nf, nc, i)) {
    e->pos.fila = nf;
    e->pos.col  = nc;
} else if (posValidaEnemigo(hab, e->pos.fila + dr, e->pos.col) && 
           !celdaOcupadaPorEnemigo(e->pos.fila + dr, e->pos.col, i)) {
    e->pos.fila += dr;
} else if (posValidaEnemigo(hab, e->pos.fila, e->pos.col + dc) && 
           !celdaOcupadaPorEnemigo(e->pos.fila, e->pos.col + dc, i)) {
    e->pos.col  += dc;
}
    }
}
bool hayColisionEnemigo(Jugador* jugador) {
    for (int i = 0; i < totalEnemigos; i++) {
        Enemigo* e = &enemigos[i];
        if (!e->activo || e->habitacion != jugador->habitacionActual) continue;
        if (e->pos.fila == jugador->pos.fila && e->pos.col == jugador->pos.col)
            return true;
    }
    return false;
}

void recogerObjeto(Jugador* jugador) {
    Habitacion* hab = &habitaciones[jugador->habitacionActual];
    if (hab->tieneObjeto && !jugador->tieneObjeto) {
        if (jugador->pos.fila == hab->posObjeto.fila &&
            jugador->pos.col  == hab->posObjeto.col) {
            jugador->tieneObjeto = true;
            hab->tieneObjeto     = false;
        }
    }
}

bool verificarVictoria(Jugador* jugador) {
    Habitacion* hab = &habitaciones[jugador->habitacionActual];
    if (hab->esSalida && jugador->tieneObjeto) {
        if (jugador->pos.fila == hab->posSalida.fila &&
            jugador->pos.col  == hab->posSalida.col)
            return true;
    }
    return false;
}
void pantallaInicio() {
    SDL_Delay(100);
}

void pantallaFin(bool victoria, int vidas) {
    SDL_Delay(2000);
    bool esperando = true;
    while (esperando) {
        if (victoria) {
            dibujarRect(0, 0, ANCHO_VENTANA, ALTO_VENTANA, 0, 100, 0);
            dibujarRect(100, 150, 500, 60, 0, 200, 0);
            dibujarRect(150, 250, 400, 60, 0, 180, 0);
        } else {
            dibujarRect(0, 0, ANCHO_VENTANA, ALTO_VENTANA, 100, 0, 0);
            dibujarRect(100, 150, 500, 60, 200, 0, 0);
            dibujarRect(150, 250, 400, 60, 180, 0, 0);
        }
        SDL_RenderPresent(gRenderer);
        char tecla = leerTecla();
        if (tecla == 'q' || tecla == 'Q') {
            esperando = false;
        }
    }
}
static void aplicarDanio(Jugador* jugador, bool& danioReciente, bool& juegoActivo) {
    jugador->vida--;
    danioReciente = true;
    bool respawnOk = false;
    for (int f = 1; f < FILAS-1 && !respawnOk; f++) {
        for (int c = 1; c < COLS-1 && !respawnOk; c++) {
            if (habitaciones[jugador->habitacionActual].mapa[f][c] != '#') {
                jugador->pos = {f, c};
                respawnOk = true;
            }
        }
    }
    if (jugador->vida <= 0)
        juegoActivo = false;
}
void soltarObjeto(Jugador* jugador) {
    Habitacion* hab = &habitaciones[jugador->habitacionActual];
    if (jugador->tieneObjeto && !hab->tieneObjeto) {
        jugador->tieneObjeto = false;
        hab->tieneObjeto     = true;
        hab->posObjeto       = jugador->pos;
    }
}
void correrJuego() {
    pantallaInicio();
    Jugador jugador;
    jugador.pos              = {PUERTA_FILA1, 2};
    jugador.tieneObjeto      = false;
    jugador.vida             = 3;
    jugador.habitacionActual = 0;

    bool juegoActivo   = true;
    bool victoria      = false;
    bool danioReciente = false;

    while (juegoActivo) {
        dibujarHabitacion(jugador.habitacionActual, &jugador);
        char tecla = leerTecla();
        if (tecla == 'e' || tecla == 'E') {
            soltarObjeto(&jugador);
        continue;
        }
        if (tecla == 'q' || tecla == 'Q') break;

        moverJugador(&jugador, tecla);

if (hayColisionEnemigo(&jugador)) {
    aplicarDanio(&jugador, danioReciente, juegoActivo);
    continue;
}
recogerObjeto(&jugador);
if (verificarVictoria(&jugador)) {
    victoria    = true;
    juegoActivo = false;
    continue;
}

moverEnemigos(&jugador);

if (hayColisionEnemigo(&jugador)) {
    aplicarDanio(&jugador, danioReciente, juegoActivo);
}
    }
    pantallaFin(victoria, jugador.vida);
  }
