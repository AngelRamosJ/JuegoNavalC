#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED
#define ORIENTACION_HORIZONTAL 0
#define ORIENTACION_VERTICAL 1
#define TAM_MAX_ENTEROS 4
#define LONGITUD_MAX_NAVE 5
#define VARIABLES_CONFIGURACION 5
#define ARCHIVO_CONFIGURACION "D:\\Configuracion_Juego_Naval.txt" //CAMBIAR LA RUTA DEPENDIENDO DE DONDE ESTE EL ARCHIVO
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
/*******************DECLARACIONES DE ESTRUCTURAS DE DATOS*******************/
typedef struct Coordenada{
    int y;
    int x;
}Coordenada;

typedef struct Casilla{
    Coordenada coordenada;
    int estado;
}Casilla;

typedef struct Nave{
    int longitud;
    int orientacion;
    Casilla *casilla;
    int damage;
}Nave;

typedef struct nodoListaSimple{
    Nave nave;
    struct nodoListaSimple *siguiente;
}nodoListaSimple;

typedef struct lista{
    nodoListaSimple *Inicio;
    nodoListaSimple *Final;
    int (*inserta)(struct lista *, const Nave *);
    int (*buscaCasilla)(const struct lista *, const Casilla *);
    Nave * (*buscaNave)(const struct lista *, const Coordenada *);
}lista;

typedef struct Configuracion{
    int tamX;
    int tamY;
    int numNaves;
    int numTiradas;
    //int longitudMaxNave;
    int* numTipoNaves;
}Configuracion;

typedef struct Tablero{
    Configuracion config;
    lista naves;
    Casilla *tiradas;
    int contadorTiradas;
}Tablero;
/*******************DECLARACIONES DE LIBRERIAS*******************/
//NAVES
int incrementaDamage(Nave *n);
void llenaNave(Nave *n, const unsigned int tamX, const unsigned int tamY);
//TABLERO
void creaTablero(Tablero *t, const Configuracion *c);
int inicializaConfiguracion(Configuracion *c);
void estableceConfiguracion(Configuracion *c, const unsigned int nivel);
int tiradaTablero(Tablero *t, const Coordenada *coord);
void guardaTirada(Tablero *t, const Casilla *tirada);
int existeTirada(const Tablero *t, const Casilla *tirada);
int esGanador(const Tablero *t);
int esPerdedor(const Tablero *t);
char confirmaEstadoTablero(const Tablero *t);
//VALIDACI�N
int validaRango(const unsigned int valor, const unsigned int valorMin, const unsigned int valorMax);
int validaRangoTablero(const Nave *n, const unsigned int tamX, const unsigned int tamY);
int validaTraslapeNave(const Nave *nave, const lista *naves);
//LECTURA
int leerEnteroPositivo();
//CALCULOS
int sumaElementosVect(const int *elementos, const unsigned int numElementos);
void iniciaVector(int *v, const unsigned int tam);
char** creaMatriz(const unsigned int n, const unsigned int m);
void liberarMemoriaMatriz(char **matriz, const unsigned int filas);
//LISTAS
void iniciaLista(lista *l);
void insercionAlFinal(lista *l, const Nave *nave);
void primeraInsercion(lista *l, const Nave *nave);
int esListaVacia(const lista *l);
int insercionLista(lista *l, const Nave *nave);
int buscaCasillaNave(const lista *l, const Casilla *casilla);
Nave * buscaNaveCoordenada(const lista *l, const Coordenada *coordenada);
//ARCHIVOS
int contarLineas(const char *nomArch);
int leeConfiguracion(const char *nomArch, char *conf, const unsigned int nivel);
int leeConfiguracionEspecifica(const char *nomArch, char **matriz, const unsigned int nivel);
int leerTodoArchivo(const char *nomArch,char **matriz);
//VISTAS
void muestraConfiguracion();
void muestraConfiguracionEspecifica(const unsigned int nivel);
void muestraTablero(const Tablero *t);
void muestraTableroOriginal(const Tablero *t);
void muestraTableroEnemigo(const Tablero *t);
int menuJuegoNaval();
void muestraInstrucciones(const Tablero *t);
#endif // DATA_H_INCLUDED


