/**
 * @file tablaSimbolos.h
 * @date 26/02/2024
 * @brief Definicion de una tabla de simbolos para la abstraccion de la 
 * tabla hash
 * 
 * Mejora la abstraccion de la implementacion de la tabla hash, permitiendo
 * al usuario trabajar con la tabla de simbolos sin tener que entender la 
 * tabla hash
 */

#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../definiciones.h"
#include "../tablasHash/tablaHash.h"

/**
 * La estructura token es la siguiente:
 * 
 * typedef struct token {
 *      int componente; // Código numérico del componente léxico.
 *      char* lexema;  // Representación textual del token.
 *      struct token *next; // Para la implementacion de las listas enlazadas
 * } token;
*/

//------------------------------- Funciones de Estructura ------------------------------

/**
 * @brief Función que inicializa la tabla de símbolos, introduciendo las palabras reservadas del lenguaje
 * @param tabla: tabla de símbolos que se inicializará
 * @return 1 si se ha inicializado correctamente, 0 si no se ha podido inicializar
*/
int inicializarTabla(hashTable *tabla);

/**
 * @brief Función que destruye la tabla de símbolos
 * @param tabla: tabla de símbolos que se destruirá
*/
void destruirTabla(hashTable tabla);

/**
 * @brief Función que imprime la tabla de símbolos
 * @param tabla: tabla de símbolos que se imprimirá
*/
void imprimirTabla(hashTable tabla);


//------------------------------- Funciones de Interacción ------------------------------

/**
 * @brief Función que inserta un elemento en la tabla de símbolos
 * @param t: variable de tipo token que contiene el componente léxico a insertar
 * @param tabla: tabla de símbolos en la que se insertará el componente léxico
 * @return 1 si se ha insertado correctamente, 0 si no se ha podido insertar
*/
int insertarElemento(token t, hashTable *tabla);

/**
 * @brief Función que modifica un elemento en la tabla de símbolos es importante entender que el 
 *        lexema no se puede modificar, por que si no no se podria encontrar el token (Hay que 
 *        pasarle el componente ya modificado)
 * @param t: variable de tipo token que contiene el componente léxico a modificar
 * @param tabla: tabla de símbolos en la que se modificará el componente léxico
*/
int modificarElemento(token t, hashTable tabla);

/**
 * @brief Función que busca un elemento en la tabla de símbolos. 
 * @param t: variable de tipo token que contiene el componente léxico a buscar, y que será 
 *          comparado con los elementos de la tabla y modificara el componente léxico de t
 * @param tabla: tabla de símbolos en la que se buscará el componente léxico
 * @return componente si se ha encontrado, 0 si no
*/
int buscarElemento(char *lexema, hashTable tabla);

/**
 * @brief Función que borra un elemento en la tabla de símbolos
 * @param lexema: lexema que se eliminará
 * @param tabla: tabla de símbolos en la que se eliminará el componente léxico
 * @return 1 si se ha eliminado correctamente, 0 si no se ha podido eliminar
*/
int borrarElemento(char *lexema, hashTable tabla);


#endif	// TABLASIMBOLOS_H