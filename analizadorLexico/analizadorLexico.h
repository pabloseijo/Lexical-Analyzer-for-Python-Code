/**
 * @file analizadorLexico.h
 * @author Pablo Seijo García
 * @date 30/02/2024
 * @brief Definición de las funciones del sistema del analizador léxico
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
 * @brief Función que devuelve el siguiente token (es decir el siguiente componente léxico)
 * @param t: puntero a una estructura token
 * @param tabla: tabla de símbolos en la que se buscará el siguiente componente léxico
*/
int seguinte_comp_lexico(token *t, hashTable *tabla);