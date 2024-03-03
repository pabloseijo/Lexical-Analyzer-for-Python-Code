/**
 * @author Pablo Seijo García
 * @date 01/03/2024
 * @brief Fichero que contiene las definiciones de las funciones de la tabla de símbolos
*/

/**
 * @note Decidi usar un nivel de abstracción mayor y separar la tabla de símbolos de la tabla Hash 
 * de tal manera que el usuario no tenga que preocuparse por la implementación de las funciones de 
 * la tabla hash ni del hash en sí. 
 * 
 * @note En cualquier caso la tabla hash hace uso de punteros dobles (array de structuras) para la 
 * correcta implementación dinámica de esta, por lo que puede resultar complicado la compresión de 
 * la misma. Esto hace que la abstractización de la tabla de símbolos sea necesaria.
 * 
 * @note Para facilitar tambien la comprensión de la tabla de símbolos, se muestra en un comentario
 * la estructura que compone un token, que es cada uno de los elementos de la tabla Hash, y por tanto
 * de la tabla de símbolos. La compresión de esta estructura es clave para entender el funcionamiento
 * del programa.
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
 * @brief Estructura del token que constituye el componente léxico
 * @param componente: codigo del componente lexico
 * @param lexema: cadena de caracteres que representa el lexema
 * @param next: puntero al siguiente token (Se usa para el encadenamiento en la tabla hash)
 * 
 * typedef struct token {
 *      int componente;
 *       char* lexema;
 *       struct token *next;
 *   } token;
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
void destruirTabla(hashTable *tabla);

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