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

char buffer[BUFF_SIZE]; // Búfer para almacenar caracteres "devueltos"
int ultimoCharBuffer = -1;     // Indica el último carácter "devuelto" en el búfer

// Mete un caracter leído y no procesado en el buffer
void devolverCaracter(char charDevuelto) {

    if(ultimoCharBuffer < BUFF_SIZE - 1) {
        
        ultimoCharBuffer++;
        buffer[ultimoCharBuffer] = charDevuelto;

    } else {
        fprintf(stderr, "Error sistemaEntrada.c: buffer de charDevuelto lleno\n");
        exit(EXIT_FAILURE);
    }
}

// Lee los caracteres del archivo y los devuelve uno a uno
char siguienteCaracter(FILE *file) {

    if(ultimoCharBuffer >= 0) {
        char charDevuelto = buffer[ultimoCharBuffer];
        ultimoCharBuffer--;

        return charDevuelto; 

    } else {
        return fgetc(file);
    }
}
