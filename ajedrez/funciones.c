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


void gestionarEntradas(Partida* p)
{
    SDL_Event evento;
    while(SDL_PollEvent(&evento))
    {
        switch(evento.type)
        {
        case SDL_QUIT:
            p->estado = ESTADO_SALIR;
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


void dibujarPieza(SDL_Renderer* rend, Pieza* pieza)
{
    SDL_Rect orig;  //Tamaño de textura inicial
    orig.x = pieza->pos.x;
    orig.y = pieza->pos.y;

    SDL_QueryTexture(pieza->textura, NULL, NULL, &orig.w, &orig.h); //Devuelve el tamaño original de la textura

    SDL_Rect dest;   //Tamaño de textura deseado
    dest.x = orig.x - orig.w/2 - 5;
    dest.y = orig.y - orig.h/2 - 5;
    dest.w = orig.w;
    dest.h = orig.h;
    dest.w = orig.w + REESCALADO_TEXTURA_PIEZA_X;
    dest.h = orig.h + REESCALADO_TEXTURA_PIEZA_Y;

    SDL_RenderCopy(rend, pieza->textura, NULL, &dest);
}


void dibujarPiezas(SDL_Renderer* rend, Pieza* piezas_jugador_1, Pieza* piezas_jugador_2)
{
    int index = 0;

    for(index = 0; index < PIEZAS_INICIALES; index++)
    {
        dibujarPieza(rend, &piezas_jugador_1[index]);
        dibujarPieza(rend, &piezas_jugador_2[index]);
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

    int fila = 0;
    int columna = 0;

    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            tablero->casillas[fila][columna].pos.x = calculoPosicionCasilla(fila, columna).x; //Asigno la posicion correspondiente a cada casilla
            tablero->casillas[fila][columna].pos.y = calculoPosicionCasilla(fila, columna).y; //para cada eje
            tablero->casillas[fila][columna].pieza = 'v';
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

    int fila = 0;
    int columna = 0;
    int num_pieza = 0;

    for(fila = 0; fila < 2; fila++) //Solo quiero rellenar las dos primeras filas
    {
        for(columna = 0; columna < N; columna++)
        {
            jugador_2->piezas[num_pieza].pos.x = tablero->casillas[fila][columna].pos.x; //Uso la posición de las casillas que previamente he calculado
            jugador_2->piezas[num_pieza].pos.y = tablero->casillas[fila][columna].pos.y;
            switch(num_pieza)
            {
            case 0:
            case 7:
                jugador_2->piezas[num_pieza].tipo = TORRE;
                jugador_2->piezas[num_pieza].textura = texturas_2[2];
                tablero->casillas[fila][columna].pieza = 't';
                break;
            case 1:
            case 6:
                jugador_2->piezas[num_pieza].tipo = CABALLO;
                jugador_2->piezas[num_pieza].textura = texturas_2[1];
                tablero->casillas[fila][columna].pieza = 'c';
                break;
            case 2:
            case 5:
                jugador_2->piezas[num_pieza].tipo = ALFIL;
                jugador_2->piezas[num_pieza].textura = texturas_2[3];
                tablero->casillas[fila][columna].pieza = 'a';
                break;
            case 3:
                jugador_2->piezas[num_pieza].tipo = DAMA;
                jugador_2->piezas[num_pieza].textura = texturas_2[4];
                tablero->casillas[fila][columna].pieza = 'd';
                break;
            case 4:
                jugador_2->piezas[num_pieza].tipo = REY;
                jugador_2->piezas[num_pieza].textura = texturas_2[5];
                tablero->casillas[fila][columna].pieza = 'r';
                break;
            default:
                jugador_2->piezas[num_pieza].tipo = PEON; //Segunda fila (todo PEONES) es cuando: num_pieza > 7
                jugador_2->piezas[num_pieza].textura = texturas_2[0];
                tablero->casillas[fila][columna].pieza = 'p';
            }
            num_pieza++; //Cada vez que avanzo hacia la derecha de la matriz incrementa su valor
        }
    }

    fila = 6;       //Ultima fila
    columna = 0;    //Ultima columna
    num_pieza = 0;  //Uso la misma variable como indice

    for(fila = 6; fila < N; fila++) //Solo quiero rellenar las dos ultimas filas
    {
        for(columna = 0; columna < N; columna++) //Invierto el sentido para reutilizar el codigo del switch
        {
            jugador_1->piezas[num_pieza].pos.x = tablero->casillas[fila][columna].pos.x; //Uso la posición de las casillas que previamente he calculado
            jugador_1->piezas[num_pieza].pos.y = tablero->casillas[fila][columna].pos.y;
            switch(num_pieza)
            {
            case 8:
            case 15:
                jugador_1->piezas[num_pieza].tipo = TORRE;
                jugador_1->piezas[num_pieza].textura = texturas_1[2];
                tablero->casillas[fila][columna].pieza = 'T';
                break;
            case 9:
            case 14:
                jugador_1->piezas[num_pieza].tipo = CABALLO;
                jugador_1->piezas[num_pieza].textura = texturas_1[1];
                tablero->casillas[fila][columna].pieza = 'C';
                break;
            case 10:
            case 13:
                jugador_1->piezas[num_pieza].tipo = ALFIL;
                jugador_1->piezas[num_pieza].textura = texturas_1[3];
                tablero->casillas[fila][columna].pieza = 'A';
                break;
            case 11:
                jugador_1->piezas[num_pieza].tipo = REY;
                jugador_1->piezas[num_pieza].textura = texturas_1[5];
                tablero->casillas[fila][columna].pieza = 'R';
                break;
            case 12:
                jugador_1->piezas[num_pieza].tipo = DAMA;
                jugador_1->piezas[num_pieza].textura = texturas_1[4];
                tablero->casillas[fila][columna].pieza = 'D';
                break;
            default:
                jugador_1->piezas[num_pieza].tipo = PEON; //Segunda fila (todo PEONES) es cuando: num_pieza > 7
                jugador_1->piezas[num_pieza].textura = texturas_1[0];
                tablero->casillas[fila][columna].pieza = 'P';
            }
            num_pieza++; //Cada vez que avanzo hacia la derecha de la matriz incrementa su valor
        }
    }

    fila = 0;
    columna = 0;
    num_pieza = 0;

    printf("\n");
    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            printf("%c, ", tablero->casillas[fila][columna].pieza);
        }
        printf("\n");
    }
}


void inicializarPartida(App* app, Partida* p)   //Reinicia la partida
{
    p->estado = ESTADO_INICIO_PARTIDA;

    inicializarTablero(app, &p->tablero);

    inicializarPiezas(app, &p->jugador_1, &p->jugador_2, &p->tablero);
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
