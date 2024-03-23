/**
 * @file analizadorSintactico.h
 * @date 26/02/2024
 * @brief Definición de las funciones del analizador sintáctico
 * 
 * Este archivo contiene las definiciones de las funciones del analizador sintáctico
*/

#ifndef ANALIZADORSINTACTICO_H
#define ANALIZADORSINTACTICO_H

#include <stdio.h>
#include <stdlib.h>
#include "../analizadorLexico/analizadorLexico.h"
#include "analizadorSintactico.h"
#include "../sistemaEntrada/sistemaEntrada.h"

/**
 * Función que inicia el análisis léxico, pidiendo componentes léxicos al analizador
 * @param tabla: tabla de símbolos en la que se buscará el siguiente componente léxico
 * @param ficheroEntrada: fichero de entrada
 */
void iniciarAnalisis(hashTable tabla, FILE *ficheroEntrada);

#endif // ANALIZADORSINTACTICO_H