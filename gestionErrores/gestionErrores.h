/**
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones de gestionErrores.c
*/

#ifndef GESTIONERRORES_H
#define GESTIONERRORES_H


/**
 * @brief No se encuentra el fichero y se sale del programa
 * 
 * @param nombreFichero Nombre del fichero que no se ha encontrado
 */
void ficheroNoEncontrado(char *nombreFichero);

/**
 * @brief Se ha excedido el tamaño máximo del lexema
 * 
 */
void tamLexemaExcedido();

/**
 * @brief No se reconoce el caracater
 * 
 * @param c Caracter no reconocido
 */
void caracterNoReconocido(char c);

#endif