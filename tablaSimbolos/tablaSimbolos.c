/**
 * @author Pablo Seijo García
 * @date 01/03/2024
 * @brief Fichero que contiene las funciones de la tabla de símbolos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../definiciones.h"
#include "tablaSimbolos.h"
#include "../tablasHash/tablaHash.h"

//------------------------------- Funciones de Estructura ------------------------------

/**
 * @brief Función que inicializa la tabla de símbolos, introduciendo las palabras reservadas del lenguaje
 * @param tabla: tabla de símbolos que se inicializará
 * @return 1 si se ha inicializado correctamente, 0 si no se ha podido inicializar
*/
int inicializarTabla(hashTable *tabla){

    //Primero tenemos que inicializar la tabla con las keywords del lenguaje,
    //que podemos obtener de python.org en nuestro caso

    //--------------------------- Palabras reservadas ------------------------------

    char *keywords[9];

    keywords[0] = "for";
    keywords[1] = "if";
    keywords[2] = "else";
    keywords[3] = "in";
    keywords[4] = "not";
    keywords[5] = "return";
    keywords[6] = "import";
    keywords[7] = "from";
    keywords[8] = "as";

    //--------------------------- Inicialización de la tabla ------------------------------

    // Inicializamos la tabla de hash con el tamño del array de keywords
    if(initHashTable(tabla, 15) == 0){
        printf("Error al inicializar la tabla de hash\n");
        return 0;
    }

    // Insertamos las palabras reservadas en la tabla de hash
    for(int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++){
        if(insertToken(tabla, keywords[i], i + 300) == 0){
            printf("Error al insertar el token %s en la tabla de hash\n", keywords[i]);
        }
    }

    return 1;
}

/**
 * @brief Función que destruye la tabla de símbolos
 * @param tabla: tabla de símbolos que se destruirá
*/
void destruirTabla(hashTable *tabla){
    deleteHashTable(tabla);
}

/**
 * @brief Función que imprime la tabla de símbolos
 * @param tabla: tabla de símbolos que se imprimirá
*/
void imprimirTabla(hashTable tabla){
    printTable(tabla);
}

//------------------------------- Funciones de Interacción ------------------------------

/**
 * @brief Función que inserta un elemento en la tabla de símbolos
 * @param t: variable de tipo token que contiene el componente léxico a insertar
 * @param tabla: tabla de símbolos en la que se insertará el componente léxico
 * @return 1 si se ha insertado correctamente, 0 si no se ha podido insertar
*/
int insertarElemento(token t, hashTable *tabla){
    return insertToken(tabla, t.lexema, t.componente);
}

/**
 * @brief Función que modifica un elemento en la tabla de símbolos es importante entender que el 
 *        lexema no se puede modificar, por que si no no se podria encontrar el token (Hay que 
 *        pasarle el componente ya modificado)
 * @param t: variable de tipo token que contiene el componente léxico a modificar
 * @param tabla: tabla de símbolos en la que se modificará el componente léxico
*/
int modificarElemento(token t, hashTable tabla){
    return modifyToken(&tabla, t.lexema, t.componente);
}

/**
 * @brief Función que busca un elemento en la tabla de símbolos. 
 * @param t: variable de tipo token que contiene el componente léxico a buscar, y que será 
 *          comparado con los elementos de la tabla y modificara el componente léxico de t
 * @param tabla: tabla de símbolos en la que se buscará el componente léxico
 * @return token con el componente léxico buscado (si no se encuentra el lexema, el componente léxico será 0)
*/
int buscarElemento(char *lexema, hashTable tabla){
    return searchTokenComponent(tabla, lexema);
}

/**
 * @brief Función que borra un elemento en la tabla de símbolos
 * @param lexema: lexema que se eliminará
 * @param tabla: tabla de símbolos en la que se eliminará el componente léxico
 * @return 1 si se ha eliminado correctamente, 0 si no se ha podido eliminar
*/
int borrarElemento(char *lexema, hashTable tabla){
    return deleteToken(tabla, lexema);
}