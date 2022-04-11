#include <stdio.h>

#define N 12 //caracteres para los nombres de jugadores


int main(int argc, const char * argv[]) {
    
    int sel, modo, color, temp, opTime;
    char jugador1[N], jugador2[N];
    
    printf("Bienvenido al Juego del Ajedrez!\nElige una de las opciones:\n\n");
    printf("1. Nueva Partida\n2. Cargar Partida Anterior\n3. Instrucciones\n4. Ranking\n5. Salir\n");
    scanf("%i", &sel);
    
    do{
    switch(sel){
        case 1:
            do{
            printf("Elige modo de juego:\n");
            printf("1. Humano vs. Humano\n2. Humano vs. Maquina\n3. Salir\n");
            scanf("%i", &modo);
            } while (modo !=  1 && modo != 2 && modo != 3);
            
        switch (modo){
                
            case 1:
                fflush(stdin);
                printf("Ingrese el nombre del Jugador 1:\n");
                fgets(jugador1, N, stdin);
                
                fflush(stdin);
                printf("Ingrese el nombre del Jugador 2:\n");
                fgets(jugador2, N, stdin);
                
                do{
                printf("ÀQue color sera %s? \n", jugador1);
                printf("1. Blancas\n2. Negras\n");
                scanf("%i", &color);
                } while(color!= 1 && color!= 2);
                
                
                if(color == 1){
                    printf("De acuerdo, %s sera el color negro\n", jugador2);
                } else if(color == 2){
                    printf("De acuerdo, %s sera el color blanco\n", jugador2);
                }
                
                do{
                printf("ÀDesea jugar con limite de tiempo?\n");
                printf("1. Si\n2. No\n");
                scanf("%i", &opTime);
                }while (opTime != 1 && opTime != 2);
                
                if(opTime == 1){
                    printf("Ingrese el limite de tiempo deseado: ");
                    scanf("%i", &temp);
                }
                printf("TODO LISTO! EMPIEZA LA PARTIDA, %s VS %s!", jugador1, jugador2);
                break;
                
            case 2:
                fflush(stdin);
                printf("Ingrese el nombre del Jugador 1:\n");
                fgets(jugador1, 20, stdin);
                
                do{
                printf("ÀQue color sera %s? \n", jugador1);
                printf("1. Blancas\n2. Negras\n");
                scanf("%i", &color);
                } while(color!= 1 && color!= 2);
                
                if(color == 1){
                    printf("De acuerdo, la maquina sera el color negro\n");
                } else if(color == 2){
                    printf("De acuerdo, la maquina sera el color blanco\n");
                }
                do{
                printf("ÀDesea jugar con limite de tiempo?\n");
                printf("1. Si\n2. No\n");
                scanf("%i", &opTime);
                }while (opTime != 1 && opTime != 2);
                
                if(opTime == 1){
                    printf("Ingrese el limite de tiempo deseado: ");
                    scanf("%i", &temp);
                }
                printf("TODO LISTO! EMPIEZA LA PARTIDA CONTRA LA MAQUINA!");
                break;
                
            case 3:
                break;
            }
        case 2:
            //Se abriria la memoria con partidas pasadas que se guardaron
            break;
        case 3:
            //Aparece informacion guardada en un fichero sobre las reglas y jugadas basicas del ajedrez.
            break;
        case 4:
            //Aparece una lista de los jugadores y sus victorias ordenadas de mas a menos victorias (en fichero).
            break;
        case 5:
            //Salir
            break;
            
    }
    } while(sel <1 && sel>5);
    
    return 0;
}

