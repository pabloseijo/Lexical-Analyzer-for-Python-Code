/**
 * @file sistemaEntrada.c
 * @author Pablo Seijo García
 * @date 27/02/2024
 * @brief Definición de las funciones del sistema de entrada
 * 
 * Define el tamaño del búfer y las funciones para devolver y leer caracteres.
 * Mediante la tecnica de doble centinela se implementa el doble buffering 
 * que nos permite leer el archivo a trozos.
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 12000

/**
 * Mueve el puntero delantero una posición hacia atrás
*/
void retrocederCaracter();

/**
 * Lee los caracteres del archivo y los devuelve uno a uno
 * @param file: puntero al archivo
*/
char siguienteCaracter(FILE *file);

/**
 * Inicializa la estructura de doble centinela y carga el primer bloque
 * @param file: puntero al archivo
*/
void inicializarDobleCentinela (FILE *file);

/**
 * Devuelve el lexema que se ha ido formando
 * @return string que contiene el lexema
*/
char *devolverLexema();

/**
 * Mueve el puntero delantero una posición hacia delante
*/
void moverInicioLexemaADelantero();

/**
 * Devuelve el caracter que se encuentra en el puntero delantero
*/
char devolverDelantero();