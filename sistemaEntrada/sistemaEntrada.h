/**
 * @author Pablo Seijo García
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones de sistemaEntrada.c
*/

#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 100

//--------------------------- Funciones Públicas ------------------------------

/**
 * @brief Función que devuelve el siguiente carácter del archivo o del búfer de "devueltos". 
 *         Esto se debe a que como se pueden leer caracteres por adelantado, si no forman 
 *         parte de un token, se pueden devolver para que sean leídos por el siguiente componente léxico
 * @param c: carácter a devolver
*/
void devolverCaracter(char c) ;

/**
 * @brief Función que lee los caracteres del archivo y los devuelve uno a uno
 * @param file: puntero al archivo
*/
char siguienteCaracter(FILE *file);