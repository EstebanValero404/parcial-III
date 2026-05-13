#pragma once
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #define LIMPIAR system("cls")
#else
    #define LIMPIAR system("clear")
#endif

char leerTecla();
