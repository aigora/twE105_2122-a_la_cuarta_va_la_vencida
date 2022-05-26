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


Vector2D calcularCasillaClick(Vector2D tamano_casilla)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY); //Obtengo la posición del puntero

    Vector2D fila_columna;
    fila_columna.y = mouseY / tamano_casilla.y;
    fila_columna.x = mouseX / tamano_casilla.x;
    return fila_columna;
}


_Bool clickCasillaInicial(char tipo1, char tipo2)
{
    if(tipo1 == tipo2) //Mismo tipo = misma casilla final e inicial
        return 1;
    else
        return 0;
}


int clickPieza(char tipo_pieza, int turno)
{
    int tipo; //0 vacio, 1 pieza propia, 2 pieza rival

    if(tipo_pieza != '0')   //Hay una pieza
    {
        if(turno == TURNO_JUGADOR_1)    //Pertence al jugador 1
        {
            if(tipo_pieza == 'R' || tipo_pieza == 'D' || tipo_pieza == 'C' ||
               tipo_pieza == 'T' || tipo_pieza == 'A' || tipo_pieza == 'P')
            {
                tipo = 1;
            }
            else
            {
                printf("\n\n Esa pieza no es tuya!\n");
                tipo = 2;
            }
        }
        else  //Pertenece al jugador 2
        {
            if(tipo_pieza == 'r' || tipo_pieza == 'd' || tipo_pieza == 'c' ||
               tipo_pieza == 't' || tipo_pieza == 'a' || tipo_pieza == 'p')
            {
                tipo = 1;
            }
            else
            {
                printf("\n\n Esa pieza no es tuya!\n");
                tipo = 2;
            }
        }
    }
    else    //Casilla vacía
    {
        tipo = 0;
    }

    return tipo;
}

_Bool clickCasillaLibre(char tipo_pieza)
{
    if(tipo_pieza == '0')
    {
        //printf("Casilla LIBRE\n");
        return 1;
    }
    else
    {
        //printf("Casilla OCUPADA\n");
        return 0;
    }
}

void cambiarTurno(Partida* p)
{
    if(p->turno == TURNO_JUGADOR_1)
    {
        p->turno = TURNO_JUGADOR_2;
        printf("\n\n - Turno de: %s", p->jugador_2.nombre);
    }
    else if(p->turno == TURNO_JUGADOR_2)
    {
        p->turno = TURNO_JUGADOR_1;
        printf("\n\n - Turno de: %s", p->jugador_1.nombre);
    }
}

void moverPieza(Pieza* p_inicial, Pieza* p_final)
{
        //Realmente no se mueve la pieza, más bien se intercambia el tipo de pieza entre casillas
    p_inicial->en_movimiento = 0; //Flag para que deje de seguir el ratón

    p_final->textura = p_inicial->textura; //Le paso la textura
    p_final->tipo = p_inicial->tipo; //Le paso "la pieza"

    p_inicial->tipo = '0'; //Vacío la casilla inicial
    //printf("POSICION PIEZA: %i, %i\tPOSICION CASILLA: %i, %i", pos_pieza->x, pos_pieza->y, pos_casilla.x, pos_casilla.y);
}


int calcularSentido(int dir, Vector2D desp)
{
    int sentido;

    if(dir == VERTICAL)
    {
        if(desp.y < 0)
            sentido = ARRIBA;
        else //desp.y < 0
            sentido = ABAJO;
    }
    else if(dir == HORIZONTAL)
    {
        if(desp.x > 0)
            sentido = DERECHA;
        else
            sentido = IZQUIERDA;
    }

    printf("\n\nSentido: %i", sentido);
    return sentido;
}


int calcularDireccion(Vector2D desp)
{
    int direccion;

    if(desp.x == 0 && desp.y != 0)
        direccion = VERTICAL;
    else if(desp.x != 0 && desp.y == 0)
        direccion = HORIZONTAL;
    else if(desp.x != 0 && desp.y != 0 && abs(desp.x/desp.y) == 1)
        direccion = DIAGONAL;
    else
        direccion = NO_PERMITIDO;

    printf("\n\nDireccion: %i", direccion);
    return direccion;
}


Vector2D calcularDesplazamiento(Vector2D origen, Vector2D destino)
{
    Vector2D desplazamiento;
    int tamano_casilla = SCREEN_WITDH / N;

    desplazamiento.x = (destino.x - origen.x) / tamano_casilla;
    desplazamiento.y = (destino.y - origen.y) / tamano_casilla;

    printf("\n\nDesplazamiento: (%i, %i)", desplazamiento.x, desplazamiento.y);
    return desplazamiento;
}


int calcularMovimientoLegal(Casilla c[][N], Casilla c_origen, Casilla c_destino)
{
    Movimiento m;

    m.casillas.x = calcularDesplazamiento(c_origen.pos, c_destino.pos).x; //Numero de casillas que se desplaza la pieza
    m.casillas.y = calcularDesplazamiento(c_origen.pos, c_destino.pos).y;

    m.direccion = calcularDireccion(m.casillas);

    m.sentido = calcularSentido(m.direccion, m.casillas);

    //m.salto = calcularSaltoMovimiento();

    if(c_origen.pieza.tipo == 'P')
    {
        if(m.direccion == VERTICAL && m.sentido == ARRIBA)
        {
            if(c_origen.pieza.primer_movimiento == 1 && (abs(m.casillas.y) == UNA_CASILLA || abs(m.casillas.y) == DOS_CASILLAS))
                m.legalidad = 1;
            else if(c_origen.pieza.primer_movimiento == 0 && m.casillas.y == UNA_CASILLA)
                m.legalidad = 1;
            else
                m.legalidad = 0;
        }
        else
            m.legalidad = 0;
    }
    else if(c_origen.pieza.tipo == 'T')
    {
        //...
    }

    return m.legalidad;
}


void jugarTurno(Partida* p, int columna_casilla, int fila_casilla)
{
    Casilla* casilla_click = &p->tablero.casillas[columna_casilla][fila_casilla]; //pieza que hay en la casilla en el momento del click
    Jugador* jugador;

    if(p->turno == TURNO_JUGADOR_1)
        jugador = &p->jugador_1;
    else
        jugador = &p->jugador_2;

    int movimiento_legal = calcularMovimientoLegal(p->tablero.casillas, *jugador->casilla_movimiento, *casilla_click);
    int click_pieza = clickPieza(casilla_click->pieza.tipo, p->turno); //0 no hay, 1 es propia, 2 es del rival
    int click_casilla_inicial = clickCasillaInicial(jugador->casilla_movimiento->pieza.tipo, casilla_click->pieza.tipo);

    if(jugador->moviendo_pieza == 0 && click_pieza == 1)   //No hay piezas en movimiento y se toca una pieza del propio jugador
    {
        jugador->moviendo_pieza = 1;
        casilla_click->pieza.en_movimiento = 1;
        jugador->casilla_movimiento = casilla_click; //Almaceno la dirección de la pieza en movimiento
    }
    else if(jugador->moviendo_pieza == 1 && click_casilla_inicial)
    {
        jugador->casilla_movimiento->pieza.en_movimiento = 0;
        jugador->moviendo_pieza = 0;
    }
    /*else if(jugador->moviendo_pieza == 1 && (clickCasillaLibre(casilla->pieza.tipo)) //Si hay una pieza en movimiento y se toca una casilla libre
    {
        moverPieza(&jugador->casilla_movimiento->pieza, &casilla->pieza); //La pieza "se mueve"
        jugador->moviendo_pieza = 0;
        cambiarTurno(p);
    }*/
        //Prueba de movimientos legales
    else if(jugador->moviendo_pieza == 1 && movimiento_legal) //Si hay una pieza en movimiento y se toca una casilla libre
    {
        moverPieza(&jugador->casilla_movimiento->pieza, &casilla_click->pieza); //La pieza "se mueve"
        jugador->moviendo_pieza = 0;
        printf("\n\n\nHOLA");
        cambiarTurno(p);
    }

}


void gestionarClick(Partida* p, int columna, int fila)
{
    if(p->estado == ESTADO_INICIO)
    {
        p->estado = ESTADO_JUGANDO;
        printf("\n\n\n\n\n\tLa partida ha comenzado!");
        printf("\n\n - Turno de: %s", p->jugador_1.nombre);
    }

    if(p->estado == ESTADO_JUGANDO)
    {
        jugarTurno(p, columna, fila);
    }
    else
    {
        //reiniciarPartida(p);
    }
}

void gestionarEntradas(Partida* p)
{
    SDL_Event evento;
    int fila_click, columna_click;

    while(SDL_PollEvent(&evento))
    {
        switch(evento.type)
        {
        case SDL_QUIT:
            p->estado = ESTADO_SALIR;
            break;
        case SDL_MOUSEBUTTONDOWN:
            columna_click = calcularCasillaClick(p->tablero.tamano_casillas).x; //Columna y fila de la casilla donde tiene lugar el click
            fila_click = calcularCasillaClick(p->tablero.tamano_casillas).y;
            gestionarClick(p, columna_click, fila_click);
            break;
        default:
            break;
        }
    }
}


SDL_Texture* cargarTextura(App* app, char* nombre_fichero)
{
    SDL_Texture* textura;
    //SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Cargando %s", nombre_fichero);
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


void dibujarPiezaEnMovimiento(SDL_Renderer* rend, Casilla c)
{
    int w, h, mouseX, mouseY;
	SDL_QueryTexture(c.pieza.textura, NULL, NULL, &w, &h);

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

    SDL_RenderCopy(rend, c.pieza.textura, NULL, &dest);
}


void dibujarPieza(SDL_Renderer* rend, Casilla c)
{
    SDL_Rect orig;  //Tamaño de textura inicial
    SDL_Rect dest;   //Tamaño de textura deseado
    if(c.pieza.tipo != '0')
    {
        if(c.pieza.en_movimiento == 1)
        {
            dibujarPiezaEnMovimiento(rend, c);
        }
        else
        {
            orig.x = c.pos.x;
            orig.y = c.pos.y;

            SDL_QueryTexture(c.pieza.textura, NULL, NULL, &orig.w, &orig.h); //Devuelve el tamaño original de la textura

            dest.x = orig.x - orig.w/2 - 5;
            dest.y = orig.y - orig.h/2 - 5;
            dest.w = orig.w;
            dest.h = orig.h;
            dest.w = orig.w + REESCALADO_TEXTURA_PIEZA_X;
            dest.h = orig.h + REESCALADO_TEXTURA_PIEZA_Y;

            SDL_RenderCopy(rend, c.pieza.textura, NULL, &dest);
        }
    }
}


void dibujarPiezas(SDL_Renderer* rend, Casilla casillas[][N])
{
    int fila = 0;
    int columna = 0;

    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            dibujarPieza(rend, casillas[fila][columna]);
        }
    }
}


void dibujar(SDL_Renderer* rend, Tablero tablero)
{
    dibujarTablero(rend, tablero.textura, tablero.pos);
    dibujarPiezas(rend, tablero.casillas);
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
        //Cargamos las texturas de las piezas doradas
    SDL_Texture* texturas_2[6];
    texturas_2[0] = cargarTextura(app, "texturas/peon_dorado.png");    //Peon dorado
    texturas_2[1] = cargarTextura(app, "texturas/caballo_dorado.png"); //Caballo dorado
    texturas_2[2] = cargarTextura(app, "texturas/torre_dorada.png");   //Torre dorada
    texturas_2[3] = cargarTextura(app, "texturas/alfil_dorado.png");   //Alfil dorado
    texturas_2[4] = cargarTextura(app, "texturas/dama_dorada.png");    //Dama dorada
    texturas_2[5] = cargarTextura(app, "texturas/rey_dorado.png");     //Rey dorado

        //Cargamos las texturas de las piezas blancas
    SDL_Texture* texturas_1[6];
    texturas_1[0] = cargarTextura(app, "texturas/peon_blanco.png");    //Peon blanco
    texturas_1[1] = cargarTextura(app, "texturas/caballo_blanco.png"); //Caballo blanco
    texturas_1[2] = cargarTextura(app, "texturas/torre_blanca.png");   //Torre blanca
    texturas_1[3] = cargarTextura(app, "texturas/alfil_blanco.png");   //Alfil blanco
    texturas_1[4] = cargarTextura(app, "texturas/dama_blanca.png");    //Dama blanca
    texturas_1[5] = cargarTextura(app, "texturas/rey_blanco.png");     //Rey blanco

        //Cargamos la textura del tablero e inicializamos sus parámetros
    tablero->pos.x = 0;
    tablero->pos.y = 0;
    tablero->textura = cargarTextura(app, "texturas/tablero.png");
    tablero->tamano_casillas.x = SCREEN_WITDH / N;
    tablero->tamano_casillas.y = SCREEN_HEIGHT / N;

        //Asignamos la posicion real de cada casilla
    int fila = 0;
    int columna = 0;

    for(fila = 0; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            tablero->casillas[columna][fila].pos.x = calculoPosicionCasilla(fila, columna).x; //Posicion de cada casilla
            tablero->casillas[columna][fila].pos.y = calculoPosicionCasilla(fila, columna).y;
            tablero->casillas[columna][fila].pieza.en_movimiento = 0; //Ninguna pieza se mueve
            tablero->casillas[columna][fila].pieza.tipo = '0'; //El tablero no tiene piezas todavía
            tablero->casillas[columna][fila].pieza.primer_movimiento = 0; //La unica pieza que necesita este flag es el peon
        }
    }

    fila = 0;
    columna = 0;
    int num_pieza = 0;

        //"Colocamos" las piezas doradas
    for(fila = 0; fila < 2; fila++)
    {
        for(columna = 0; columna < N; columna++)
        {
            switch(num_pieza)
            {
            case 0:
            case 7:
                tablero->casillas[columna][fila].pieza.tipo = 't';
                tablero->casillas[columna][fila].pieza.textura = texturas_2[2];
                break;
            case 1:
            case 6:
                tablero->casillas[columna][fila].pieza.tipo = 'c';
                tablero->casillas[columna][fila].pieza.textura = texturas_2[1];
                break;
            case 2:
            case 5:
                tablero->casillas[columna][fila].pieza.tipo = 'a';
                tablero->casillas[columna][fila].pieza.textura = texturas_2[3];
                break;
            case 3:
                tablero->casillas[columna][fila].pieza.tipo = 'd';
                tablero->casillas[columna][fila].pieza.textura = texturas_2[4];
                break;
            case 4:
                tablero->casillas[columna][fila].pieza.tipo = 'r';
                tablero->casillas[columna][fila].pieza.textura = texturas_2[5];
                break;
            default:
                tablero->casillas[columna][fila].pieza.tipo = 'p'; //Segunda fila (todo PEONES) es cuando: num_pieza > 7
                tablero->casillas[columna][fila].pieza.textura = texturas_2[0];
                tablero->casillas[columna][fila].pieza.primer_movimiento = 1;
            }
            num_pieza++; //Cada vez que avanzo hacia la derecha de la matriz incrementa su valor
        }
    }

    fila = 6;
    columna = 0;
    num_pieza = 0;

        //"Colocamos" las piezas blancas
    for(fila = 6; fila < N; fila++)
    {
        for(columna = 0; columna < N; columna++) //Invierto el sentido para reutilizar el codigo del switch
        {
            switch(num_pieza)
            {
            case 8:
            case 15:
                tablero->casillas[columna][fila].pieza.tipo = 'T';
                tablero->casillas[columna][fila].pieza.textura = texturas_1[2];
                break;
            case 9:
            case 14:
                tablero->casillas[columna][fila].pieza.tipo = 'C';
                tablero->casillas[columna][fila].pieza.textura = texturas_1[1];
                break;
            case 10:
            case 13:
                tablero->casillas[columna][fila].pieza.tipo = 'A';
                tablero->casillas[columna][fila].pieza.textura = texturas_1[3];
                break;
            case 11:
                tablero->casillas[columna][fila].pieza.tipo = 'R';
                tablero->casillas[columna][fila].pieza.textura = texturas_1[5];
                break;
            case 12:
                tablero->casillas[columna][fila].pieza.tipo = 'D';
                tablero->casillas[columna][fila].pieza.textura = texturas_1[4];
                break;
            default:
                tablero->casillas[columna][fila].pieza.tipo = 'P';
                tablero->casillas[columna][fila].pieza.textura = texturas_1[0];
                tablero->casillas[columna][fila].pieza.primer_movimiento = 1;
            }
            num_pieza++;
        }
    }
}


void inicializarPartida(App* app, Partida* p, char nombre_1[], char nombre_2[])   //Reinicia la partida
{
    p->estado = ESTADO_INICIO;
    p->turno = TURNO_JUGADOR_1;

    strcpy(p->jugador_1.nombre, nombre_1);
    strcpy(p->jugador_2.nombre, nombre_2);

    p->jugador_1.moviendo_pieza = 0;
    p->jugador_2.moviendo_pieza = 0;

    inicializarTablero(app, &p->tablero); //El tablero contiene todas las piezas
}


void humanoVShumano(char nombre_1[], char nombre_2[])
{
    App app;
    iniciarSDL(&app);

    Partida p;
    inicializarPartida(&app, &p, nombre_1, nombre_2);

    printf("\n\n\tHaz click en la ventana para empezar la partida.\n");

    do
    {
        prepararEscena(&app);

        gestionarEntradas(&p);
        dibujar(app.renderer, p.tablero);

        presentarEscena(&app);

        SDL_Delay(16);
    }
    while(p.estado != ESTADO_SALIR);

    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

//void humanoVSmaquina(){}
