#include <stdio.h>
#include <stdlib.h>
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
int sumaElementosVect(const int *elementos, const unsigned int numElementos){
    int sum=0;
    for(int i=0;i<numElementos;i++){
        sum=sum+elementos[i];
    }
    return sum;
}

void iniciaVector(int *v, const unsigned int tam){
    for(int i=0;i<tam;i++){
        v[i]=0;
    }
}

char** creaMatriz(const unsigned int n, const unsigned int m){
    char** matriz = (char**) malloc(sizeof(char*)*n);
    for(int i=0;i<n;i++) {
    	matriz[i] = (char*) malloc (sizeof(char)*m);
  	}
  	return matriz;
}

void liberarMemoriaMatriz(char **matriz, const unsigned int filas){
    for(int i=0; i<filas; i++){
        free(matriz[i]);
    }
    free(matriz);
}
