/**
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones de gestionErrores.c
 * 
 * Este archivo contiene las cabeceras de las funciones de gestionErrores.c
*/

#ifndef GESTIONERRORES_H
#define GESTIONERRORES_H


/**
 * No se encuentra el fichero y se sale del programa
 * @param nombreFichero Nombre del fichero que no se ha encontrado
 */
void ficheroNoEncontrado(char *nombreFichero);

/**
 * Se ha excedido el tamaño máximo del lexema
 */
void tamLexemaExcedido();

/**
 * No se reconoce el caracater
 * @param c Caracter no reconocido
 */
void caracterNoReconocido(char c);

/**
 * Se ha excedido el tamaño máximo de la tabla de símbolos
 */
void numParametrosIncorrecto();


#endif