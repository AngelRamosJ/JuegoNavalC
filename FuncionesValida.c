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
int validaRango(const unsigned int valor, const unsigned int valorMin, const unsigned int valorMax){
    if(valor>=valorMin && valor<=valorMax){
        return 1;
    }else{
        printf("Valor Invalido, el valor debe estar entre %d y %d\n",valorMin,valorMax);
        return 0;
    }
}

int validaRangoTablero(const Nave *n, const unsigned int tamX, const unsigned int tamY){
    if(n->orientacion){
        if((n->longitud-1+n->casilla[0].coordenada.x)>tamX){
            printf("\n\t!!!!!!!!!!!!!!!!La Nave esta fuera del Tablero!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }else{
        if((n->longitud-1+n->casilla[0].coordenada.y)>tamY){
            printf("\n\t!!!!!!!!!!!!!!!!La Nave esta fuera del Tablero!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }
    return 1;
}

int validaTraslapeNave(const Nave *nave, const lista *naves){
    for(int i=0; i<nave->longitud; i++){
        if(naves->buscaCasilla(naves,&(nave->casilla[i]))){
            printf("\n\t!!!!!!!!!!!!!!!!La Nave se tralapa con otra Nave!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }
    return 1;
}
