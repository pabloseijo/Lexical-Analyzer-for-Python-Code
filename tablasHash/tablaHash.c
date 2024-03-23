/**
 * @file tablaHash.c
 * @date 21/02/2024
 * @brief Implementación de una tabla de hash para manejar tokens en un 
 * compilador.
 * 
 * Utiliza números primos que optimizan la función hash, mejorando la 
 * distribución y minimizando las colisiones.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tablaHash.h"

// Tamaño inicial de la tabla de hash y número de elementos almacenados.
int TABLE_SIZE = 0;
int NUMBER_OF_ELEMENTS = 0;

// Función hash para calcular el índice de un lexema en la tabla.
unsigned int hash(char *string);


//--------------------------- Estructura ------------------------------


// Inicialización de la tabla de hash con tamaño dinámico.
int initHashTable(hashTable *tabla, int size){

    *tabla = (token **) malloc (sizeof(token*) * size);

    if (*tabla == NULL) {
        perror("Fallo a la hora de asignar memoria para la tabla de hash\n");
        return 0;
    }

    for (int i = 0; i < size; i++){
        (*tabla)[i] = NULL;
    }

    TABLE_SIZE = size;
 
    return 1;
}

// Libera los recursos asociados con la tabla de hash.
int deleteHashTable(hashTable tabla){

    if(tabla == NULL){
        printf("ERROR: La tabla no existe\n");
        return 0;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (tabla[i] != NULL) {
            // Para cada elemento de la tabla hay una lista enlazada de tokens
            token *actual = tabla[i];

            while (actual != NULL) {
                token *siguiente = actual->next;

                free(actual->lexema); // Libera el lexema del token
                free(actual); // Libera el token

                actual = siguiente;
            }
        }
    }

    free(tabla);

    return 1;
}

// Ajusta el tamaño de la tabla para mantener el rendimiento óptimo.
int resizeHashTable(hashTable *tabla, int newSize) {

    // Paso 1: Crear una nueva tabla de hash del nuevo tamaño que sera nuestra nueva tabla

    hashTable nuevaTabla = malloc (sizeof(token *) * newSize);

    if (nuevaTabla == NULL) {
        printf("ERROR: Fallo al asignar nueva tabla de hash\n");
        return 0;
    }

    for (int i = 0; i < newSize; i++) {
        nuevaTabla[i] = NULL;
    }

    // Paso 2: Rehashear e insertar cada token de la tabla antigua en la nueva tabla

    int oldSize = TABLE_SIZE; 
    TABLE_SIZE = newSize;

    for (int i = 0; i < oldSize; i++) {

        token *actual = (*tabla)[i];

        while (actual != NULL) {
            // Guarda el próximo token antes de modificar el actual
            token *siguiente = actual->next;

            // Calcula el nuevo índice para el token actual basado en el nuevo tamaño de tabla
            int nuevoIndice = hash(actual->lexema);

            // Inserta el token en la nueva tabla (al principio de la lista en ese índice)
            actual->next = nuevaTabla[nuevoIndice];
            nuevaTabla[nuevoIndice] = actual;

            // Avanza al siguiente token en la lista original
            actual = siguiente;
        }
    }

    // No se liberan los elementos, solo la tabla antigua
    free(*tabla);

    *tabla = nuevaTabla;
    TABLE_SIZE = newSize;

    return 1;
}

// Muestra el contenido actual de la tabla para depuración.
void printTable(hashTable tabla) {
    printf("\nSTART\n");


    for (int index = 0; index < TABLE_SIZE; index++) {
        printf("%d\t", index); 
        if (tabla[index] == NULL) {
            printf("---\n"); // Indica que no hay token en esta posición
        }
        
        else {
            // Recorre la lista enlazada en esta posición de la tabla
            token *actual = tabla[index];
            while (actual != NULL) {
                printf("( %s | %d ) -> ", actual->lexema, actual->componente); 
                actual = actual->next;
            }
            printf("\n");
        }
    }

    printf("END\n");
}

// Inserta un nuevo token, redimensionando la tabla si es necesario.
int insertToken(hashTable *tabla, char *lexema, int componente){

    if (*tabla == NULL) return 0; 

    int index = hash(lexema);

    token *nuevoToken = (token *) malloc (sizeof(token));

    if (nuevoToken == NULL) return -1;

    // Implementamos el encadenamiento en tablas hash (mas eficiente) debido a las restricciones
    // del sistema
    nuevoToken->lexema = strdup(lexema); 
    nuevoToken->componente = componente; 
    nuevoToken->next = NULL;

    // Si hay colisión, insertamos el nuevo token al principio de la lista enlazada
    if ((*tabla)[index] == NULL) {
        (*tabla)[index] = nuevoToken;
    } else {
        nuevoToken->next = (*tabla)[index];
        (*tabla)[index] = nuevoToken;
    }

    NUMBER_OF_ELEMENTS++;

    // Redimensionamos la tabla si el factor de carga supera 0.75
    if(NUMBER_OF_ELEMENTS/TABLE_SIZE > 0.75){
        resizeHashTable(tabla, TABLE_SIZE * 2);
    }

    return 1;
}

// Busca un token por su lexema y devuelve su componente léxico si existe.
int searchTokenComponent(hashTable tabla, char *lexema) {

    int index = hash(lexema);

    token *actual = tabla[index];
    
    // Recorremos la lista enlazada en la posición de la tabla
    while (actual != NULL) {
        if (strcmp(actual->lexema, lexema) == 0) {
            return actual->componente; 
        }

        actual = actual->next;
    }

    return 0;
}

// Elimina un token específico si se encuentra en la tabla.
int deleteToken(hashTable tabla, char *lexema) {

    int index = hash(lexema);

    token *actual = tabla[index];
    token *anterior = NULL; 

    while (actual != NULL) {
        if (strcmp(actual->lexema, lexema) == 0) {
            if (anterior == NULL) {
                tabla[index] = actual->next;
            } else {
                anterior->next = actual->next;
            }

            free(actual->lexema);
            free(actual); 

            return 1;
        }

        anterior = actual;
        actual = actual->next;
    }

    return 0; 
}

// Actualiza el componente léxico de un token existente.
int modifyToken(hashTable *tabla, char * lexema, int componente){

    int componenteAux = searchTokenComponent(*tabla, lexema);

    if(componenteAux == 0) return 0;

    else {
        deleteToken(*tabla, lexema);
        insertToken(tabla, lexema, componente);
        
        return 1;
    } 
}

//------------------------------- Funciones privadas ------------------------------

// Cacula el hash para un string dado.
unsigned int hash(char *string){

    unsigned int hash = 0; 
    int asciiEquivalent;

    //Los primos tienen una gran capacidad de dispersión
    const unsigned int prime = 67;

    // Utilizamos una operación XOR y una multiplicación por un número primo para mezclar los bits y mejorar la dispersión
    for(int i = 0; i < strlen(string); i++){
        asciiEquivalent = string[i]; //ASCII del caracter
        hash = (hash * prime) ^ asciiEquivalent;
    }

    return hash % TABLE_SIZE;
}
