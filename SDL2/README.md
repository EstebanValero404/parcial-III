Gráficos con SDL2
¿Para qué se implementó?
Para reemplazar el sistema de caracteres ASCII por una ventana gráfica real con 
colores y sprites, mejorando significativamente la experiencia visual del juego 
y acercándolo más al estilo visual de Adventure (Atari 2600).

¿Por qué se consideró necesario?
El juego en consola de texto cumplía los requisitos mínimos, pero carecía de 
valor diferencial visual. Implementar gráficos reales convierte el proyecto en 
algo más cercano a un videojuego real y demuestra la capacidad de integrar 
tecnologías externas al lenguaje base.

¿Cómo se llevó a cabo su implementación?
Se utilizó SDL2 (Simple DirectMedia Layer), una librería de código abierto 
ampliamente utilizada en la industria del desarrollo de videojuegos. SDL2 
proporciona acceso de bajo nivel a hardware de audio, teclado, ratón y gráficos 
a través de OpenGL y Direct3D, siendo compatible con Windows, Linux y Mac.

La integración se realizó de la siguiente manera:
- Se agregó la carpeta `SDL2/` al proyecto con los headers y librerías necesarias
- Se modificó `utilidades.h` para incluir SDL2 y definir el renderer global
- Se crearon las funciones `iniciarSDL()`, `cerrarSDL()` y `dibujarRect()`
- Se reescribió `dibujarHabitacion()` en `mapa.cpp` para usar SDL2 en lugar de `cout`
- Se actualizó `leerTecla()` para usar eventos SDL en lugar de `_getch()`
- El makefile detecta automáticamente el sistema operativo y usa el path correcto

Elementos visuales implementados:
- Paredes en gris oscuro con borde
- Suelo café oscuro
- Jugador representado en rojo
- Fantasmas en azul claro
- Esqueletos en blanco hueso
- Llave en amarillo
- Salida en verde
