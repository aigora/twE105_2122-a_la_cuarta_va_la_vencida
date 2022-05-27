#include "funciones.h"

int main(int argc, char *argv[])
{
    _Bool salir = 0;

    char nombre_1[20];
    char nombre_2[20];

    int menu;

    while(!salir)
    {
        printf("*******************AJEDREZ*********************\n\n Introduce tu nombre: ");
        scanf("%s", nombre_1);

        limpiarPantalla();

        printf("\n Bienvenido, %s! \n\n Opciones disponibles:\n  1 - Jugar contra un amigo.\n  2 - Salir. \n ", nombre_1);
        scanf("%i", &menu);

        limpiarPantalla();

        switch(menu)
        {
            case 1:
                printf("\n Como se llama tu amigo? ");
                scanf("%s", nombre_2);
                humanoVShumano(nombre_1, nombre_2);
                break;
            case 2:
                printf("\n ¡Hasta la proxima!\n\n");
                salir = 1;
                break;
            default:
                break;
        }
    }

    return 0;
}
