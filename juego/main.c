#include <stdio.h>


int main(int argc, const char * argv[]) {
    
    int sel, modo, color, temp;
    char jugador1[20], jugador2[20];
    
    printf("Bienvenido! Elige una de las opciones\n");
    printf("1. Nueva Partida\n2. Cargar Partida Anterior\n 3. Instrucciones\n 4. Ranking\n5. Salir");
    scanf("%i", &sel);
    
    do{
    switch(sel){
            
        case 1:
            do{
            printf("Elige modo de juego:\n");
            printf("1. Humano vs. Humano\n2. Humano vs. Maquina\n");
            scanf("%i", &modo);
            } while (modo !=  )
            if(modo==1){
                printf("Ingrese el nombre del jugador 1:\n");
                fgets(jugador1, 20, stdin);
                
                printf("ÀQue color sera %s?\n", jugador1);
                printf("1. Blancas\n2. Negras\n");
                scanf("%i", &color);
                if(color == 1){
                    printf("De acuerdo, la maquina sera el color negro\n");
                } else if(color == 2){
                    printf("De acuerdo, la maquina sera el color blanco\n");
                }
                
                printf("ÀDesea jugar con limite de tiempo?\n");
                printf("1. Si\n2. No\n");
                scanf("%i", &temp);
                
                
            }
    }
    } while(sel <1 && sel>5)
    
    return 0;
}

