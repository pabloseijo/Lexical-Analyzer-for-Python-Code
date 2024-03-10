#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analizadorLexico/analizadorLexico.h"
#include "./definiciones.h"
#include "./tablaSimbolos/tablaSimbolos.h"
#include "./sistemaEntrada/sistemaEntrada.h"

// Incluye aquí todas las librerías necesarias de esta carpeta


int main(){
    token t;

    FILE *ficheroEntrada = fopen("entrada.txt", "r");

    inicializarDobleCentinela(ficheroEntrada);

    hashTable tabla;

    inicializarTabla(&tabla);

    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);


    printf("\n");
}