#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

#ifdef _WIN32
  #include<windows.h>
#endif

#define SCREEN_WITDH 800
#define SCREEN_HEIGHT 800
#define BOX_SIZE 100 //Tamaño de casilla

#define N 8 //Numero de filas y columnas que tiene un tablero de ajedrez

#define REESCALADO_TEXTURA_PIEZA_X 10
#define REESCALADO_TEXTURA_PIEZA_Y 8

#define ESTADO_INICIO 0
#define ESTADO_JUGANDO 1
#define ESTADO_VICTORIA_JUGADOR_1 2
#define ESTADO_VICTORIA_JUGADOR_2 3
#define ESTADO_REINICIO 4
#define ESTADO_SALIR 5

#define TURNO_JUGADOR_1 1
#define TURNO_JUGADOR_2 2
#define JUGADOR_1 1
#define JUGADOR_2 2

#define VERTICAL 0
#define HORIZONTAL 1
#define DIAGONAL 2
#define FORMA_DE_L 3
#define NO_PERMITIDO 4

#define NORTE 0
#define SUR 1
#define ESTE 2
#define OESTE 3
#define NORESTE 4
#define SURESTE 5
#define SUROESTE 6
#define NOROESTE 7

#define UNA_CASILLA 1
#define DOS_CASILLAS 2
#define TRES_CASILLAS 3

typedef struct
{
    int x;
    int y;
} Vector2D;

typedef struct
{
    int direccion;          //Puede ser vertical, horizontal, diagonal o en forma de L
    int sentido;            //Hacia arriba o hacia abajo
    Vector2D dif_casillas;  //Cantidad de casillas que se avanza en cada uno de los ejes
    _Bool obstaculo;        //1 si hay alguna pieza en el camino y 0 si no la hay
    _Bool forma_de_L;       //1 si el movimiento tiene forma de L Y 0 si no tiene esa forma
    _Bool legalidad;        //1 si el movimiento es legal y 0 si no lo es
} Movimiento;

typedef struct
{
    char tipo;      //0(vacio), r(rey), d(dama), c(caballo), a(alfil), t(torre), p(peon) | minusculas->doradas, MAYUSCULAS->BLANCAS
    SDL_Texture* textura;
    int en_movimiento;
    int primer_movimiento; //Flag para que el peón sepa que puede moverse dos casillas la primera vez
}Pieza;

typedef struct //Casillas que forman el tablero 8x8
{
    Vector2D pos;
    Pieza pieza;
} Casilla;

typedef struct
{
    int id;
    char nombre[20];
    int victorias;
    int moviendo_pieza;
    Casilla* casilla_movimiento; //Almacena la pieza que estoy moviendo
}Jugador;

typedef struct
{
    Vector2D pos;
    SDL_Texture* textura;
    Casilla casillas[N][N];
} Tablero;

typedef struct //Ventana del juego
{
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

typedef struct
{
    Jugador jugador_1;
    Jugador jugador_2;
    Tablero tablero;
    int turno;
    int estado;
    int ganador;
} Partida;


void humanoVShumano(char nombre_1[], char nombre_2[]);

void limpiarPantalla();

void iniciarSDL(App* app);

void prepararEscena(App* app);

void presentarEscena(App* app);

Vector2D calcularColumnaFila(Vector2D pos);

Vector2D calcularCasillaClick();

_Bool clickCasillaInicial(Vector2D pos_inicial, Vector2D pos_final);

int clickPieza(char tipo_pieza, int turno);

_Bool clickCasillaLibre(char tipo_pieza);

void cambiarTurno(Partida* p);

void moverPieza(Pieza* p_inicial, Pieza* p_final);

int calcularSentido(int dir, Vector2D desp);

int calcularDireccion(Vector2D desp);

_Bool calcularMovimientoEnL(Vector2D desp);

_Bool calcularObstaculo(Casilla c[][N], Casilla c_origen, Casilla c_destino, Movimiento m);

Vector2D calcularDesplazamiento(Vector2D origen, Vector2D destino);

_Bool calcularMovimientoLegal(Casilla c[][N], Casilla c_origen, Casilla c_destino, int pieza_destino);

void jugarTurno(Partida* p, int columna, int fila);

void victoria(Jugador* j);

void gestionarClick(Partida* p, int columna, int fila);

void gestionarEntradas(Partida* p);

SDL_Texture* cargarTextura(App* app, char* nombre_fichero);

void dibujarTablero(SDL_Renderer* rend, SDL_Texture* textura, Vector2D pos);

void dibujarPiezaEnMovimiento(SDL_Renderer* rend, Casilla c);

void dibujarPieza(SDL_Renderer* rend, Casilla c);

void dibujarPiezas(SDL_Renderer* rend, Casilla casillas[][N]);

void dibujar(SDL_Renderer* rend, Tablero tablero);

Vector2D calculoPosicionCasilla(int fila, int columna); //Calcula la posicion x,y de cada casilla

void inicializarTablero(App* app, Tablero* tablero);

void inicializarPartida(App* app, Partida* p, char nombre_1[], char nombre_2[]);   //Reinicia la partida

void inicializar(App* app, Partida* p, char nombre_1[], char nombre_2[]);

