#include <stdio.h>
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
int leeConfiguracion(const char *nomArch, char *conf, const unsigned int nivel){
    int bandera = 0;
    int tamCad = 0;
    char lectura[100];
    char auxLectura[100];
    char numeroNivel[4];
    char *elemento;
    char tipoNave[2];
    char nivelJuego[7] = "Nivel ";
    char separadorEspacio[4] = " ";
    sprintf(numeroNivel,"%d",nivel);
    strcat(nivelJuego,numeroNivel);
    FILE* arch;
    arch = fopen(nomArch,"rt");
    if(arch==NULL){
        return 0;
    }
    while(!feof(arch)){
        fgets(lectura,100,arch);
        tamCad = strlen(lectura);
        if(lectura[tamCad-1]==10){
            strncpy(auxLectura,lectura,tamCad-1);
            auxLectura[tamCad-1] = '\0';
        }else{
            strcpy(auxLectura,lectura);
        }
        if(strcmp(auxLectura,nivelJuego)==0){
            bandera = 1;
            continue;
        }
        if(bandera){
            elemento = strtok(auxLectura,separadorEspacio);
            if(bandera == VARIABLES_CONFIGURACION){
                while((elemento = strtok(NULL,separadorEspacio))!=NULL){
                    tipoNave[0] = elemento[2];
                    tipoNave[1] = '\0';
                    strcat(conf,tipoNave);
                    strcat(conf," ");
                }
                break;
            }else{
                if((elemento = strtok(NULL,separadorEspacio))!=NULL){
                    strcat(conf,elemento);
                    strcat(conf," ");
                }
            }
            bandera++;
        }
    }
    fclose(arch);
    return 1;
}

int leeConfiguracionEspecifica(const char *nomArch, char **matriz, const unsigned int nivel){
    int bandera = 0;
    int tamCad = 0;
    int i = 0;
    char lectura[100];
    char auxLectura[100];
    char numeroNivel[4];
    char nivelJuego[7] = "Nivel ";
    sprintf(numeroNivel,"%d",nivel);
    strcat(nivelJuego,numeroNivel);
    FILE* arch;
    arch = fopen(nomArch,"rt");
    if(arch==NULL){
        return 0;
    }
    while(!feof(arch)){
        fgets(lectura,100,arch);
        tamCad = strlen(lectura);
        if(lectura[tamCad-1]==10){
            strncpy(auxLectura,lectura,tamCad-1);
            auxLectura[tamCad-1] = '\0';
        }else{
            strcpy(auxLectura,lectura);
        }
        if(strcmp(auxLectura,nivelJuego)==0){
            bandera = 1;
        }
        if(bandera){
            strcpy(matriz[i],lectura);
            i++;
            if(bandera==VARIABLES_CONFIGURACION+1){
                break;
            }
            bandera++;
        }
    }
    fclose(arch);
    return 1;
}

int contarLineas(const char *nomArch){
    int numLineas=0;
    char lectura[100];
    FILE* arch;
    arch = fopen(nomArch,"rt");
    if(arch==NULL){
        return 0;
    }
    while(feof(arch)==0){
        fgets(lectura,100,arch);
        numLineas++;
    }
    fclose(arch);
    return numLineas;
}

int leerTodoArchivo(const char *nomArch,char **matriz){
    int i = 0;
    FILE* arch;
    arch = fopen(nomArch,"rt");
    if(arch==NULL){
        return 0;
    }
    while(!feof(arch)){
       fgets(matriz[i],100,arch);
       i++;
    }
    fclose(arch);
    return 1;
}
