#include <stdio.h>
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

#define VACIO 0
#define PEON 1
#define CABALLO 2
#define TORRE 3
#define ALFIL 4
#define DAMA 5
#define REY 6

#define COLOR_BLANCO 0
#define COLOR_DORADO 1

#define ESTADO_INICIO_PARTIDA 0
#define ESTADO_EN_PARTIDA 1
#define ESTADO_VICTORIA_JUGADOR_1 2
#define ESTADO_VICTORIA_JUGADOR_2 3
#define ESTADO_SALIR 4

typedef struct
{
    int x;
    int y;
} Vector2D;

typedef struct
{
    Vector2D pos;
    int tipo;
    SDL_Texture* textura;
}Pieza;

typedef struct
{
    char nombre[20];
    int num_piezas;
    Pieza piezas[PIEZAS_INICIALES];
}Jugador;

typedef struct
{
    Vector2D pos;
    char pieza;
} Casilla;

typedef struct
{
    Vector2D pos;
    SDL_Texture* textura;
    Casilla casillas[N][N];
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
void humanoVSmaquina(Jugador* jugador_1);
void iniciarSDL(App* app);
void prepararEscena(App* app);
void presentarEscena(App* app);
void gestionarEntradas(Partida* p);
SDL_Texture* cargarTextura(App* app, char* nombre_fichero);
void dibujarTablero(SDL_Renderer* rend, SDL_Texture* textura, Vector2D pos);
Vector2D calculoPosicionCasilla(int fila, int columna);
