/**
 * @file analizadorSintactico.c
 * @date 26/02/2024
 * @brief Implementación de las funciones del analizador sintáctico
*/

#include <stdio.h>
#include <stdlib.h>
#include "../analizadorLexico/analizadorLexico.h"
#include "analizadorSintactico.h"
#include "../sistemaEntrada/sistemaEntrada.h"


void imprimirComponenteLexico(token c);

/**
 * Función que inicia el análisis léxico, pidiendo componentes léxicos al analizador
 * léxico hasta que este devuelva el componente asociado a EOF.
 */
void iniciarAnalisis(hashTable tabla, FILE *ficheroEntrada) {
    token t;

    printf("%-33s %-10s\n", "Lexema", "ID");
    printf("--------------------------------- ----------\n");

    do { //Pedimos componentes lexicos al analizadorLexico hasta que reciba EOF.
        printf("--------------------------------- ----------\n");

        int liberarMemoria = seguinte_comp_lexico(&t, &tabla, ficheroEntrada);
        //Si ha habido algún error, o es el fin de fichero no se imprime el componente.
        if (t.componente != EOF) {
            imprimirComponenteLexico(t);
        }

        if(liberarMemoria) free(t.lexema);

    } while (t.componente != EOF);
}

/**
 * Función privada que imprime los pares <lexema, componente> que recibe el analizador sintáctico.
 */
void imprimirComponenteLexico(token t) {
    // Imprime los valores en el mismo ancho de columna que las cabeceras
    printf("%-33s %-10d\n", t.lexema, t.componente);
}