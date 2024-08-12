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
void iniciaLista(lista *l){
    l->Inicio = NULL;
    l->Final = NULL;
    l->inserta = insercionLista;
    l->buscaCasilla = buscaCasillaNave;
    l->buscaNave = buscaNaveCoordenada;
}

void insercionAlFinal(lista *l, const Nave *nave){   //Seria bueno tener un apuntador a Final
    nodoListaSimple *nuevo = NULL;
    nuevo = (nodoListaSimple*) malloc(sizeof(nodoListaSimple));
    nuevo->nave = *nave;
    //iniciaNodo(nuevo,nave);
    l->Final->siguiente = nuevo;
    nuevo->siguiente=NULL;
    l->Final = nuevo;
}

void primeraInsercion(lista *l, const Nave *nave){
    nodoListaSimple *nuevo;
    nuevo = (nodoListaSimple*) malloc(sizeof(nodoListaSimple));
    nuevo->nave = *nave;
    nuevo->siguiente = NULL;
    //iniciaNodo(nuevo,nave);
    l->Inicio = nuevo;
    l->Final = nuevo;
}

int esListaVacia(const lista *l){
    if(l->Inicio==NULL){
        return 1;
    }
    return 0;
}

int insercionLista(lista *l, const Nave *nave){
    if(esListaVacia(l)){
        primeraInsercion(l,nave);
        return 1;
    }else{
        insercionAlFinal(l,nave);
        return 1;
    }
    return 0;
}

int buscaCasillaNave(const lista *l, const Casilla *casilla){
    nodoListaSimple *aux = l->Inicio;
    while(aux!=NULL){
        for(int i=0; i<aux->nave.longitud; i++){
            if(aux->nave.casilla[i].coordenada.x == casilla->coordenada.x && aux->nave.casilla[i].coordenada.y == casilla->coordenada.y){
                return 1;
            }
        }
        aux = aux->siguiente;
    }
    return 0;
}

Nave * buscaNaveCoordenada(const lista *l, const Coordenada *coordenada){
    nodoListaSimple *aux = l->Inicio;
    while(aux!=NULL){
        for(int i=0; i<aux->nave.longitud; i++){
            if(aux->nave.casilla[i].coordenada.x == coordenada->x && aux->nave.casilla[i].coordenada.y == coordenada->y){
                return &(aux->nave);
            }
        }
        aux = aux->siguiente;
    }
    return &(aux->nave);
}


