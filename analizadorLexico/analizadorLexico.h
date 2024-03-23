/**
 * @file analizadorLexico.h
 * @date 30/02/2024
 * @brief Definición de las funciones del sistema del analizador léxico
 * 
 * Este archivo contiene las definiciones de las funciones del sistema del analizador léxico
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../definiciones.h"
#include "../tablaSimbolos/tablaSimbolos.h"
#include "../tablasHash/tablaHash.h"
#include "../sistemaEntrada/sistemaEntrada.h"

/**
 * Función que devuelve el siguiente token (es decir el siguiente componente léxico)
 * @param t: puntero a una estructura token
 * @param tabla: tabla de símbolos en la que se buscará el siguiente componente léxico
 * @param fichero: fichero de entrada
 * @return Devuelve 1 si se ha procesado correctamente el token, 0 si ha habido un error
*/
int seguinte_comp_lexico(token *tokenProcesado, hashTable *tabla, FILE *fichero);