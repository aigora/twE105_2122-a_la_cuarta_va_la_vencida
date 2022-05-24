#include "funciones.h"

void iniciarSDL(App* app)
{
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); //Permite importar imagenes con esos formatos

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error al intentar inicializar SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app->window = SDL_CreateWindow("Ajedrez", //Creamos la ventana
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  SCREEN_WITDH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
    if (!(app->window))
    {
        printf("Error al intentar crear la ventana: %s\n", SDL_GetError());
        exit(1);
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, //Creamos el renderer para la ventana
                                      SDL_RENDERER_ACCELERATED |
                                      SDL_RENDERER_PRESENTVSYNC);
    if (!(app->renderer))
    {
        SDL_DestroyWindow(app->window);
        printf("Error al intentar crear el renderer: %s\n", SDL_GetError());
        exit(-1);
    }
}


void prepararEscena(App* app)
{
    SDL_SetRenderDrawColor(app->renderer, 96, 150, 50, 255);
    SDL_RenderClear(app->renderer);
}


void presentarEscena(App* app)
{
    SDL_RenderPresent(app->renderer);
}

Vector2D calcularCasillaClick(Vector2D pos_mouse, Vector2D tamano_casilla)
{
    Vector2D fila_columna;
    fila_columna.y = pos_mouse.y / tamano_casilla.y;
    fila_columna.x = pos_mouse.x / tamano_casilla.x;
    return fila_columna;
}

_Bool clickPiezaPropia(char tipo_pieza)
{

    if(tipo_pieza != '0')
        return 1;
    else
        return 0;
}

_Bool clickCasillaLibre(char tipo_pieza)
{
    if(tipo_pieza == '0')
    {
        printf("Casilla LIBRE\n");
        return 1;
    }
    else
    {
        printf("Casilla OCUPADA\n");
        return 0;
    }
}

void cambiarTurno(Partida* p)
{
    if(p->turno == TURNO_JUGADOR_1)
    {
        p->turno = TURNO_JUGADOR_2;
    }
    else if(p->turno == TURNO_JUGADOR_2)
    {
        p->turno = TURNO_JUGADOR_1;
    }
}

void moverPieza(Pieza piezas[][N], Casilla* casilla)
{
    int fila = 0;
    int columna = 0;

    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            if(piezas[fila][columna].en_movimiento) //Buscamos la pieza que el jugador está moviendo
            {
                //La pieza se mueve a la casilla seleccionada
                piezas[fila][columna].pos.x = casilla->pos.x;
                piezas[fila][columna].pos.y = casilla->pos.y;
                //La pieza deja de moverse
                piezas[fila][columna].en_movimiento = 0;
                //Asigno a la casilla el tipo de pieza nuevo
                casilla->pieza.tipo = piezas[fila][columna].tipo;
            }
        }
    }
    //printf("\nSe ha  movido la pieza\n");
    //printf("POSICION PIEZA: %i, %i\tPOSICION CASILLA: %i, %i", pos_pieza->x, pos_pieza->y, pos_casilla.x, pos_casilla.y);
}

void jugarTurno(Jugador* jugador, Tablero* tablero, Vector2D pos_mouse, Vector2D casilla)
{
    Pieza* pieza = &jugador->piezas[casilla.y][casilla.x];
    Casilla* cas = &tablero->casillas[casilla.y][casilla.x];

    if(jugador->moviendo_pieza == 0 && clickPiezaPropia(pieza->tipo))
    {
        printf("\nMoviendo pieza\n");
        jugador->moviendo_pieza = 1;
        jugador->piezas[casilla.y][casilla.x].en_movimiento = 1;
        printf("\nPIEZA EN MOVIMIENTO: %i\n", pieza->en_movimiento);
    }

    else if(jugador->moviendo_pieza == 1 && clickCasillaLibre(pieza->tipo))
    {
        moverPieza(jugador->piezas, cas);
        jugador->moviendo_pieza = 0;
        //jugador->soltar_pieza = 1;
        printf("\nPIEZA MOVIDA\n");
    }

    //PRUEBA
    int fila = 0;
    int columna = 0;

    printf("\n");
    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            printf("%c, ", tablero->casillas[fila][columna].pieza.tipo);
        }
        printf("\n");
    }

    fila = 0;
    columna = 0;

    printf("\n");
    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            printf("%c, ", jugador->piezas[fila][columna].tipo);
        }
        printf("\n");
    }
}

void gestionarClick(Partida* p, Vector2D pos_mouse, Vector2D casilla)
{
    if(p->estado == ESTADO_JUGANDO)
    {
        if(p->turno == TURNO_JUGADOR_1)
        {
            jugarTurno(&p->jugador_1, &p->tablero, pos_mouse, casilla);
        }
        else if(p->turno == TURNO_JUGADOR_2)
        {
            jugarTurno(&p->jugador_2, &p->tablero, pos_mouse, casilla);
        }
    }
    else
    {
        //reiniciarPartida(p);
    }
}

void gestionarEntradas(Partida* p)
{
    SDL_Event evento;
    Vector2D pos_mouse;
    Vector2D casilla;   //fila(x) y columna(y) de la casilla clickada

    while(SDL_PollEvent(&evento))
    {
        switch(evento.type)
        {
        case SDL_QUIT:
            p->estado = ESTADO_SALIR;
            break;
        case SDL_MOUSEBUTTONDOWN:
            pos_mouse.x = evento.button.x;
            pos_mouse.y = evento.button.y;
            casilla.x = calcularCasillaClick(pos_mouse, p->tablero.tamano_casillas).x;
            casilla.y = calcularCasillaClick(pos_mouse, p->tablero.tamano_casillas).y;

            gestionarClick(p, pos_mouse, casilla);
            break;
        default:
            break;
        }
    }
}

SDL_Texture* cargarTextura(App* app, char* nombre_fichero)
{
    SDL_Texture* textura;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Cargando %s", nombre_fichero);
    textura = IMG_LoadTexture(app->renderer, nombre_fichero);
    return textura;
}


void dibujarTablero(SDL_Renderer* rend, SDL_Texture* textura, Vector2D pos)
{
    SDL_Rect dest;

    dest.x = pos.x;
    dest.y = pos.y;
    dest.w = SCREEN_WITDH;  //Queremos que ocupe toda la ventana
    dest.h = SCREEN_HEIGHT;

    SDL_RenderCopy(rend, textura, NULL, &dest);    //Dibuja la textura del tablero con la informacion de rect
}

void dibujarPiezaEnMovimiento(SDL_Renderer* rend, Pieza pieza)
{
    int w, h, mouseX, mouseY;
	SDL_QueryTexture(pieza.textura, NULL, NULL, &w, &h);

	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_Rect orig, dest;
	orig.w = w;
	orig.h = h;

    dest.w = orig.w;
    dest.h = orig.h;
    dest.w = orig.w + REESCALADO_TEXTURA_PIEZA_X;
    dest.h = orig.h + REESCALADO_TEXTURA_PIEZA_Y;

	dest.x = mouseX - dest.w/2;
	dest.y = mouseY - dest.h/2;

    SDL_RenderCopy(rend, pieza.textura, NULL, &dest);
}

void dibujarPieza(SDL_Renderer* rend, Pieza pieza)
{
    SDL_Rect orig;  //Tamaño de textura inicial
    SDL_Rect dest;   //Tamaño de textura deseado
    if(pieza.tipo != '0')
    {
        if(pieza.en_movimiento == 1)
        {
            dibujarPiezaEnMovimiento(rend, pieza);
            printf("Chupadita\n");
        }
        else
        {
            orig.x = pieza.pos.x;
            orig.y = pieza.pos.y;

            SDL_QueryTexture(pieza.textura, NULL, NULL, &orig.w, &orig.h); //Devuelve el tamaño original de la textura

            dest.x = orig.x - orig.w/2 - 5;
            dest.y = orig.y - orig.h/2 - 5;
            dest.w = orig.w;
            dest.h = orig.h;
            dest.w = orig.w + REESCALADO_TEXTURA_PIEZA_X;
            dest.h = orig.h + REESCALADO_TEXTURA_PIEZA_Y;

            SDL_RenderCopy(rend, pieza.textura, NULL, &dest);
        }
    }
}


void dibujarPiezas(SDL_Renderer* rend, Pieza piezas_jugador_1[][N], Pieza piezas_jugador_2[][N])
{
    int fila = 0;
    int columna = 0;

    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            dibujarPieza(rend, piezas_jugador_1[fila][columna]);
            dibujarPieza(rend, piezas_jugador_2[fila][columna]);
        }
    }
}


void dibujar(SDL_Renderer* rend, Partida* p)
{
    dibujarTablero(rend, p->tablero.textura, p->tablero.pos);
    dibujarPiezas(rend, p->jugador_1.piezas, p->jugador_2.piezas);
}


Vector2D calculoPosicionCasilla(int fila, int columna) //Calcula la posicion x,y de cada casilla
{
    Vector2D pos_casilla;
    pos_casilla.x = (SCREEN_WITDH / (N * 2)) + ((SCREEN_WITDH / N) * columna);  //Posición de referencia(primera casilla) + distancia a la siguiente casilla
    pos_casilla.y = (SCREEN_WITDH / (N * 2)) + ((SCREEN_HEIGHT / N) * fila);
    return pos_casilla;
}


void inicializarTablero(App* app, Tablero* tablero)
{
    tablero->pos.x = 0;
    tablero->pos.y = 0;
    tablero->textura = cargarTextura(app, "texturas/tablero.png");
    tablero->tamano_casillas.x = SCREEN_WITDH / N;
    tablero->tamano_casillas.y = SCREEN_HEIGHT / N;

    int fila = 0;
    int columna = 0;

    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            tablero->casillas[fila][columna].pos.x = calculoPosicionCasilla(fila, columna).x; //Asigno la posicion correspondiente a cada casilla
            tablero->casillas[fila][columna].pos.y = calculoPosicionCasilla(fila, columna).y; //para cada eje
            tablero->casillas[fila][columna].pieza.tipo = '0'; //Vacio
        }
    }
}


void inicializarPiezas(App* app, Jugador* jugador_1, Jugador* jugador_2, Tablero* tablero)
{
    SDL_Texture* texturas_2[6];
    texturas_2[0] = cargarTextura(app, "texturas/peon_dorado.png");    //Peon dorado
    texturas_2[1] = cargarTextura(app, "texturas/caballo_dorado.png"); //Caballo dorado
    texturas_2[2] = cargarTextura(app, "texturas/torre_dorada.png");   //Torre dorada
    texturas_2[3] = cargarTextura(app, "texturas/alfil_dorado.png");   //Alfil dorado
    texturas_2[4] = cargarTextura(app, "texturas/dama_dorada.png");    //Dama dorada
    texturas_2[5] = cargarTextura(app, "texturas/rey_dorado.png");     //Rey dorado

    SDL_Texture* texturas_1[6];
    texturas_1[0] = cargarTextura(app, "texturas/peon_blanco.png");    //Peon blanco
    texturas_1[1] = cargarTextura(app, "texturas/caballo_blanco.png"); //Caballo blanco
    texturas_1[2] = cargarTextura(app, "texturas/torre_blanca.png");   //Torre blanca
    texturas_1[3] = cargarTextura(app, "texturas/alfil_blanco.png");   //Alfil blanco
    texturas_1[4] = cargarTextura(app, "texturas/dama_blanca.png");    //Dama blanca
    texturas_1[5] = cargarTextura(app, "texturas/rey_blanco.png");     //Rey blanco

    jugador_1->moviendo_pieza = 0;
    jugador_1->soltar_pieza = 0;
    jugador_2->moviendo_pieza = 0;
    jugador_2->soltar_pieza = 0;

    int fila = 0;
    int columna = 0;

    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            jugador_1->piezas[fila][columna].tipo = '0';
            jugador_1->piezas[fila][columna].en_movimiento = 0;
            jugador_2->piezas[fila][columna].tipo = '0';
            jugador_2->piezas[fila][columna].en_movimiento = 0;
        }
    }

    fila = 0;
    columna = 0;
    int num_pieza = 0;

    for(fila = 0; fila < 2; fila++) //Solo quiero rellenar las dos primeras filas
    {
        for(columna = 0; columna < N; columna++)
        {
            jugador_2->piezas[fila][columna].pos.x = tablero->casillas[fila][columna].pos.x; //Uso la posición de las casillas que previamente he calculado
            jugador_2->piezas[fila][columna].pos.y = tablero->casillas[fila][columna].pos.y;
            switch(num_pieza)
            {
            case 0:
            case 7:
                jugador_2->piezas[fila][columna].tipo = 't';
                jugador_2->piezas[fila][columna].textura = texturas_2[2];
                tablero->casillas[fila][columna].pieza.tipo = jugador_2->piezas[fila][columna].tipo;
                break;
            case 1:
            case 6:
                jugador_2->piezas[fila][columna].tipo = 'c';
                jugador_2->piezas[fila][columna].textura = texturas_2[1];
                tablero->casillas[fila][columna].pieza.tipo = jugador_2->piezas[fila][columna].tipo;
                break;
            case 2:
            case 5:
                jugador_2->piezas[fila][columna].tipo = 'a';
                jugador_2->piezas[fila][columna].textura = texturas_2[3];
                tablero->casillas[fila][columna].pieza.tipo = jugador_2->piezas[fila][columna].tipo;
                break;
            case 3:
                jugador_2->piezas[fila][columna].tipo = 'd';
                jugador_2->piezas[fila][columna].textura = texturas_2[4];
                tablero->casillas[fila][columna].pieza.tipo = jugador_2->piezas[fila][columna].tipo;
                break;
            case 4:
                jugador_2->piezas[fila][columna].tipo = 'r';
                jugador_2->piezas[fila][columna].textura = texturas_2[5];
                tablero->casillas[fila][columna].pieza.tipo = jugador_2->piezas[fila][columna].tipo;
                break;
            default:
                jugador_2->piezas[fila][columna].tipo = 'p'; //Segunda fila (todo PEONES) es cuando: num_pieza > 7
                jugador_2->piezas[fila][columna].textura = texturas_2[0];
                tablero->casillas[fila][columna].pieza.tipo = jugador_2->piezas[fila][columna].tipo;
            }
            num_pieza++; //Cada vez que avanzo hacia la derecha de la matriz incrementa su valor
        }
    }

    fila = 6;       //Ultima fila
    columna = 0;    //Ultima columna
    num_pieza = 0;

    for(fila = 6; fila < N; fila++) //Solo quiero rellenar las dos ultimas filas
    {
        for(columna = 0; columna < N; columna++) //Invierto el sentido para reutilizar el codigo del switch
        {
            jugador_1->piezas[fila][columna].pos.x = tablero->casillas[fila][columna].pos.x; //Uso la posición de las casillas que previamente he calculado
            jugador_1->piezas[fila][columna].pos.y = tablero->casillas[fila][columna].pos.y;
            switch(num_pieza)
            {
            case 8:
            case 15:
                jugador_1->piezas[fila][columna].tipo = 'T';
                jugador_1->piezas[fila][columna].textura = texturas_1[2];
                tablero->casillas[fila][columna].pieza.tipo = jugador_1->piezas[fila][columna].tipo;
                break;
            case 9:
            case 14:
                jugador_1->piezas[fila][columna].tipo = 'C';
                jugador_1->piezas[fila][columna].textura = texturas_1[1];
                tablero->casillas[fila][columna].pieza.tipo = jugador_1->piezas[fila][columna].tipo;
                break;
            case 10:
            case 13:
                jugador_1->piezas[fila][columna].tipo = 'A';
                jugador_1->piezas[fila][columna].textura = texturas_1[3];
                tablero->casillas[fila][columna].pieza.tipo = jugador_1->piezas[fila][columna].tipo;
                break;
            case 11:
                jugador_1->piezas[fila][columna].tipo = 'R';
                jugador_1->piezas[fila][columna].textura = texturas_1[5];
                tablero->casillas[fila][columna].pieza.tipo = jugador_1->piezas[fila][columna].tipo;
                break;
            case 12:
                jugador_1->piezas[fila][columna].tipo = 'D';
                jugador_1->piezas[fila][columna].textura = texturas_1[4];
                tablero->casillas[fila][columna].pieza.tipo = jugador_1->piezas[fila][columna].tipo;
                break;
            default:
                jugador_1->piezas[fila][columna].tipo = 'P';
                jugador_1->piezas[fila][columna].textura = texturas_1[0];
                tablero->casillas[fila][columna].pieza.tipo = jugador_1->piezas[fila][columna].tipo;
            }
            num_pieza++;
        }
    }

    fila = 0;
    columna = 0;

    printf("\n");
    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            printf("%c, ", tablero->casillas[fila][columna].pieza.tipo);
        }
        printf("\n");
    }

    fila = 0;
    columna = 0;

    printf("\n");
    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            printf("%c, ", jugador_1->piezas[fila][columna].tipo);
        }
        printf("\n");
    }

    fila = 0;
    columna = 0;

    printf("\n");
    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            printf("%c, ", jugador_2->piezas[fila][columna].tipo);
        }
        printf("\n");
    }
}


void inicializarPartida(App* app, Partida* p)   //Reinicia la partida
{
    p->estado = ESTADO_INICIO_PARTIDA;
    p->turno = TURNO_JUGADOR_1;

    inicializarTablero(app, &p->tablero);
    inicializarPiezas(app, &p->jugador_1, &p->jugador_2, &p->tablero);

    p->estado = ESTADO_JUGANDO;
}


void humanoVShumano(char nombre_1[], char nombre_2[])
{
    App app;
    iniciarSDL(&app);

    Partida p;
    inicializarPartida(&app, &p);

    printf("\nEstado de partida: %i\n", p.estado);

    do
    {
        prepararEscena(&app);

        gestionarEntradas(&p);
        dibujar(app.renderer, &p);

        presentarEscena(&app);

        SDL_Delay(16);
    }
    while(p.estado != ESTADO_SALIR);

    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

//void humanoVSmaquina(){}
