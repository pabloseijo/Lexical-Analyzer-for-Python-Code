/**
 * @author Pablo Seijo García
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones de sistema de entrada
*/

#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"

char buffer[BUFF_SIZE]; // Búfer para almacenar caracteres "devueltos"
int bufferTop = -1;     // Indica el último carácter "devuelto" en el búfer

//--------------------------- Funciones Públicas ------------------------------

/**
 * @brief Función que devuelve el siguiente carácter del archivo o del búfer de "devueltos". 
 *         Esto se debe a que como se pueden leer caracteres por adelantado, si no forman 
 *         parte de un token, se pueden devolver para que sean leídos por el siguiente componente léxico
 * @param c: carácter a devolver
*/
void devolverCaracter(char c) {

    //Si el buffer no está lleno se almacena el carácter para ser leído posteriormente
    if(bufferTop < BUFF_SIZE - 1) {

        //Incrementamos el bufferTop (índice del último carácter almacenado en el buffer)
        bufferTop++;

        // Almacena el carácter en el buffer
        buffer[bufferTop] = c;

    } else {

        //Si por alguna razón el buffer está lleno, se muestra un mensaje de error y se finaliza el programa
        fprintf(stderr, "Error: buffer de ungetChar lleno\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Función que lee los caracteres del archivo y los devuelve uno a uno
 * @param file: puntero al archivo
*/
char siguienteCaracter(FILE *file) {

    if(bufferTop >= 0) {
        // Devuelve el último carácter "devuelto" y luego decrementa el bufferTop
        return buffer[bufferTop--]; 

    } else {
        // De lo contrario, lee el siguiente carácter del archivo
        return fgetc(file);
    }
}
