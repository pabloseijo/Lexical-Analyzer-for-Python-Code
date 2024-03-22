/**
 * @file sistemaEntrada.c
 * @date 27/02/2024
 * @brief Implementacion de las funciones del sistema de entrada
 *
 * Lee los caracteres del archivo uno a uno y los va devolviendo,
 * si un caracter no es procesado, se devuelve para poder ser leído
 * de nuevo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistemaEntrada.h"
#include "../gestionErrores/gestionErrores.h"

typedef struct dobleBuffering{
    char bufferA[BUFF_SIZE];
    char bufferB[BUFF_SIZE];
    char *inicioLexema;
    char *delantero; // Apunta los caracteres procesados
} dobleBuffering;

dobleBuffering dobleCentinela;
int charRetrocedidoAlInicioBuffer = 0;

// Funciones privadas
void cargarBloque(FILE *file);
int delanteroEnBufferA();
int inicioEnBufferA();
int delanteroEnBufferB();
int inicioEnBufferB();

// Inicializa la estructura de doble centinela y carga el primer bloque
void inicializarDobleCentinela (FILE *file){

    dobleCentinela.inicioLexema = dobleCentinela.bufferA;
    dobleCentinela.delantero = dobleCentinela.bufferA;

    cargarBloque(file);
}

// Mete un caracter leído y no procesado en el buffer
void retrocederCaracter() {

    if( (delanteroEnBufferA() && dobleCentinela.delantero > dobleCentinela.bufferA) || (delanteroEnBufferB() && dobleCentinela.delantero > dobleCentinela.bufferB) ){
        dobleCentinela.delantero--;

    } else if (dobleCentinela.delantero == dobleCentinela.bufferA || dobleCentinela.delantero == dobleCentinela.bufferB) {
        charRetrocedidoAlInicioBuffer = 1;
    }

}

// Lee los caracteres del archivo y los devuelve uno a uno
char siguienteCaracter(FILE *file) {

    if (charRetrocedidoAlInicioBuffer) {
        charRetrocedidoAlInicioBuffer = 0; // Restablecer después de reconocer el estado
        return *dobleCentinela.delantero; // Devuelve el carácter retrocedido sin mover el delantero
    }

    if ((delanteroEnBufferA() && (dobleCentinela.delantero == dobleCentinela.bufferA + BUFF_SIZE - 2)) ||
        (delanteroEnBufferB() && (dobleCentinela.delantero == dobleCentinela.bufferB + BUFF_SIZE - 2))) {

        cargarBloque(file); 

    } else if (*dobleCentinela.delantero == EOF) { 
        exit(EXIT_SUCCESS);
        
    } else dobleCentinela.delantero++;

    return *dobleCentinela.delantero;
}

// Devuelve el lexema leído hasta el momento
char *devolverLexema(){

    char *lexemaDevuelto;
    int longitudLexema;

    //Sumamos 1 al final de la longitud para incluir el caracter actual en la resta de punteros
    if ( (delanteroEnBufferA() && inicioEnBufferA()) || (delanteroEnBufferB() && inicioEnBufferB()) ) {
        longitudLexema = dobleCentinela.delantero - dobleCentinela.inicioLexema + 1;

        if ( (lexemaDevuelto = (char *) malloc (sizeof(char) * (longitudLexema + 1))) == NULL) {
            fprintf(stderr, "ERROR sistemaEntrada.c: no se pudo reservar memoria para el lexema devuelto\n");
            exit(EXIT_FAILURE);

        } else if(longitudLexema > TAM_LEXEMA){ // Lanzamos la excepción
            
            tamLexemaExcedido(); 

            // Devuelve la primera parte del lexema que quepa en el tamaño máximo
            for(int i = 0; i <= TAM_LEXEMA; i++) {
                lexemaDevuelto[i] = *(dobleCentinela.inicioLexema + i);
            }

            return lexemaDevuelto;
        }

        memset(lexemaDevuelto, 0, longitudLexema + 1); // Inicializamos a 0 el string

        for(int i = 0; i <= longitudLexema; i++) {
            lexemaDevuelto[i] = *(dobleCentinela.inicioLexema + i);
        }

    } else {  // Los punteros están en diferentes buffers
        if (inicioEnBufferA()) {
            longitudLexema = ( (dobleCentinela.bufferA + BUFF_SIZE - 1) - dobleCentinela.inicioLexema) + (dobleCentinela.delantero - dobleCentinela.bufferB) + 1;

            char* aux = dobleCentinela.inicioLexema;
            int cont = 0;

            if ( (lexemaDevuelto = (char *) malloc (sizeof(char) * (longitudLexema + 1))) == NULL) {
                fprintf(stderr, "ERROR sistemaEntrada.c: no se pudo reservar memoria para el lexema devuelto\n");
                exit(EXIT_FAILURE);

            } else if(longitudLexema > TAM_LEXEMA){
                tamLexemaExcedido();

                memset(lexemaDevuelto, 0, longitudLexema + 1); // Inicializamos a 0 el string

                // Devuelve la primera parte del lexema que quepa en el tamaño máximo
                if(TAM_LEXEMA < (dobleCentinela.bufferA + BUFF_SIZE - 1) - dobleCentinela.inicioLexema){
                    for(int i = 0; i < TAM_LEXEMA; i++) {
                        lexemaDevuelto[i] = *(dobleCentinela.inicioLexema + i);
                    }
                } else {

                    while(aux < dobleCentinela.bufferA + BUFF_SIZE - 1){
                        lexemaDevuelto[cont++] = *aux;
                        aux++;
                    }

                    if(!charRetrocedidoAlInicioBuffer){
                        aux = dobleCentinela.bufferB;

                        while(cont < TAM_LEXEMA){
                            lexemaDevuelto[cont++] = *aux;
                            aux++;
                        }
                    } 
                }

                return lexemaDevuelto;
            } else {

                memset(lexemaDevuelto, 0, longitudLexema + 1); // Inicializamos a 0 el string

                while(aux < dobleCentinela.bufferA + BUFF_SIZE - 1){
                    lexemaDevuelto[cont++] = *aux;
                    aux++;
                }

                if(!charRetrocedidoAlInicioBuffer){
                    aux = dobleCentinela.bufferB;

                    while(aux <= dobleCentinela.delantero){
                        lexemaDevuelto[cont++] = *aux;
                        aux++;
                    }
                } 
            }

        } else { // Inicio lexema en Buffer B
            longitudLexema = ( (dobleCentinela.bufferB + BUFF_SIZE - 1) - dobleCentinela.inicioLexema) + (dobleCentinela.delantero - dobleCentinela.bufferA) + 1;

            char* aux = dobleCentinela.inicioLexema;
            int cont = 0;

          

            if ( (lexemaDevuelto = (char *) malloc (sizeof(char) * (longitudLexema + 1))) == NULL) {
                fprintf(stderr, "ERROR sistemaEntrada.c: no se pudo reservar memoria para el lexema devuelto\n");
                exit(EXIT_FAILURE);

            } else if(longitudLexema > TAM_LEXEMA){
                tamLexemaExcedido();
                memset(lexemaDevuelto, 0, longitudLexema + 1); // Inicializamos a 0 el string

                // Devuelve la primera parte del lexema que quepa en el tamaño máximo
                if(TAM_LEXEMA < (dobleCentinela.bufferA + BUFF_SIZE - 1) - dobleCentinela.inicioLexema){
                    for(int i = 0; i < TAM_LEXEMA; i++) {
                        lexemaDevuelto[i] = *(dobleCentinela.inicioLexema + i);
                    }
                } else {

                    while(aux < dobleCentinela.bufferA + BUFF_SIZE - 1){
                        lexemaDevuelto[cont++] = *aux;
                        aux++;
                    }

                    if(!charRetrocedidoAlInicioBuffer){
                        aux = dobleCentinela.bufferB;

                        while(cont < TAM_LEXEMA){
                            lexemaDevuelto[cont++] = *aux;
                            aux++;
                        }
                    }
                }

            } else {

                memset(lexemaDevuelto, 0, longitudLexema + 1); // Inicializamos a 0 el string

                while(aux < dobleCentinela.bufferB + BUFF_SIZE - 1){
                    lexemaDevuelto[cont++] = *aux;
                    aux++;
                }

                if(!charRetrocedidoAlInicioBuffer){
                    aux = dobleCentinela.bufferA;

                    while(aux <= dobleCentinela.delantero){
                        lexemaDevuelto[cont++] = *aux;
                        aux++;
                    }
                } 
            }
        }
    }

    lexemaDevuelto[longitudLexema] = '\0';

    moverInicioLexemaADelantero();
    return lexemaDevuelto;
}

// Mueve el puntero inicioLexema al puntero delantero
void moverInicioLexemaADelantero() {
    dobleCentinela.inicioLexema = dobleCentinela.delantero;
}

// Devuelve el caracter que está en el puntero delantero
char devolverDelantero(){
    return *dobleCentinela.delantero;
}

//------------------------------------------ FUNCIONES PRIVADAS --------------------------------------------------

// Carga un bloque con los siguientes char
void cargarBloque(FILE *file) {
    static int cargarEnBufferA = 0;  // Static hace que la variable mantenga su valor entre llamadas a la función

    cargarEnBufferA = !cargarEnBufferA;

    char* bufferActual = cargarEnBufferA ? dobleCentinela.bufferA : dobleCentinela.bufferB;

    for(int i = 0; i < BUFF_SIZE; i++) {
        bufferActual[i] = '\0';
    }

    size_t itemsLeidos = fread(bufferActual, sizeof(char), BUFF_SIZE - 1, file);

    bufferActual[itemsLeidos < BUFF_SIZE - 1 ? itemsLeidos : BUFF_SIZE - 1] = EOF;

    dobleCentinela.delantero = bufferActual;
}


// Devuelve 1 si delanreo está en el bufferA, 0 en caso contrario
int delanteroEnBufferA(){
    return (dobleCentinela.delantero >= dobleCentinela.bufferA && dobleCentinela.delantero <= dobleCentinela.bufferA + BUFF_SIZE - 2);
}

// Devuelve 1 si inicioLexema está en el bufferA, 0 en caso contrario
int inicioEnBufferA(){
    return (dobleCentinela.inicioLexema >= dobleCentinela.bufferA && dobleCentinela.inicioLexema <= dobleCentinela.bufferA + BUFF_SIZE - 2);
}

// Devuelve 1 si delantero está en el bufferB, 0 en caso contrario
int delanteroEnBufferB(){
    return (dobleCentinela.delantero >= dobleCentinela.bufferB && dobleCentinela.delantero <= dobleCentinela.bufferB + BUFF_SIZE - 2);
}

// Devuelve 1 si inicioLexema está en el bufferB, 0 en caso contrario
int inicioEnBufferB(){
    return (dobleCentinela.inicioLexema >= dobleCentinela.bufferB && dobleCentinela.inicioLexema <= dobleCentinela.bufferB + BUFF_SIZE - 2);
}

