#include <iostream>
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

void moverEnemigos(Jugador* jugador) {
    int hab = jugador->habitacionActual;

    for (int i = 0; i < totalEnemigos; i++) {
        Enemigo* e = &enemigos[i];

        if (!e->activo || e->habitacion != hab) continue;

        if (e->tipo == ESQUELETO) {
            e->contadorTurno++;
            if (e->contadorTurno % 2 != 0) continue;
        }

        int dr = 0, dc = 0;
        if (e->pos.fila < jugador->pos.fila) dr =  1;
        if (e->pos.fila > jugador->pos.fila) dr = -1;
        if (e->pos.col  < jugador->pos.col)  dc =  1;
        if (e->pos.col  > jugador->pos.col)  dc = -1;

        int nf = e->pos.fila + dr;
        int nc = e->pos.col  + dc;

        if (posValidaEnemigo(hab, nf, nc)) {
            e->pos.fila = nf;
            e->pos.col  = nc;
        } else if (posValidaEnemigo(hab, e->pos.fila + dr, e->pos.col)) {
            e->pos.fila += dr;
        } else if (posValidaEnemigo(hab, e->pos.fila, e->pos.col + dc)) {
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
    LIMPIAR;
    std::cout << "==========================================\n";
    std::cout << "         CALABOZO OSCURO  v1.0            \n";
    std::cout << "    Esteban Valero & Sebastian Gonzales   \n";
    std::cout << "==========================================\n\n";
    std::cout << "Eres [ @ ] atrapado en un calabozo.\n";
    std::cout << "Busca la llave [ K ] y llega a la salida [ E ].\n";
    std::cout << "Evita a los fantasmas [ G ] y esqueletos [ S ]!\n\n";
    std::cout << "Controles:\n";
    std::cout << "  W / A / S / D  ->  moverse\n";
    std::cout << "  Q              ->  salir\n\n";
    std::cout << "Mapa de habitaciones:\n";
    std::cout << "  [1]-[2]-[3]\n";
    std::cout << "            |\n";
    std::cout << "  [4]-[5]-[6]\n\n";
    std::cout << "  @ empieza en hab 1 | K en hab 5 | E en hab 4\n\n";
    std::cout << "Presiona cualquier tecla para comenzar...\n";
    leerTecla();
}
void mostrarEstado(Jugador* jugador, bool danio) {
    std::cout << "Vida: ";
    for (int i = 0; i < jugador->vida; i++) std::cout << "[v]";
    for (int i = jugador->vida; i < 3; i++) std::cout << "[ ]";
    std::cout << "  Inventario: ";
    if (jugador->tieneObjeto) std::cout << "[LLAVE]";
    else                      std::cout << "[vacio]";
    std::cout << "\n";
    if (danio) std::cout << "*** Recibiste dano! Perdiste una vida ***\n";
    else       std::cout << "\n";
    std::cout << "Leyenda: @=Tu  G=Fantasma(rapido)  S=Esqueleto(lento)  K=Llave  E=Salida\n";
    std::cout << "Meta: recoge K y llega a E  |  Controles: WASD  Q=Salir\n";
}
void pantallaFin(bool victoria, int vidas) {
    LIMPIAR;
    if (victoria) {
        std::cout << "==========================================\n";
        std::cout << "      *** VICTORIA! ESCAPASTE! ***        \n";
        std::cout << "==========================================\n";
        std::cout << "Has logrado salir del calabozo oscuro!\n";
        std::cout << "Vidas restantes: " << vidas << "/3\n";
    } else {
        std::cout << "==========================================\n";
        std::cout << "      *** DERROTA! HAS MUERTO! ***        \n";
        std::cout << "==========================================\n";
        std::cout << "El calabozo oscuro te ha vencido...\n";
    }
    std::cout << "\nPresiona cualquier tecla para salir...\n";
    leerTecla();
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
        LIMPIAR;
        dibujarHabitacion(jugador.habitacionActual, &jugador);
        mostrarEstado(&jugador, danioReciente);
        danioReciente = false;

        char tecla = leerTecla();
        if (tecla == 'q' || tecla == 'Q') break;

        moverJugador(&jugador, tecla);
        recogerObjeto(&jugador);

        if (verificarVictoria(&jugador)) {
            victoria    = true;
            juegoActivo = false;
            continue;
        }

        moverEnemigos(&jugador);

        if (hayColisionEnemigo(&jugador)) {
            jugador.vida--;
            danioReciente = true;
            jugador.pos   = {FILAS / 2, COLS / 2};
            if (jugador.vida <= 0)
                juegoActivo = false;
        }
    }
    pantallaFin(victoria, jugador.vida);
}
