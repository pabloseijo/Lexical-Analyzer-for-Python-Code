/**
 * @author Pablo Seijo García
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones de la tabla de hash
*/

#ifndef TABLAHASH_H 
#define TABLAHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256 // Ponemos un tamaño máximo para los lexemas

//--------------------------- Estructura ------------------------------

/**
 *  @brief Estructura del token que constituye el componente léxico
 *  @param componente: codigo del componente lexico
 *  @param lexema: cadena de caracteres que representa el lexema
 *  @param next: puntero al siguiente token (Se usa para el encadenamiento en la tabla hash)
*/
typedef struct token {
    int componente;
    char* lexema;
    struct token *next;
} token;

// Creamos un tipo hashTable para simplificar el manejo de la tabla de hash
typedef token **hashTable; // El puntero a puntero se debe a que el tamaño de la tabla se determina en tiempo de ejecución

//--------------------------- Inicialización ------------------------------

/**
 * @brief Función que inicializa la tabla de hash
 * @param tabla: tabla de hash que se inicializará
 * @param size: tamaño de la tabla de hash
 * @return 1 si se ha inicializado correctamente, 0 si no se ha podido inicializar
*/
int initHashTable(hashTable *tabla, int size);


/**
 * @brief Función que libera la memoria asociada con la tabla de hash
 * @param tabla: tabla de hash que se liberará
 * @return 1 si se ha liberado correctamente, 0 si no
*/
int deleteHashTable(hashTable *tabla);

/**
 * @brief Función que redimensiona la tabla de hash
 * @param tabla: tabla de hash que se redimensionará
 * @param newSize: nuevo tamaño de la tabla de hash
 * @return 1 si se ha redimensionado correctamente, 0 si no
*/
int resizeHashTable(hashTable *tabla, int newSize);

//------------------------------- Funciones Auxiliares ------------------------------

/**
 * @brief Función que imprime la tabla hash
 * @param tabla: tabla de hash que se imprimirá
*/
void printTable(hashTable tabla);


//--------------------------- Funciones de Interacción ------------------------------

/**
 * @brief Función que inserta un token en la tabla de hash por el método de encadenamiento
 * @param tabla: tabla de hash en la que se insertará el token
 * @param lexema: lexema que se insertará
 * @param componente: componente léxico que se insertará
 * @return true si se ha insertado correctamente, false si no
*/
int insertToken(hashTable tabla, char *lexema, int componente);

/**
 * @brief Función que busca un token en la tabla de hash
 * @param tabla: tabla de hash en la que se buscará el token
 * @param lexema: lexema que se buscará
 * @return token si se ha encontrado, NULL si no
*/
token *searchToken(hashTable tabla, char *lexema);

/**
 * @brief Función que elimina un token en la tabla de hash
 * @param tabla: tabla de hash en la que se eliminará el token
 * @param lexema: lexema que se eliminará
 * @return 1 si se ha eliminado correctamente, 0 si no esta en la tabla
*/
int deleteToken(hashTable tabla, char *lexema);

/**
 * @brief Función que modifica el componente del token a modificar a la tabla hash
 * @param tabla: tabla de hash en la que se modificará el token
 * @param t: token que se modificará
 * !IMPORTANTE: Esta función solo se usará en caso de que se modifique el componente léxico de un token
*/
int modifyToken(hashTable tabla, char * lexema, int componente);

#endif //TABLAHASH_H