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
    }

}

// Lee los caracteres del archivo y los devuelve uno a uno
char siguienteCaracter(FILE *file) {
    // Verifica si el delantero apunta al último caracter del buffer actual antes de EOF
    if ((delanteroEnBufferA() && (dobleCentinela.delantero == dobleCentinela.bufferA + BUFF_SIZE - 2)) ||
        (delanteroEnBufferB() && (dobleCentinela.delantero == dobleCentinela.bufferB + BUFF_SIZE - 2))) {
        cargarBloque(file); // Cargar el siguiente bloque al alcanzar el fin del buffer actual
    }
    else if (*dobleCentinela.delantero == EOF) { // Si delantero apunta a EOF, pero no está al final del buffer, es EOF real
        exit(EXIT_SUCCESS);
    } else dobleCentinela.delantero++;


    // Devolver el caracter actual
    return *dobleCentinela.delantero;
}

// Devuelve el lexema leído hasta el momento
char *devolverLexema(){

    char *lexemaDevuelto;
    int longitudLexema;

    if ( (delanteroEnBufferA() && inicioEnBufferA()) || (delanteroEnBufferB() && inicioEnBufferB()) ) {
        longitudLexema = dobleCentinela.delantero - dobleCentinela.inicioLexema;

        if ( (lexemaDevuelto = (char *) malloc (sizeof(char) * longitudLexema)) == NULL) {
            fprintf(stderr, "ERROR sistemaEntrada.c: no se pudo reservar memoria para el lexema devuelto\n");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i <= longitudLexema; i++) {
            lexemaDevuelto[i] = *(dobleCentinela.inicioLexema + i);
        }

    } else {  // Los punteros están en diferentes buffers
        if (inicioEnBufferA()) {
            longitudLexema = ( (dobleCentinela.bufferA + BUFF_SIZE - 1) - dobleCentinela.inicioLexema) + (dobleCentinela.delantero - dobleCentinela.bufferB);

            if ( (lexemaDevuelto = (char *) malloc (sizeof(char) * longitudLexema)) == NULL) {
                fprintf(stderr, "ERROR sistemaEntrada.c: no se pudo reservar memoria para el lexema devuelto\n");
                exit(EXIT_FAILURE);
            }

            char* aux = dobleCentinela.inicioLexema;
            int cont = 0;

            while(aux < dobleCentinela.bufferA + BUFF_SIZE - 1){
                lexemaDevuelto[cont++] = *aux;
                aux++;
            }

            aux = dobleCentinela.bufferB;

            while(aux <= dobleCentinela.delantero){
                lexemaDevuelto[cont++] = *aux;
                aux++;
            }

        } else { // Inicio lexema en Buffer B
            longitudLexema = ( (dobleCentinela.bufferB + BUFF_SIZE - 1) - dobleCentinela.inicioLexema) + (dobleCentinela.delantero - dobleCentinela.bufferA);

            if ( (lexemaDevuelto = (char *) malloc (sizeof(char) * longitudLexema)) == NULL) {
                fprintf(stderr, "ERROR sistemaEntrada.c: no se pudo reservar memoria para el lexema devuelto\n");
                exit(EXIT_FAILURE);
            }

            char* aux = dobleCentinela.inicioLexema;
            int cont = 0;

            while(aux < dobleCentinela.bufferB + BUFF_SIZE - 1){
                lexemaDevuelto[cont++] = *aux;
                aux++;
            }

            aux = dobleCentinela.bufferA;

            while(aux <= dobleCentinela.delantero){
                lexemaDevuelto[cont++] = *aux;
                aux++;
            }
        }
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


void imprimirBuffers() {
    printf("Buffer A:\n");
    for(int i = 0; i < BUFF_SIZE; i++) {
        // Imprime cada caracter del bufferA en formato hexadecimal
        printf(" %c |", dobleCentinela.bufferA[i]);
        // Para hacer la salida más legible, puedes agregar una nueva línea cada N caracteres
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }

    printf("\nBuffer B:\n");
    for(int i = 0; i < BUFF_SIZE; i++) {
        // Imprime cada caracter del bufferB en formato hexadecimal
        printf(" %c |", dobleCentinela.bufferB[i]);
        // Nueva línea cada N caracteres para mejorar la legibilidad
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n"); // Finaliza con una nueva línea
}

// Carga un bloque con los siguientes char
void cargarBloque(FILE *file) {
    static int cargarEnBufferA = 0;  // Cambio inicial para comenzar con el Buffer A, y luego alternar

    // Alternar entre cargar en Buffer A y Buffer B
    cargarEnBufferA = !cargarEnBufferA;

    char* bufferActual = cargarEnBufferA ? dobleCentinela.bufferA : dobleCentinela.bufferB;

    for(int i = 0; i < BUFF_SIZE; i++) {
        bufferActual[i] = '\0';
    }

    size_t itemsLeidos = fread(bufferActual, sizeof(char), BUFF_SIZE - 1, file);

    // Establecer EOF al final del contenido leído, si es necesario
    bufferActual[itemsLeidos < BUFF_SIZE - 1 ? itemsLeidos : BUFF_SIZE - 1] = EOF;

    // Ajustar el puntero delantero al inicio del nuevo buffer
    dobleCentinela.delantero = bufferActual;

    imprimirBuffers();
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
