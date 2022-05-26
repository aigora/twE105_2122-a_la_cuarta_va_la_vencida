#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define SCREEN_WITDH 800
#define SCREEN_HEIGHT 800

#define N 8
#define PIEZAS_INICIALES 16
#define TIPOS_DE_PIEZAS 6

#define REESCALADO_TEXTURA_PIEZA_X 10
#define REESCALADO_TEXTURA_PIEZA_Y 8

#define ESTADO_INICIO_PARTIDA 0
#define ESTADO_JUGANDO 1
#define ESTADO_VICTORIA_JUGADOR_1 2
#define ESTADO_VICTORIA_JUGADOR_2 3
#define ESTADO_SALIR 4

#define TURNO_JUGADOR_1 1
#define TURNO_JUGADOR_2 2

typedef struct
{
    int x;
    int y;
} Vector2D;

typedef struct
{
    char tipo;      //0(vacio), r(rey), d(dama), c(caballo), a(alfil), t(torre), p(peon) | minusculas->doradas, MAYUSCULAS->BLANCAS
    SDL_Texture* textura;
    int en_movimiento;
}Pieza;

typedef struct
{
    Vector2D pos;
    Pieza pieza;
} Casilla;

typedef struct
{
    char nombre[20];
    int moviendo_pieza;
    Casilla* casilla_movimiento; //Almacena la pieza que estoy moviendo
}Jugador;

typedef struct
{
    Vector2D pos;
    SDL_Texture* textura;
    Casilla casillas[N][N];
    Vector2D tamano_casillas;
} Tablero;

typedef struct //Ventana
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
} Partida;

void humanoVShumano(char nombre_1[], char nombre_2[]);
