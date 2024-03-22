#ifndef ANALIZADORSINTACTICO_H
#define ANALIZADORSINTACTICO_H

#include <stdio.h>
#include <stdlib.h>
#include "../analizadorLexico/analizadorLexico.h"
#include "analizadorSintactico.h"
#include "../sistemaEntrada/sistemaEntrada.h"

/**
 * Función que inicia el análisis léxico, pidiendo componentes léxicos al analizador
 * léxico hasta que este devuelva el componente asociado a EOF.
 */
void iniciarAnalisis(hashTable tabla, FILE *ficheroEntrada);

#endif // ANALIZADORSINTACTICO_H