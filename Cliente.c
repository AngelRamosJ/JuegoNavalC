#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <winsock.h>
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
 * NOTAS EXTRAS:
 * ESTE PRINICPAL, HACE LA FUNCIONALIDAD DEL CLIENTE, EN ESTE CASO, DEL PRIMER JUGADOR
 * ESTE PROGRAMA REALIZA EL JUEGO NAVAL MEDIANTE EL USO DE LA IP 127.0.0.1 (LOCALHOST), PERO SE PUEDE CAMBIAR PARA REALIZAR CON UNA IP PRIVADA,
 * LA IMPLEMENTACION DE ESTE PROGRAMA PARA UNA IP PUBLICA TIENE UN GRADO MAYOR DE DIFICULTAD, POR LO QUE, SE QUEDA PARA FUTURO MANTENIMIENTO
 */
/*NOTAS: Los mensajes pueden ser numeros del 1 al tama�o del tablero (ya que es un tablero cuadrado en las configuraciones)
         Estos mensajes seran usados para las coordenadas y deberan ir pasados con un separador, en este caso ' ' (espacio)
         Adem�s, g - indica que ha ganado el otro jugador
                 p - indica que ha perdido el otro jugador
                 n - indica que no ha pasado nada relevante
                 1 - indica que la confirmacion es positiva
                 2 - indica que la confirmacion es negativa*/
char SendBuff[512],RecvBuff[512];
int main(){
    Configuracion conf;
    Tablero tablero;
    Casilla tirada;
    Coordenada tiradaEnemiga;
    char estadoTablero, estadoTableroEnemigo;
    int opcion, confirmacion, nivel;
    char aux[512] = "";
    WSADATA wsaData;
    SOCKET conn_socket;
    struct sockaddr_in server;
    struct hostent *hp;
    int resp;
    //Inicializamos la DLL de sockets
    resp=WSAStartup(MAKEWORD(1,0),&wsaData);
    if(resp){
        printf("Error al inicializar socket\n\n");
        getchar();return -1;
    }
    //Obtenemos la IP del servidor... en este caso
    // localhost indica nuestra propia máquina...
    hp=(struct hostent *)gethostbyname("localhost");
    if(!hp){
        printf("No se ha encontrado servidor...\n\n");
        getchar();WSACleanup();return WSAGetLastError();
    }
    // Creamos el socket...
    conn_socket=socket(AF_INET,SOCK_STREAM, 0);
    if(conn_socket==INVALID_SOCKET) {
        printf("Error al crear socket\n\n");
        getchar();WSACleanup();return WSAGetLastError();
    }
    memset(&server, 0, sizeof(server)) ;
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_family = hp->h_addrtype;
    server.sin_port = htons(6000);
    printf("\t\t******************JUEGO NAVAL******************\n\n");
    printf("Conectando con el Servidor (Segundo Jugador).......\n\n");
    //Se usa un conect aqui
    // Nos conectamos con el servidor...
    if(connect(conn_socket,(struct sockaddr *)&server,sizeof(server))==SOCKET_ERROR){
        printf("Fallo al conectarse con el servidor\n\n");
        closesocket(conn_socket);
        WSACleanup();getchar();return WSAGetLastError();
    }
    do{
        printf("\n******************Elige la Configuracion del Juego******************\n\n");
        nivel = inicializaConfiguracion(&conf);
        printf("\n\nEsperando Confirmacion del Otro Jugador.......\n");
        //Se usa un send aqui
        sprintf(SendBuff,"%d",nivel);
        send(conn_socket,SendBuff,sizeof(SendBuff),0);
        //Se usa un recv aqui
        recv(conn_socket,RecvBuff, sizeof(RecvBuff), 0);
        confirmacion = atoi(RecvBuff);
        if(confirmacion==2){
            printf("\n\nEl Otro Jugador Rechazo la Configuracion Seleccionada\n");
        }
    }while(confirmacion!=1); //Con lo que se reciba se decide
    printf("\n\n******************Acomoda las Naves de tu Tablero******************\n");
    creaTablero(&tablero,&conf);
    //Se usa un send aqui para cconfirmar la creacion del otro tablero
    send(conn_socket,SendBuff,sizeof(SendBuff),0);
    printf("\n\nEsperando a que el Otro Jugador Acomode su Tablero.......");
    //Se usa un recv aqui para recibir la confirmacion
    recv(conn_socket,RecvBuff, sizeof(RecvBuff), 0);
    do{
        printf("\n\n******************ES TU TURNO******************");
        opcion = menuJuegoNaval();
        printf("\n");
        switch(opcion){
            case 1:
                muestraTablero(&tablero);
                break;
            case 2:
                muestraTableroOriginal(&tablero);
                break;
            case 3:
                muestraTableroEnemigo(&tablero);
                break;
            case 4:
                printf("\n-----------------Tirada-----------------");
                do{
                    do{
                        printf("\nDame la Coordenada del EJE X de la Tirada:  ");
                        tirada.coordenada.x = leerEnteroPositivo();
                    }while(tirada.coordenada.x==-1 || validaRango(tirada.coordenada.x,1,tablero.config.tamX)==0);
                    do{
                        printf("\nDame la Coordenada del EJE Y de la Tirada:  ");
                        tirada.coordenada.y = leerEnteroPositivo();
                    }while(tirada.coordenada.y==-1 || validaRango(tirada.coordenada.y,1,tablero.config.tamY)==0);
                }while(existeTirada(&tablero,&tirada));
                //Se usa un send aqui para mandar la tirada
                sprintf(aux,"%d",tirada.coordenada.x);
                strcpy(SendBuff,aux);
                sprintf(aux,"%d",tirada.coordenada.y);
                strcat(SendBuff," ");
                strcat(SendBuff,aux);
                send(conn_socket,SendBuff,sizeof(SendBuff),0);
                //Se usa un recv aqui para confirmar la tirada
                recv(conn_socket,RecvBuff, sizeof(RecvBuff), 0);
                tirada.estado = atoi(RecvBuff); //Este solo es de prueba, el verdadero debe utilizar la informacion recibida
                guardaTirada(&tablero,&tirada);
                estadoTablero = confirmaEstadoTablero(&tablero);
                //Se usa un send para confirmar el estado de la partida
                aux[0]=estadoTablero;
                aux[1]='\0';
                strcpy(SendBuff,aux);
                send(conn_socket,SendBuff,sizeof(SendBuff),0);
                if(estadoTablero=='g'){
                    printf("\n\nHaz Ganado el Juego Naval");
                    printf("\n\n\t*****************TABLERO ENEMIGO FINAL*****************\n");
                    muestraTableroEnemigo(&tablero);
                    printf("\n\n\t*****************TABLERO TUYO FINAL*****************\n");
                    muestraTablero(&tablero);
                    closesocket(conn_socket);
                    WSACleanup();
                    opcion = 6;
                }
                break;
            case 5:
                muestraInstrucciones(&tablero);
                break;
            case 6:
                //Se usa un close aqui para cerrar la conexion
                //Quizas se deba mandar un mensaje para que se desconecte el servidor
                sprintf(aux,"%d",0);
                strcpy(SendBuff,aux);
                sprintf(aux,"%d",0);
                strcat(SendBuff," ");
                strcat(SendBuff,aux);
                send (conn_socket, SendBuff, sizeof(SendBuff), 0);
                closesocket(conn_socket);
                WSACleanup();
                printf("\n\n******************Juego Finalizado******************");
                opcion = 6;
                break;
            default:
                closesocket(conn_socket);
                WSACleanup();
                printf("\n!!!!!Error Inespaerado!!!!!");
                opcion = 6;
        }
        if(opcion == 4){
            printf("\n\nES TURNO DEL OTRO JUGADOR........");
            //Se usa un recv para dar el turno al otro usuario
            recv (conn_socket, RecvBuff, sizeof(RecvBuff), 0);
            strcpy(aux,RecvBuff);
            tiradaEnemiga.x = atoi(strtok(aux," "));
            tiradaEnemiga.y = atoi(strtok(NULL," "));
            if(tiradaEnemiga.x == 0){
                printf("\n\n\t*****************EL OTRO JUGADOR FINALIZO LA PARTIDA*****************\n");
                closesocket(conn_socket);
                WSACleanup();
                exit(0);
            }
            confirmacion = tiradaTablero(&tablero,&tiradaEnemiga);
            //Se usa un send para confirmarle al otro jugador el estado de la tirada
            sprintf(SendBuff, "%d", confirmacion);
            send (conn_socket, SendBuff, sizeof(SendBuff), 0);
            //Se usa un recv para saber si ha ganado o perdido el otro jugador. Recibo el estadoTableroEnemigo
            recv (conn_socket, RecvBuff, sizeof(RecvBuff), 0);
            estadoTableroEnemigo = RecvBuff[0];
            switch(estadoTablero){
                case 'p':
                    if(estadoTableroEnemigo=='g'){
                        printf("\n\nHaz Perdido el Juego Naval, ya no tienes Tiradas. El Otro Jugador Ha Ganado");
                        printf("\n\n\t*****************TABLERO ENEMIGO FINAL*****************\n");
                        muestraTableroEnemigo(&tablero);
                        printf("\n\n\t*****************TABLERO TUYO FINAL*****************\n");
                        muestraTablero(&tablero);
                        opcion = 6;
                    }else{
                        if(estadoTableroEnemigo=='p'){
                            printf("\n\nHaz Quedado Empatado con le Otro Jugador");
                            printf("\n\n\t*****************TABLERO ENEMIGO FINAL*****************\n");
                            muestraTableroEnemigo(&tablero);
                            printf("\n\n\t*****************TABLERO TUYO FINAL*****************\n");
                            muestraTablero(&tablero);
                            opcion = 6;
                        }
                    }
                    break;
                case 'n':
                    if(estadoTableroEnemigo=='g'){
                        printf("\n\nHaz Perdido el Juego Naval. El Otro Jugador Ha Ganado");
                        printf("\n\n\t*****************TABLERO ENEMIGO FINAL*****************\n");
                        muestraTableroEnemigo(&tablero);
                        printf("\n\n\t*****************TABLERO TUYO FINAL*****************\n");
                        muestraTablero(&tablero);
                        opcion = 6;
                    }
                    break;
            }
        }
    }while(opcion!=6);
}

