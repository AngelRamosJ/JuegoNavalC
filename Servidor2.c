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
 * ESTE PRINICPAL, HACE LA FUNCIONALIDAD DEL SERVIDOR, EN ESTE CASO, DEL SEGUNDO JUGADOR
 * ESTE PROGRAMA REALIZA EL JUEGO NAVAL MEDIANTE EL USO DE LA IP 127.0.0.1 (LOCALHOST), PERO SE PUEDE CAMBIAR PARA REALIZAR CON UNA IP PRIVADA,
 * LA IMPLEMENTACION DE ESTE PROGRAMA PARA UNA IP PUBLICA TIENE UN GRADO MAYOR DE DIFICULTAD, POR LO QUE, SE QUEDA PARA FUTURO MANTENIMIENTO
 */
/*NOTAS: Los mensajes pueden ser numeros del 1 al tamaï¿½o del tablero (ya que es un tablero cuadrado en las configuraciones)
         Estos mensajes seran usados para las coordenadas y deberan ir pasados con un separador, en este caso ' ' (espacio)
         Ademï¿½s, g - indica que ha ganado el otro jugador
                 p - indica que ha perdido el otro jugador
                 n - indica que no ha pasado nada relevante
                 1 - indica que la confirmacion es positiva
                 2 - indica que la confirmacion es negativa*/
char SendBuff[512],RecvBuff[512];
int main(){
    WSADATA wsaData;
    SOCKET conn_socket,comm_socket;
    SOCKET comunicacion;
    struct sockaddr_in server;
    struct sockaddr_in client;
    struct hostent *hp;
    int resp,stsize;
    Configuracion conf;
    Tablero tablero;
    Casilla tirada;
    Coordenada tiradaEnemiga;
    char estadoTablero = 'n', estadoTableroEnemigo;
    char aux[512] = "";
    int opcion = 4, confirmacion,nivel;
    printf("\t\t******************JUEGO NAVAL******************\n\n");
    printf("Conectando con el Configurando Servidor.......\n\n");
    //Se usa un blind y listen aqui
    //Inicializamos la DLL de sockets
    resp=WSAStartup(MAKEWORD(1,0),&wsaData);
        if(resp){
        printf("Error al inicializar socket\n\n");
        getchar();return resp;
    }
    //Obtenemos la IP que usarÃ¡ nuestro servidor...
    // en este caso localhost indica nuestra propia mÃ¡quina...
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
    // Asociamos ip y puerto al socket
    resp=bind(conn_socket, (struct sockaddr *)&server, sizeof(server));
    if(resp==SOCKET_ERROR){
        printf("Error al asociar puerto e ip al socket\n\n");
        closesocket(conn_socket);WSACleanup();
        getchar();return WSAGetLastError();
    }
    if(listen(conn_socket, 1)==SOCKET_ERROR){
        printf("Error al habilitar conexiones entrantes\n\n");
        closesocket(conn_socket);WSACleanup();
        getchar();return WSAGetLastError();
    }
    printf("Conectando con el Cliente (Primer Jugador).......\n\n");
    //Se usa un accept aqui
     // Aceptamos conexiones entrantes
    printf("Esperando conexiones entrantes... \n\n");
    stsize=sizeof(struct sockaddr);
    comm_socket=accept(conn_socket,(struct sockaddr *)&client,&stsize);
    if(comm_socket==INVALID_SOCKET){
        printf("Error al aceptar conexion entrante\n\n");
        closesocket(conn_socket);WSACleanup();
        getchar();return WSAGetLastError();
    }
    printf("Conexion entrante desde: %s\n\n", inet_ntoa(client.sin_addr));
    // Como no vamos a aceptar mÃ¡s conexiones cerramos el socket escucha
    closesocket(conn_socket);
    printf("******************Preparando la Configuracion del Juego******************\n\n");
    do{
        printf("Esperando Eleccion de Configuracion del Otro Jugador.......\n\n");
        //Se usa un recv aqui
        recv (comm_socket, RecvBuff, sizeof(RecvBuff), 0);
        nivel = atoi(RecvBuff); //Este es de prueba, no sera exactamente asi
        printf("Esta fue la Configuracion que el Otro Jugador Eligio\n\n");
        muestraConfiguracionEspecifica(nivel);
        do{
            printf("\n\nEstas Deacuerdo con la Configuracion(1. SI, 2. NO): ");
            confirmacion = leerEnteroPositivo();
        }while(confirmacion==-1 || validaRango(confirmacion,1,2)==0);
        //Se usa un send aqui para indicar que si la confirmacion es positiva o negativa
        sprintf(SendBuff, "%d", confirmacion);
        send (comm_socket, SendBuff, sizeof(SendBuff), 0);
    }while(confirmacion!=1);
    estableceConfiguracion(&conf,nivel);
    printf("\n\nEsperando a que el Otro Jugardor Acomode su Tablero.......");
    //Se usa un recv aqui para recibir la confirmacion de que el jugador acomodo el tablero
    recv (comm_socket, RecvBuff, sizeof(RecvBuff), 0);
    printf("\n\n******************Acomoda las Naves de tu Tablero******************\n");
    creaTablero(&tablero,&conf);
    //Se usa un send aqui para cconfirmar la creacion del tablero al otro jugador
    send (comm_socket, SendBuff, sizeof(SendBuff), 0);
    do{
        if(opcion == 4){
            printf("\n\nES TURNO DEL OTRO JUGADOR........");
            //Se usa un recv para dar el turno al otro usuario
            recv (comm_socket, RecvBuff, sizeof(RecvBuff), 0);
            strcpy(aux,RecvBuff);
            tiradaEnemiga.x = atoi(strtok(aux," "));
            tiradaEnemiga.y = atoi(strtok(NULL," "));
            if(tiradaEnemiga.x == 0){
                printf("\n\n\t*****************EL OTRO JUGADOR FINALIZO LA PARTIDA*****************\n");
                closesocket(comm_socket);
                WSACleanup();
                exit(0);
            }
            confirmacion = tiradaTablero(&tablero,&tiradaEnemiga);
            //Se usa un send para confirmarle al otro jugador el estado de la tirada
            sprintf(SendBuff, "%d", confirmacion);
            send (comm_socket, SendBuff, sizeof(SendBuff), 0);
            //Se usa un recv para saber si ha ganado o perdido el otro jugador. Recibo el estadoTableroEnemigo
            recv (comm_socket, RecvBuff, sizeof(RecvBuff), 0);
            estadoTableroEnemigo=RecvBuff[0];
            if(estadoTableroEnemigo=='g'){
                printf("\n\nHaz Perdido el Juego Naval. El Otro Jugador Ha Ganado");
                printf("\n\n\t*****************TABLERO ENEMIGO FINAL*****************\n");
                muestraTableroEnemigo(&tablero);
                printf("\n\n\t*****************TABLERO TUYO FINAL*****************\n");
                muestraTablero(&tablero);
                closesocket(comm_socket);
                WSACleanup();
                exit(0);
            }
        }
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
                send (comm_socket, SendBuff, sizeof(SendBuff), 0);
                //Se usa un recv aqui para confirmar la tirada
                recv (comm_socket, RecvBuff, sizeof(RecvBuff), 0);
                tirada.estado = atoi(RecvBuff); //Este solo es de prueba, el verdadero debe utilizar la informacion recibida
                guardaTirada(&tablero,&tirada);
                estadoTablero = confirmaEstadoTablero(&tablero);
                //Se usa un send para confirmar el estado de la partida
                aux[0]=estadoTablero;
                aux[1]='\0';
                strcpy(SendBuff,aux);
                send (comm_socket, SendBuff, sizeof(SendBuff), 0);
                if(estadoTablero=='g'){
                    printf("\n\nHaz Ganado el Juego Naval");
                    printf("\n\n\t*****************TABLERO ENEMIGO FINAL*****************\n");
                    muestraTableroEnemigo(&tablero);
                    printf("\n\n\t*****************TABLERO TUYO FINAL*****************\n");
                    muestraTablero(&tablero);
                    closesocket(comm_socket);
                    WSACleanup();
                    opcion = 6;
                }
                if(estadoTablero=='p'){
                    printf("\n\nHaz Quedado Empatado con le Otro Jugador");
                    printf("\n\n\t*****************TABLERO ENEMIGO FINAL*****************\n");
                    muestraTableroEnemigo(&tablero);
                    printf("\n\n\t*****************TABLERO TUYO FINAL*****************\n");
                    muestraTablero(&tablero);
                    closesocket(comm_socket);
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
                send (comm_socket, SendBuff, sizeof(SendBuff), 0);
                closesocket(comm_socket);
                WSACleanup();
                printf("\n\n******************Juego Finalizado******************");
                opcion = 6;
                break;
            default:
                printf("\n!!!!!Error Inespaerado!!!!!");
                closesocket(comm_socket);
                WSACleanup();
                opcion = 6;
        }
    }while(opcion!=6);
}

