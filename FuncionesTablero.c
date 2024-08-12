#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int esGanador(const Tablero *t){
    int contador = 0;
    for(int i=0; i<t->contadorTiradas; i++){
        if(t->tiradas[i].estado==2){
            contador++;
        }
    }
    return contador==t->config.numNaves;
}

int esPerdedor(const Tablero *t){
    return t->config.numTiradas==t->contadorTiradas;
}

char confirmaEstadoTablero(const Tablero *t){
    if(esGanador(t)){
        return 'g';
    }else{
        if(esPerdedor(t)){
            return 'p';
        }else{
            return 'n';
        }
    }
}

int existeTirada(const Tablero *t, const Casilla *tirada){
    for(int i=0; i<t->contadorTiradas; i++){
        if(t->tiradas[i].coordenada.x == tirada->coordenada.x && t->tiradas[i].coordenada.y == tirada->coordenada.y){
            printf("\n\t!!!!!!!!!!!!!!!!Ya Realizaste esa Tirada!!!!!!!!!!!!!!!!\n");
            return 1;
        }
    }
    return 0;
}

void guardaTirada(Tablero *t, const Casilla *tirada){
    t->tiradas[(t->contadorTiradas)++] = *tirada;
}

//0. Diste a agua 1. Le diste a una nave 2. Le diste y tiraste la nave
int tiradaTablero(Tablero *t, const Coordenada *coord){
    int indice=0;
    Nave *n = t->naves.buscaNave(&(t->naves),coord);
    if(n==NULL){
        return 0;
    }else{
        if(n->orientacion){
            indice = coord->x - n->casilla[0].coordenada.x;
        }else{
            indice = coord->y - n->casilla[0].coordenada.y;
        }
        n->casilla[indice].estado = 1;
        if(incrementaDamage(n)){
            return 2;
        }else{
            return 1;
        }
    }
}

void estableceConfiguracion(Configuracion *c, const unsigned int nivel){
    char conf[50] = "";
    leeConfiguracion(ARCHIVO_CONFIGURACION,conf,nivel);
    c->tamX = atoi(strtok(conf," "));
    c->tamY = atoi(strtok(NULL," "));
    c->numNaves = atoi(strtok(NULL," "));
    c->numTiradas = atoi(strtok(NULL," "));
    c->numTipoNaves = (int*)malloc(sizeof(int)*LONGITUD_MAX_NAVE);
    for(int i = 0; i<LONGITUD_MAX_NAVE; i++){
        c->numTipoNaves[i] = atoi(strtok(NULL," "));
    }
}

int inicializaConfiguracion(Configuracion *c){
    int nivel;
    muestraConfiguracion();
    do{
        printf("\n\nQue nivel deseas de Configuracion del Juego? ");
        nivel=leerEnteroPositivo();
    }while(nivel==-1 || validaRango(nivel,0,5)==0);
    estableceConfiguracion(c,nivel);
    return nivel;
}

void creaTablero(Tablero *t, const Configuracion *c){
    Nave nave;
    t->config = *c;
    t->tiradas = (Casilla*) malloc(c->numTiradas*sizeof(Casilla));
    t->contadorTiradas = 0;
    iniciaLista(&(t->naves));
    for(int i=0; i<LONGITUD_MAX_NAVE; i++){
        for(int j=0; j<c->numTipoNaves[i]; j++){
            nave.longitud = i+1;
            do{
                muestraTablero(t);
                printf("\nLlenado de la Nave %d de longitud %d\n",j+1,i+1);
                llenaNave(&nave,c->tamX,c->tamY);
            }while(validaRangoTablero(&nave,c->tamX,c->tamY)==0 || validaTraslapeNave(&nave,&(t->naves))==0);
            t->naves.inserta(&(t->naves),&nave);
        }
    }
    printf("\n\t*****************TABLERO FINAL*****************\n");
    muestraTablero(t);
}
