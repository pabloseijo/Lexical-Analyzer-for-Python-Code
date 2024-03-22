#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analizadorLexico/analizadorLexico.h"
#include "./definiciones.h"
#include "./tablaSimbolos/tablaSimbolos.h"
#include "./sistemaEntrada/sistemaEntrada.h"
#include "./analizadorSintactico/analizadorSintactico.h"
#include "./gestionErrores/gestionErrores.h"

// Incluye aquí todas las librerías necesarias de esta carpeta


int main(int argc,char *argv[]){

    FILE *ficheroEntrada;
    char *nombreFichero = argv[1];

    if(argc != 2){
        numParametrosIncorrecto();
    }

    if( (ficheroEntrada = fopen(nombreFichero, "r")) == NULL){
        ficheroNoEncontrado(nombreFichero);
    }

    inicializarDobleCentinela(ficheroEntrada);

    hashTable tabla;

    inicializarTabla(&tabla);

    imprimirTabla(tabla);
    
    iniciarAnalisis(tabla, ficheroEntrada);

    imprimirTabla(tabla);

    destruirTabla(tabla);
    fclose(ficheroEntrada);

    printf("\n");
}