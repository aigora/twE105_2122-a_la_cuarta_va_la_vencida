#include "funciones.h"

int main(int argc, char *argv[])
{
    _Bool salir = 0;

    char nombre_1[20];
    char nombre_2[20];

    int ganador, menu;
    int i = 0;

    Jugador j[50];

    FILE* pf;

    while(!salir)
    {
        printf("\n*******************AJEDREZ*********************\n\n Introduce tu nombre: ");
        scanf("%s", nombre_1);

        limpiarPantalla();

        printf("\n Bienvenido, %s! \n\n Opciones disponibles:\n  1 - Jugar contra un amigo.\n  2 - Salir. \n ", nombre_1);
        scanf("%i", &menu);

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

    pf = fopen("Registro_jugadores.txt", "a+");
    if(pf == NULL)
	{
        printf("Error al abrir fichero.\n");
        return -1;
    }
    else
	{
        while(feof(pf) == 0)
        {
            fprintf(pf,"%s\n", nombre_1);
            fprintf(pf,"%s\n", nombre_2);
        }
    }
	fclose(pf);

    return 0;
}
