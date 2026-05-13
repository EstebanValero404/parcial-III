#pragma once
#include <cstdlib>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define LIMPIAR system("cls")
#else
    #include <termios.h>
    #include <unistd.h>
    #define LIMPIAR system("clear")
#endif

char leerTecla();
