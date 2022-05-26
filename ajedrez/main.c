#include "funciones.h"

int main(int argc, char *argv[])
{
    _Bool salir = 0;

    char nombre_1[20];
    char nombre_2[20];

    int menu;

    while(!salir)
    {
        printf("*******************AJEDREZ*********************\n\n\tIntroduce tu nombre: \n\t\t");

        scanf("%s", nombre_1);

        printf("\n\n\tBienvenido, %s! \n\n\tOpciones:\n\t1 - Jugar contra un amigo.\n\t2 - Jugar contra la máquina. \n\t\
3 - Salir. \n\n\t\t", nombre_1);

        scanf("%i", &menu);

        switch(menu)
        {
            case 1:
                printf("\n\n\tComo se llama tu amigo?\n\t\t");
                scanf("%s", nombre_2);
                humanoVShumano(nombre_1, nombre_2);
                break;

            case 2:
                //humanoVSmaquina(&jugador_1);
                break;

            case 3:
                printf("\n\n¡Hasta la proxima!\n\n");
                salir = 1;
                break;

            default:
                break;
        }
    }

    return 0;
}
