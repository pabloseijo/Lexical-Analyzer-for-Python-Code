#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analizadorLexico/analizadorLexico.h"
#include "./definiciones.h"
#include "./tablaSimbolos/tablaSimbolos.h"
#include "./sistemaEntrada/sistemaEntrada.h"
#include "./analizadorSintactico/analizadorSintactico.h"

// Incluye aquí todas las librerías necesarias de esta carpeta


int main(){

    FILE *ficheroEntrada = fopen("wilcoxon.py", "r");

    inicializarDobleCentinela(ficheroEntrada);

    hashTable tabla;

    inicializarTabla(&tabla);

    //imprimirTabla(tabla);
    
    iniciarAnalisis(tabla, ficheroEntrada);

    //imprimirTabla(tabla);

    destruirTabla(tabla);
    fclose(ficheroEntrada);

    printf("\n");
}