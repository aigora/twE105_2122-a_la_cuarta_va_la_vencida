#include <stdio.h>

int main(){
  int sel;
  
  
  printf("Bienvenido! Seleccione una de las siguientes opciones:\n1. Jugar\n2. Instrucciones\n3. Partidas anteriores\n4. Ranking de mejores jugadores");
  scanf("%i", sel);
  
  switch (sel) {
  	case 1:
  		printf("Empieza el juego\n"); //Esto es temporal, aquí iría el código para iniciar el juego
  	
	case 2:
		printf("Instrucciones\n"); //El programa lee un archivo txt e imprime en pantalla lo leído. 
	
	case 3:
          printf("Partidas Anteriores\n"); //El programa lee un archivo txt con infromacion sobre partidas anteriores lmacenadas en la memoria.
    case 4:
          printf("Tabla de mejores jugadores\n"); // El programa lee un archivo txt con los nombres de los jugadores y sus victorias.
  }
  
  return 0;
}
  
