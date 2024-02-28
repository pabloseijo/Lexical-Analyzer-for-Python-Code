/**
 * @author Pablo Seijo García
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones de la tabla de símbolos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "definiciones.h"
#include "tablaSimbolos.h"
#include "../tablasHash/tablaHash.h"

//--------------------------- Declaración de Funciones Privadas ------------------------------



//--------------------------- Funciones Públicas ------------------------------

/**
 * @brief Función que inicializa la tabla de símbolos, introduciendo las palabras reservadas del lenguaje
*/
void inicializarTabla(){

    //Primero tenemos que inicializar la tabla con las keywotds del lenguaje,
    //que podemos obtener de python.org en nuestro caso

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

    
    //TODO: Implementar

}

/**
 * @brief Función que inserta un elemento en la tabla de símbolos
 * @param t: variable de tipo token que contiene el componente léxico a insertar
 * @param tabla: tabla de símbolos en la que se insertará el componente léxico
 * @return 1 si se ha insertado correctamente, 0 si no se ha podido insertar
*/
int insertarElemento(token t, token * tabla[]){
    return insertToken(tabla, t.lexema, t.componente); //? Esta función es realmente necesaria
}

/**
 * @brief Función que modifica un elemento en la tabla de símbolos
 * @param t: variable de tipo token que contiene el componente léxico a modificar
*/
void modificarElemento(token t){
    //TODO: Implementar
}

/**
 * @brief Función que busca un elemento en la tabla de símbolos. Si lo encuentra devuelve su posición, si no, devuelve -1
 * @param t: variable de tipo token que contiene el componente léxico a buscar
*/
int buscarElemento(token t){
    return -1; //TODO: Implementar
}

/**
 * @brief Función que elimina un elemento en la tabla de símbolos
 * @param t: variable de tipo token que contiene el componente léxico a eliminar
*/
void crearTabla(){
    //TODO: Implementar
}

/**
 * @brief Función que destruye la tabla de símbolos
*/
void destruirTabla(){
    //TODO: Implementar
}

/**
 * @brief Función que imprime la tabla de símbolos
*/
void imprimirTabla(){
    //TODO: Implementar
}

//--------------------------- Funciones Privadas ------------------------------

