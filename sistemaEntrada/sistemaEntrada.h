/**
 * @file sistemaEntrada.c
 * @author Pablo Seijo García
 * @date 27/02/2024
 * @brief Definición de las funciones del sistema de entrada
 * 
 * Define el tamaño del búfer y las funciones para devolver y leer caracteres
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 100

/**
 * @brief Mete un caracter leído y no procesado en un buffer para ser leído de nuevo
 * @param charDevuelto: carácter a devolver
*/
void devolverCaracter(char charDevuelto) ;

/**
 * @brief Lee los caracteres del archivo y los devuelve uno a uno
 * @param file: puntero al archivo
*/
char siguienteCaracter(FILE *file);