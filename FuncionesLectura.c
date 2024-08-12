#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Data.h"
/*
 * Nombres: Angel Octavio Ramos Jimenez y Alexis Eduardo Almanza Ortega
 * Materia: Lenguaje de Programaci�n Estructurado
 * Fecha: 15 de Junio de 2021
 * Nombre del Trabajo: PROYECTO FINAL JUEGO NAVAL
 * Nombre de la Profesora: Sara Vera Noguez
 * NOTA IMPORTANTE: PARA LA LIBRERIA WINSOCK, SE DEBE PONER EN EL IDE LA FRASE '-lwsock32' para que funcione dicha LIBRERIA
 *                  EN NETBEANS SE PONE EN: CLICK DERECHO EN EL PROYECTO -> PROPIEDADES -> BUILD (NO SE DA CLICK) -> LINKER (SUBMENU DE BUILD)
 *                  -> LIBRARIES -> EN LOS '...' QUE HAY -> ADD OPTION... -> OTHER OPTION -> ESCRIBIR LA FRASE Y DAR ACEPTAR
 */
int leerEnteroPositivo(){ //Si es -1 es cadena vacia
    fflush( stdin );
    char *cadena, caracter;
    int contador=0;
    cadena = (char*)malloc(sizeof(char));
    do{
       caracter = getchar();
       if(isdigit(caracter)){
            cadena[contador]=caracter;
            contador++;
        }
    }while(caracter!='\n' && contador<TAM_MAX_ENTEROS);
    cadena[contador]='\0';
    if(contador==0){
        return -1;
    }else{
        return atoi(cadena);
    }
}
