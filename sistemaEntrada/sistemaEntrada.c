/**
 * @file sistemaEntrada.c
 * @author Pablo Seijo García
 * @date 27/02/2024
 * @brief Implementacion de las funciones del sistema de entrada
 * 
 * Lee los caracteres del archivo uno a uno y los va devolviendo,
 * si un caracter no es procesado, se devuelve para poder ser leído
 * de nuevo
 */

#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"

typedef struct dobleBuffering{
    char bufferA[BUFF_SIZE];
    char bufferB[BUFF_SIZE];
    char *inicioLexema;
    char *delantero; // Apunta los caracteres procesados
} dobleBuffering; 

dobleBuffering dobleCentinela;

// Funciones privadas
void cargarBloque(FILE *file);
int delanteroEnBufferA();
int inicioEnBufferA();
int delanteroEnBufferB();
int inicioEnBufferB();

// Inicializa la estructura de doble centinela y carga el primer bloque
void inicializarDobleCentinela (FILE *file){

    dobleCentinela.bufferA[BUFF_SIZE - 1] = EOF;
    dobleCentinela.bufferB[BUFF_SIZE - 1] = EOF;

    dobleCentinela.inicioLexema = dobleCentinela.bufferA;
    dobleCentinela.delantero = dobleCentinela.bufferA;

    cargarBloque(file);
}

// Mete un caracter leído y no procesado en el buffer
void retrocederCaracter() {

    if( (delanteroEnBufferA() && dobleCentinela.delantero > dobleCentinela.bufferA) || (delanteroEnBufferB() && dobleCentinela.delantero > dobleCentinela.bufferB) ){
        dobleCentinela.delantero--;

    } else {

        if (inicioEnBufferA()) {
            dobleCentinela.delantero = dobleCentinela.bufferA + BUFF_SIZE - 2;

        } else {
            dobleCentinela.delantero = dobleCentinela.bufferB + BUFF_SIZE - 2;
        }
    }

}

// Lee los caracteres del archivo y los devuelve uno a uno
char siguienteCaracter(FILE *file) {

    char charLeido;

    if(*dobleCentinela.delantero != EOF){
        charLeido = *dobleCentinela.delantero;
        dobleCentinela.delantero++;

        return charLeido;

    } else {
        if(dobleCentinela.delantero == dobleCentinela.bufferA + BUFF_SIZE - 1  
            || dobleCentinela.delantero == dobleCentinela.bufferB + BUFF_SIZE - 1){

            cargarBloque(file);

            charLeido = *dobleCentinela.delantero;
            dobleCentinela.delantero++;

            return charLeido;

        } else {
            return EOF;
        }
    }
    
}

// Devuelve el lexema leído hasta el momento
char *devolverLexema(){

    char *lexemaDevuelto;
    int longitudLexema;
    
    if ( (delanteroEnBufferA() && inicioEnBufferA()) && (delanteroEnBufferB() && inicioEnBufferB()) ) {
        longitudLexema = dobleCentinela.delantero - dobleCentinela.inicioLexema;
  
    } else {  // Los punteros están en diferentes buffers
        if (inicioEnBufferA()) {
            longitudLexema = ( (dobleCentinela.bufferA + BUFF_SIZE - 2) - dobleCentinela.inicioLexema) + (dobleCentinela.delantero - dobleCentinela.bufferB);

        } else { // Inicio lexema en Buffer B
            longitudLexema = ( (dobleCentinela.bufferB + BUFF_SIZE - 2) - dobleCentinela.inicioLexema) + (dobleCentinela.delantero - dobleCentinela.bufferA);
        }
    }

    if ( (lexemaDevuelto = (char *) malloc (sizeof(char) * longitudLexema)) == NULL) {
        fprintf(stderr, "ERROR sistemaEntrada.c: no se pudo reservar memoria para el lexema devuelto\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i <= longitudLexema; i++) {
        lexemaDevuelto[i] = *(dobleCentinela.inicioLexema + i);
    }

    dobleCentinela.inicioLexema = dobleCentinela.delantero;

    return lexemaDevuelto;
}

// Mueve el puntero inicioLexema al puntero delantero
void moverInicioLexemaADelantero() {
    dobleCentinela.inicioLexema = dobleCentinela.delantero;
}

//------------------------------------------ FUNCIONES PRIVADAS --------------------------------------------------

// Carga un bloque con los siguientes char
void cargarBloque(FILE *file){

    // fread nos permite leer un bloque de tamaño BUFF_SIZE del arhcivo
    // buffer(A y B) apunta a la primera posición del array
    if(dobleCentinela.delantero == dobleCentinela.bufferA + BUFF_SIZE - 1){
        fread(dobleCentinela.bufferB, sizeof(char), BUFF_SIZE, file);
        dobleCentinela.delantero = dobleCentinela.bufferB;

    } else {
        fread(dobleCentinela.bufferA, sizeof(char), BUFF_SIZE, file);
        dobleCentinela.delantero = dobleCentinela.bufferA;

    }
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
