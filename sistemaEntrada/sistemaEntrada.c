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

    dobleCentinela.inicioLexema = dobleCentinela.bufferA;
    dobleCentinela.delantero = dobleCentinela.bufferA;

    cargarBloque(file);
}

// Mete un caracter leído y no procesado en el buffer
void retrocederCaracter() {

    if( (delanteroEnBufferA() && dobleCentinela.delantero > dobleCentinela.bufferA) || (delanteroEnBufferB() && dobleCentinela.delantero > dobleCentinela.bufferB) ){
        dobleCentinela.delantero--;

    } else {
        //TODO: el puntero del archivo esta delante, gestionar esto
        if (inicioEnBufferA()) {
            dobleCentinela.delantero = dobleCentinela.bufferA + BUFF_SIZE - 2;

        } else {
            dobleCentinela.delantero = dobleCentinela.bufferB + BUFF_SIZE - 2;
        }
    }

}

// Lee los caracteres del archivo y los devuelve uno a uno
char siguienteCaracter(FILE *file) {
    // Incrementa 'delantero' solo si no apunta a EOF. Esto es para leer el próximo carácter.
    if (*dobleCentinela.delantero != EOF) {
        dobleCentinela.delantero++;
    }

    // Verifica si hemos llegado al fin de un buffer y necesitamos cargar el próximo bloque.
    if (*dobleCentinela.delantero == EOF) {
        if (dobleCentinela.delantero == dobleCentinela.bufferA + BUFF_SIZE - 1 || dobleCentinela.delantero == dobleCentinela.bufferB + BUFF_SIZE - 1) {
            cargarBloque(file);
        }
    }

    // Si después de intentar cargar un nuevo bloque aún estamos en EOF, es EOF real.
    if (*dobleCentinela.delantero == EOF) {
        return EOF;
    } else {
        return *dobleCentinela.delantero;
    }
}


// Devuelve el lexema leído hasta el momento
char *devolverLexema(){

    char *lexemaDevuelto;
    int longitudLexema;

    if ( (delanteroEnBufferA() && inicioEnBufferA()) || (delanteroEnBufferB() && inicioEnBufferB()) ) {
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
    static int cargarEnBufferA = 1;  // Empezamos cargando en bufferA. (Al ser static, mantiene su valor entre llamadas.)
    size_t itemsRead;

    if (cargarEnBufferA) {
        itemsRead = fread(dobleCentinela.bufferA, sizeof(char), BUFF_SIZE - 1, file);
        dobleCentinela.bufferA[itemsRead] = EOF;  // Marcador de EOF.
        dobleCentinela.delantero = dobleCentinela.bufferA;  // Reinicia el delantero.
        cargarEnBufferA = 0;  // Próxima carga en bufferB.
    } else {
        itemsRead = fread(dobleCentinela.bufferB, sizeof(char), BUFF_SIZE - 1, file);
        dobleCentinela.bufferB[itemsRead] = EOF;  // Marcador de EOF.
        dobleCentinela.delantero = dobleCentinela.bufferB;  // Mueve el delantero.
        cargarEnBufferA = 1;  // Próxima carga en bufferA.
    }

    // Si no se leyeron datos, hemos alcanzado el EOF real.
    if (itemsRead == 0) {
        printf("EOF real\n");
        *dobleCentinela.delantero = EOF;  // Asegurarse de que EOF esté correctamente asignado.
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
