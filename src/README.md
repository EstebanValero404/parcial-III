1.Sistema de detección por distancia para enemigos
¿Para qué se implementó?
Para balancear la dificultad del juego. Sin este sistema el fantasma perseguía al jugador desde cualquier punto de la habitación, haciendo el juego imposible.
¿Por qué se consideró necesario?
El comportamiento original de persecución constante eliminaba cualquier posibilidad de estrategia por parte del jugador y hacia el juego imposible . Un enemigo que solo te detecta cuando estás cerca genera tensión sin ser injusto.
¿Cómo se llevó a cabo su implementación?
Los cambios se realizaron en:
src/juego.cpp → función moverEnemigos(): se calculó la distancia entre el enemigo y el jugador. Si la distancia supera 8 celdas el fantasma no se mueve, simulando un rango de detección. Adicionalmente se le agregó un contador de turno para que se mueva cada 2 turnos en lugar de cada turno.

2.Pantallas de victoria y derrota con reinicio
¿Para qué se implementó?
Para mejorar la experiencia de usuario permitiendo jugar de nuevo sin tener que cerrar y ejecutar otra vez el programa.
¿Por qué se consideró necesario?
Un juego que se cierra al terminar obliga al jugador a volver a ejecutarlo manualmente, lo cual interrumpe la experiencia.
¿Cómo se llevó a cabo su implementación?
Los cambios se realizaron en:
src/juego.cpp → función correrJuego(): se convirtió en un loop externo que reinicia el estado completo del juego al presionar cualquier tecla en la pantalla final.
src/juego.cpp → función pantallaFin(): se modificó para retornar un booleano indicando si el jugador quiere jugar de nuevo o salir.
include/juego.h: se actualizó la declaración de pantallaFin() cambiando el tipo de retorno de void a bool.

3.Corrección de bugs críticos
¿Para qué se implementó?
Para garantizar un comportamiento correcto y justo del juego en todas las situaciones posibles del juego.
¿Por qué se consideró necesario?
Los bugs afectaban directamente la jugabilidad y la calidad del proyecto.
¿Cómo se llevó a cabo su implementación?
Se corrigieron tres bugs principales:
Spawn seguro tras daño → src/juego.cpp: se creó la función auxiliar aplicarDanio() que busca iterativamente la primera celda libre disponible en lugar de asumir que una posición fija siempre está libre.
Anti-apilamiento de enemigos → src/juego.cpp: se creó la función celdaOcupadaPorEnemigo() que verifica si la celda destino ya está ocupada por otro enemigo antes de realizar el movimiento.
Orden de colisiones → src/juego.cpp → función correrJuego(): se corrigió el game loop para detectar colisiones tanto después del movimiento del jugador como después del movimiento de los enemigos, eliminando el frame de gracia involuntario.
