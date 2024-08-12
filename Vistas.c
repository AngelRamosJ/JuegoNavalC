#include <stdio.h>
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
void muestraConfiguracion(){
    char **matriz;
    int numLineas = contarLineas(ARCHIVO_CONFIGURACION);
    matriz = creaMatriz(numLineas,100);
    leerTodoArchivo(ARCHIVO_CONFIGURACION,matriz);
    for(int i = 0; i<numLineas; i++){
        printf("%s",matriz[i]);
    }
    liberarMemoriaMatriz(matriz,numLineas);
}

void muestraConfiguracionEspecifica(const unsigned int nivel){
    char **matriz;
    matriz = creaMatriz(VARIABLES_CONFIGURACION+1,100);
    leeConfiguracionEspecifica(ARCHIVO_CONFIGURACION,matriz,nivel);
    for(int i = 0; i<VARIABLES_CONFIGURACION+1; i++){
        printf("%s\n",matriz[i]);
    }
    liberarMemoriaMatriz(matriz,VARIABLES_CONFIGURACION+1);
}

void muestraTablero(const Tablero *t){
    nodoListaSimple *aux;
    int numNave= 49;
    char **matriz;
    matriz = creaMatriz(t->config.tamX,t->config.tamY);
    for(int i=0; i<t->config.tamX; i++){
        for(int j=0; j<t->config.tamY; j++){
            matriz[i][j] = 79;
        }
    }
    aux = t->naves.Inicio;
  //  for(int i=0; i<t->config.numNaves; i++){
    while(aux!=NULL){
        for(int j=0; j<aux->nave.longitud; j++){
            if(aux->nave.casilla[j].estado){
                matriz[aux->nave.casilla[j].coordenada.x-1][aux->nave.casilla[j].coordenada.y-1] = 88;
            }else{
                matriz[aux->nave.casilla[j].coordenada.x-1][aux->nave.casilla[j].coordenada.y-1] = numNave;
            }
        }
        numNave++;
        aux = aux->siguiente;
    }
    printf("\n\t\t");
    for(int i=0; i<t->config.tamX; i++){
        printf("%d\t",i+1);
    }
    printf("\n");
    for(int i=0; i<t->config.tamX; i++){
        printf("\n\t%d\t",i+1);
        for(int j=0; j<t->config.tamY; j++){
            printf("%c\t",matriz[i][j]);
        }
        printf("\n");
    }
    liberarMemoriaMatriz(matriz,t->config.tamX);
}

void muestraTableroOriginal(const Tablero *t){
    nodoListaSimple *aux;
    int numNave= 49;
    char **matriz;
    matriz = creaMatriz(t->config.tamX,t->config.tamY);
    for(int i=0; i<t->config.tamX; i++){
        for(int j=0; j<t->config.tamY; j++){
            matriz[i][j] = 79;
        }
    }
    aux = t->naves.Inicio;
  //  for(int i=0; i<t->config.numNaves; i++){
    while(aux!=NULL){
        for(int j=0; j<aux->nave.longitud; j++){
            matriz[aux->nave.casilla[j].coordenada.x-1][aux->nave.casilla[j].coordenada.y-1] = numNave;
        }
        numNave++;
        aux = aux->siguiente;
    }
    printf("\n\t\t");
    for(int i=0; i<t->config.tamX; i++){
        printf("%d\t",i+1);
    }
    printf("\n");
    for(int i=0; i<t->config.tamX; i++){
        printf("\n\t%d\t",i+1);
        for(int j=0; j<t->config.tamY; j++){
            printf("%c\t",matriz[i][j]);
        }
        printf("\n");
    }
    liberarMemoriaMatriz(matriz,t->config.tamX);
}

void muestraTableroEnemigo(const Tablero *t){
    char **matriz;
    matriz = creaMatriz(t->config.tamX,t->config.tamY);
    for(int i=0; i<t->config.tamX; i++){
        for(int j=0; j<t->config.tamY; j++){
            matriz[i][j] = 79;
        }
    }
    for(int i=0; i<t->contadorTiradas; i++){
        switch(t->tiradas[i].estado){
            case 0: matriz[t->tiradas[i].coordenada.x-1][t->tiradas[i].coordenada.y-1] = 207;
                    break;
            case 1: matriz[t->tiradas[i].coordenada.x-1][t->tiradas[i].coordenada.y-1] = 67;
                    break;
            case 2: matriz[t->tiradas[i].coordenada.x-1][t->tiradas[i].coordenada.y-1] = 88;
                    break;
            default: matriz[t->tiradas[i].coordenada.x-1][t->tiradas[i].coordenada.y-1] = 79;
        }
    }
    printf("\n\t\t");
    for(int i=0; i<t->config.tamX; i++){
        printf("%d\t",i+1);
    }
    printf("\n");
    for(int i=0; i<t->config.tamX; i++){
        printf("\n\t%d\t",i+1);
        for(int j=0; j<t->config.tamY; j++){
            printf("%c\t",matriz[i][j]);
        }
        printf("\n");
    }
    liberarMemoriaMatriz(matriz,t->config.tamX);
}

int menuJuegoNaval(){
    int op=0;
    printf("\n******************Elige que deseas hacer******************");
    printf("\n1. Mostrar Estado Actual de Tu Tablero");
    printf("\n2. Mostrar Estado Original de Tu Tablero");
    printf("\n3. Mostrar Estado del Tablero Enemigo");
    printf("\n4. Realizar Tirada");
    printf("\n5. Mostrar Informacion del Juego");
    printf("\n6. Salir del Juego");
    do{
        printf("\nEleccion: ");
        op = leerEnteroPositivo();
    }while(op==-1 || validaRango(op,1,6)==0);
    return op;
}

void muestraInstrucciones(const Tablero *t){
    printf("\n\n************************Instrucciones del Juego Naval************************");
    printf("\n\n\t\t\t*******GENERAL*******");
    printf("\n\n\tDebes eliminar todas las Naves de Tu enemigo para Ganar");
    printf("\n\tPara ello debes Atinar a las Casillas de todas las Naves del Otro Jugador");
    printf("\n\tTienes un Limite de Tiradas, No las Desaproveches");
    printf("\n\n\t\t*******Simbolos de tu Tablero*******");
    printf("\n\n\t\tLos numeros indican las Casillas de tus Naves");
    printf("\n\t\tUn mismo numero indica que es de la misma Nave");
    printf("\n\t\tLa 'O' indica que es Agua");
    printf("\n\t\tLa 'X' indica que tu Recibiste un ataque a tu nave");
    printf("\n\n\t\t*******Simbolos del Tablero Enemigo*******");
    printf("\n\n\t\tLa 'O' indica que es una casilla Desconocida");
    printf("\n\t\tEl '%c' indica que es le diste a Agua ",207);
    printf("\n\t\tLa 'C' indica que es le diste a una Nave (Casi Muerto)");
    printf("\n\t\tLa 'X' indica que es le diste a una Nave y la Hundiste (Casi Muerto)");
    printf("\n\n\t\t*******INFORMACION DE LA CONFIGURACION*******");
    printf("\n\n\t\tTu Tablero es de tiene un tamano de %dX%d",t->config.tamX,t->config.tamY);
    printf("\n\t\tTu Tablero tiene un total de %d Naves",t->config.numNaves);
    for(int i=0; i<LONGITUD_MAX_NAVE; i++){
        printf("\n\t\tTienes %d Naves de Longitud %d",t->config.numTipoNaves[i],i+1);
    }
    printf("\n\t\tTienes %d Tiradas y haz usado %d",t->config.numTiradas,t->contadorTiradas);
    printf("\n\n************************HAZ TODO PARA GANAR Y BUENA SUERTE************************");
    
}
