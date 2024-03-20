/**
 * @file sistemaEntrada.c
 * @date 20/03/2024
 * @brief Implementación de las funciones de gestion de errores
 * 
 * Gestiona los errores que se producen en el compilador
 */

#include <stdio.h>
#include <stdlib.h>
#include "gestionErrores.h"

//No se encuentra el fichero y se sale del programa
void ficheroNoEncontrado(char *nombreFichero){
    printf("ERROR: No se ha encontrado el fichero %s\n", nombreFichero);
    exit(EXIT_FAILURE);
}

//Se ha excedido el tamaño máximo del lexema
void tamLexemaExcedido(){
    printf("ERROR: El tamaño del lexema excede el tamaño máximo permitido\n");
    exit(EXIT_FAILURE);
}

//No se reconoce el caracater
void caracterNoReconocido(char c){
    printf("ERROR: Caracter no reconocido: %c\n", c);
    exit(EXIT_FAILURE);
}

