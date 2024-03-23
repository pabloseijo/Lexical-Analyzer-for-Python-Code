/**
 * @file tablaHash.h
 * @date 21/02/2024
 * @brief Define una estructura y operaciones básicas para manejar una 
 * tabla de hash para tokens.
 * 
 * La tabla utiliza encadenamiento para resolver colisiones y se 
 * redimensiona dinámicamente para mantener un factor de carga óptimo.
*/

#ifndef TABLAHASH_H 
#define TABLAHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Estructura para almacenar componentes léxicos.
typedef struct token {
    int componente;  // Código numérico del componente léxico.
    char *lexema;    // Representación textual del token.
    struct token *next; // Para manejar colisiones mediante listas enlazadas.
} token;

// Alias para mejorar la legibilidad del código al trabajar con la tabla de hash. 
// El doble puntero se debe a que es un array de punteros a estructura.
typedef token **hashTable;

/**
 * Inicializa la tabla de hash con un tamaño especificado.
 * Reserva memoria para la tabla y la inicializa a NULL.
 * @param tabla Puntero a la tabla de hash.
 * @param size Tamaño de la tabla.
 * @return 1 si la inicialización fue exitosa, 0 en caso de error.
 */
int initHashTable(hashTable *tabla, int size);


/**
 * Libera la memoria asociada con la tabla de hash y sus elementos.
 * @param tabla Puntero a la tabla de hash.
 * @return Siempre devuelve 1.
 */
int deleteHashTable(hashTable tabla);

/**
 * Redimensiona la tabla de hash a un nuevo tamaño.
 * @param tabla Puntero a la tabla de hash.
 * @param newSize Nuevo tamaño de la tabla.
 * @return 1 si la redimensión fue exitosa, 0 en caso de error.
 */
int resizeHashTable(hashTable *tabla, int newSize);

/**
 * Imprime la tabla hash con cada lexema y su componente léxico.
 * @param tabla: tabla de hash que se imprimirá
*/
void printTable(hashTable tabla);

/**
 * Inserta un nuevo token en la tabla de hash.
 * Si el factor de carga supera 0.75, se redimensiona la tabla.
 * @param tabla Puntero a la tabla de hash.
 * @param lexema Lexema del token.
 * @param componente Componente léxico del token.
 * @return 1 si la inserción fue exitosa, 0 en caso contrario.
 */
int insertToken(hashTable *tabla, char *lexema, int componente);

/**
 * Busca un token en la tabla de hash por su lexema.
 * @param tabla Tabla de hash.
 * @param lexema Lexema del token a buscar.
 * @return Componente léxico del token si se encuentra, 0 si no se encuentra.
 */
int searchTokenComponent(hashTable tabla, char *lexema);

/**
 * Elimina un token de la tabla de hash.
 * @param tabla Tabla de hash.
 * @param lexema Lexema del token a eliminar.
 * @return 1 si el token fue eliminado correctamente, 0 si no se encontró.
 */
int deleteToken(hashTable tabla, char *lexema);

/**
 * Modifica el componente léxico de un token existente en la tabla de hash.
 * @param tabla Puntero a la tabla de hash.
 * @param lexema Lexema del token a modificar.
 * @param componente Nuevo componente léxico para el token.
 * @return 1 si la modificación fue exitosa, 0 si no se encontró el token.
 */
int modifyToken(hashTable *tabla, char * lexema, int componente);

#endif //TABLAHASH_H