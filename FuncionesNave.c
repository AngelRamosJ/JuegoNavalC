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

int incrementaDamage(Nave *n){
    ++(n->damage);
    if(n->damage==n->longitud){
        return 1;
    }else{
        return 0;
    }
}

void llenaNave(Nave *n, const unsigned int tamX, const unsigned int tamY){
    do{
        printf("\nDame la Orientacion de la Nave (0.HORIZONTAL y 1.VERTICAL):  ");
        n->orientacion=leerEnteroPositivo();
    }while(n->orientacion==-1 || validaRango(n->orientacion,ORIENTACION_HORIZONTAL,ORIENTACION_VERTICAL)==0);
    n->casilla = (Casilla*)malloc(sizeof(Casilla)*n->longitud);
    do{
        printf("\nDame la Coordenada Inicial del EJE X de la Nave:  ");
        n->casilla[0].coordenada.x=leerEnteroPositivo();
    }while(n->casilla[0].coordenada.x==-1 || validaRango(n->casilla[0].coordenada.x,1,tamX)==0);
    do{
        printf("\nDame la Coordenada Inicial del EJE Y de la Nave:  ");
        n->casilla[0].coordenada.y=leerEnteroPositivo();
    }while(n->casilla[0].coordenada.y==-1 || validaRango(n->casilla[0].coordenada.y,1,tamY)==0);
    n->casilla[0].estado = 0;
    for(int i=1; i<n->longitud; i++){
        if(n->orientacion){
            n->casilla[i].coordenada.y = n->casilla[0].coordenada.y;
            n->casilla[i].coordenada.x = n->casilla[i-1].coordenada.x+1;
            n->casilla[i].estado = 0;
        }else{
            n->casilla[i].coordenada.y = n->casilla[i-1].coordenada.y+1;
            n->casilla[i].coordenada.x = n->casilla[0].coordenada.x;
            n->casilla[i].estado = 0;
        }
    }
    n->damage = 0;
}
