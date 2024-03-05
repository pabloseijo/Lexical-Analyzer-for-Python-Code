/**
 * @file tablaSimbolos.c
 * @author Pablo Seijo García
 * @date 26/02/2024
 * @brief Implementación de una tabla de simbolos para la abstraccion de la 
 * tabla hash
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../definiciones.h"
#include "tablaSimbolos.h"
#include "../tablasHash/tablaHash.h"

// Inicializacion de la tabla de símbolos con las palabras reservadas del lenguaje
int inicializarTabla(hashTable *tabla){

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

    // Metemos 50 espacios para minimar el numero de redimensiones
    if(initHashTable(tabla, 50) == 0){
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

// Destruye la tabla de símbolos llamando a la funcion de la tabla de hash
void destruirTabla(hashTable *tabla){
    deleteHashTable(tabla);
}

// Imprime la tabla de símbolos llamando a la funcion de la tabla de hash
void imprimirTabla(hashTable tabla){
    printTable(tabla);
}

// Inserta el elemento en la tabla de símbolos llamando a la funcion de la tabla de hash
int insertarElemento(token t, hashTable *tabla){
    return insertToken(tabla, t.lexema, t.componente);
}

// Modifica el elemento en la tabla de símbolos llamando a la funcion de la tabla de hash
int modificarElemento(token t, hashTable tabla){
    return modifyToken(&tabla, t.lexema, t.componente);
}

// Busca el elemento en la tabla de símbolos llamando a la funcion de la tabla de hash
int buscarElemento(char *lexema, hashTable tabla){
    return searchTokenComponent(tabla, lexema);
}

// Borra el elemento en la tabla de símbolos llamando a la funcion de la tabla de hash
int borrarElemento(char *lexema, hashTable tabla){
    return deleteToken(tabla, lexema);
}