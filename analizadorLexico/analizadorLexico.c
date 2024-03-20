/**
 * @file analizadorLexico.c
 * @date 30/02/2024
 * @brief Implementacion de las funciones del sistema del analizador léxico
 * 
 * Procesa los caracteres devueltos por el sistema de entrada y los manda al
 * analizador sintáctico. Para ello implementa los autómatas que definen los
 * componentes léxicos del lenguaje.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analizadorLexico.h"
#include "../definiciones.h"
#include "../sistemaEntrada/sistemaEntrada.h"

FILE* ficheroEntrada;

//Autómata para el componente léxico ID
void automataID(char *charActual, token *tokenProcesado);

//Autómata para el componente léxico NÚMEROS ENTEROS
int automataInts(char *charActual, token *tokenProcesado);

//Autómata para el componente léxico NÚMEROS FLOTANTES e IMAGINARIOS
int automataFloats(char *charActual, token *tokenProcesado);

//Autómata para el componente léxico OPERADORES
int automataOp(char *charActual, token *tokenProcesado);

//Autómata para el componente léxico DELIMITADORES
int automataDel(char *charActual, token *tokenProcesado);

//Autómata para el componente léxico STRING
int automataString(char *charActual, token *tokenProcesado);

//Salta los comentarios del tipo """ o '''
int automataComentariosComillas(char *charActual);

// Función que devuelve el siguiente token (es decir el siguiente componente léxico)
int seguinte_comp_lexico(token *tokenProcesado, hashTable *tabla, FILE *fichero){

    if(fichero == NULL){
        printf("Error: No se ha abierto el fichero de entrada\n");
        exit(EXIT_FAILURE);
    }

    ficheroEntrada = fichero;

    //-------------------- COMENTARIOS Y ESPACIOS --------------------

    // Para este analizador léxico, saltamos los comentarios y los espacio en blanco
    // que en python los comentarios empiezan con #

    char charActual = devolverDelantero();
    moverInicioLexemaADelantero();

    int contadorCharSaltados = 0;

    while(charActual == ' ' || charActual == '\n' || charActual == '\t' || charActual == '#'
          || charActual == '\"' || charActual == '\'' || charActual == '\0'){

        if(charActual == '#'){
            while(charActual != '\n'){
                charActual = siguienteCaracter(ficheroEntrada);
            }
            moverInicioLexemaADelantero();
            continue;
        }
        //Saltamos los comentarios del tipo """ o '''
        else if(charActual == '\"' || charActual == '\''){
            if(automataComentariosComillas(&charActual)){
                moverInicioLexemaADelantero();
                continue;
            }
            else break;
        }
        charActual = siguienteCaracter(ficheroEntrada);
        contadorCharSaltados++;
    }

    //Avanzamos el inicio hasta acabar el comentario
    if(contadorCharSaltados>0){
        moverInicioLexemaADelantero();
    }

    //-------------------- 1: CADENAS ALFANUMÉRICAS --------------------

    // Si el caracter es una letra, podria ser el inicio de una cadema alfanumérica

    if(isalpha(charActual) || charActual == '_'){
        automataID(&charActual, tokenProcesado);

        tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

        if(tokenProcesado->componente == 0){
            tokenProcesado->componente = ID;
            insertarElemento(*tokenProcesado, tabla);
        }

        charActual = siguienteCaracter(ficheroEntrada);

        return 1;
    }

    //-------------------- 2: NÚMEROS --------------------

    // Si el caracter es un número, podria ser el inicio de un número
    if(isdigit(charActual) || charActual == '.'){

        if (charActual != '.'){
            if(automataInts(&charActual, tokenProcesado)){
                
                tokenProcesado->componente = NUM_INT;
            
                charActual = siguienteCaracter(ficheroEntrada);

                return 1;
            }
        }

        if(automataFloats(&charActual, tokenProcesado)){

            tokenProcesado->componente = NUM_FLOAT;
            
            charActual = siguienteCaracter(ficheroEntrada);

            return 1;
        }
    }

    //-------------------- 3: OPERADORES --------------------

    // Si el caracter es un operador, podria ser el inicio de un operador
    if(charActual == '+' || charActual == '-' || charActual == '*' || charActual == '/' ||
       charActual == '%' || charActual == '=' || charActual == '!' || charActual == '<' ||
       charActual == '>' || charActual == '&' || charActual == '|' || charActual == '^' ||
       charActual == '~' || charActual == '@' || charActual == ':'){

        if(automataOp(&charActual, tokenProcesado)){

            tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

            if(tokenProcesado->componente == 0){
                tokenProcesado->componente = OP;
            }

            charActual = siguienteCaracter(ficheroEntrada);

            return 1;
        }
    }

    //-------------------- 4: DELIMITADORES --------------------

    // Si el caracter es un delimitador, podria ser el inicio de un delimitador
    if(charActual == '(' || charActual == ')' || charActual == '{' || charActual == '}' || charActual == '['
       || charActual == ']' || charActual == ',' || charActual == ';' || charActual == '.' || charActual == ':'
       || charActual == '=' || charActual == '+' || charActual == '-' || charActual == '*' || charActual == '/'
       || charActual == '%' || charActual == '@' || charActual == '&' || charActual == '|' || charActual == '^'
       || charActual == '>' || charActual == '<'){

        if(automataDel(&charActual, tokenProcesado)){

            tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

            if(tokenProcesado->componente == 0){
                tokenProcesado->componente = DEL;
            }

            charActual = siguienteCaracter(ficheroEntrada);

            return 1;
        }


    }

    //-------------------- 5: STRINGS -------------------- 

    // Si el caracter es una comilla doble, podria ser el inicio de un string   
    if(charActual == '\"' || charActual == '\''){
        if(automataString(&charActual, tokenProcesado)){

            tokenProcesado->componente = STRING;
            
            charActual = siguienteCaracter(ficheroEntrada);

            return 1;
        }
    }

    if(charActual == '\000' || charActual == EOF){
        tokenProcesado->componente = EOF;

        printf("Fin de fichero\n");
        return 0;
    }

    return 0;
}

//--------------------------- AUTÓMATAS ------------------------------

void automataID(char *charActual, token *tokenProcesado){

    int estado = 0;

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO ID
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de caracteres
     *  -> Estado 2: Aceptación
    */
    while (estado != -1){
        switch(estado){

            case 0:

                if(isalpha(*charActual) || *charActual == '_'){
                    estado = 1;

                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else return;

                break;

            case 1:

                // isalnum devuelve 1 si el caracter es alfanumérico, mientras que isalpha 
                // devuelve 1 si el caracter es una letra
                if(isalnum(*charActual) || *charActual == '_'){
                    estado = 1;

                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else estado = 2;

                break;

            case 2:

                retrocederCaracter();

                tokenProcesado->lexema = devolverLexema();
                estado = -1;

                break;
        }
    }
}

int automataInts(char *charActual, token *tokenProcesado){

    int estado = 0;

    int contadorParaRetroceder = 1;

    /**
    * Ver el autómata en la carpeta automatasPNG
    *  -> Estado 0: Inicial
    *  -> Estado 1, 2, 6, 7, 8  : Aceptación
   */
    while (1){
        switch(estado){

            case 0:
                //ASCII del 1..9 
                if(isdigit(*charActual) && *charActual != '0'){
                    estado = 1;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '0'){
                    estado = 2;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else {
                    for(int i = 0; i < contadorParaRetroceder; i++) {
                        retrocederCaracter();
                    }

                    return 0;
                }

                break;

            case 1:

                //ASCII del 0..9
                while(isdigit(*charActual) || *charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                if(!isdigit(*charActual) && *charActual != '_' && *charActual != '.' && *charActual != 'e' && *charActual != 'E'){

                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();

                    return 1;
                } else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *charActual = devolverDelantero();
                    }

                    return 0;
                }

            case 2:

                if(*charActual == 'b' || *charActual == 'B'){
                    estado = 3;
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else if (*charActual == 'o' || *charActual == 'O'){
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else if (*charActual == 'x' || *charActual == 'X'){
                    estado = 5;
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else{
                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();

                    return 1;
                }

                break;

            case 3:

                if(*charActual == '0' || *charActual == '1'){
                    estado = 6;
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else if(*charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                    if(*charActual == '0' || *charActual == '1'){
                        estado = 6;
                    }
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *charActual = devolverDelantero();
                    }
                    return 0;
                }

                break;

            case 4:
                //ASCII del 0..8
                if(isdigit(*charActual) && *charActual == '9'){
                    estado = 7;
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else if(*charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                    if(*charActual >= 48 && *charActual <= 56){
                        estado = 7;
                    }
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *charActual = devolverDelantero();
                    }
                    return 0;
                }

                break;

            case 5:
                //ASCII del 0..9, a..f y A..F
                if(isdigit(*charActual)||
                   (*charActual >= 97 && *charActual <= 102) ||
                   (*charActual >= 65 && *charActual <= 70) ){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else if(*charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                    if(isdigit(*charActual) ||
                       (*charActual >= 97 && *charActual <= 102) ||
                       (*charActual >= 65 && *charActual <= 70) ){
                        estado = 8;
                    } else return 0;
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *charActual = devolverDelantero();
                    }
                    return 0;
                }

                break;

            case 6:

                while(*charActual == '0' || *charActual == '1' || *charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                if(*charActual != '1' && *charActual != 0 && *charActual != '_' && *charActual != '.' && *charActual != 'e' && *charActual != 'E'){
                    retrocederCaracter();

                    tokenProcesado->lexema = devolverLexema();

                    return 1;
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *charActual = devolverDelantero();
                    }

                    return 0;
                }


            case 7:
                //ASCII del 0..8
                while((isdigit(*charActual) && *charActual!='8') || *charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                if(!(*charActual >= 48 && *charActual <= 56) && *charActual != '_' && *charActual != '.' && *charActual != 'e' && *charActual != 'E'){
                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();

                    return 1;
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *charActual = devolverDelantero();
                    }
                    return 0;
                }

            case 8:
                while(isdigit(*charActual) ||
                      (*charActual >= 97 && *charActual <= 102) ||
                      (*charActual >= 65 && *charActual <= 70) || *charActual == '_'){

                    *charActual = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;

                }

                if(!(isdigit(*charActual) ||
                     (*charActual >= 97 && *charActual <= 102) ||
                     (*charActual >= 65 && *charActual <= 70) ) && *charActual != '_' && *charActual != '.' && *charActual != 'e' && *charActual != 'E'){

                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();
                    return 1;
                }

                else {
                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *charActual = devolverDelantero();
                    }
                    return 0;
                }

        }
    }
}

int automataFloats(char *charActual, token *tokenProcesado){

    int estado = 0;


    /**
    * Ver el autómata en la carpeta automatasPNG (Los estados 2 y 3 se colapsan en uno)
    *  -> Estado 0: Inicial
    *  -> Estado 1, 2, 3  : Aceptación
   */
    while (1){
        switch(estado){

            case 0:
                //ASCII del 0..9
                if(isdigit(*charActual)){
                    estado = 0;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '.'){
                    estado = 1;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == 'e' || *charActual == 'E'){
                    estado = 2;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else return 0;

                break;

            case 1:
                //ASCII del 0..9
                while(isdigit(*charActual) || *charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                if(*charActual == 'j'){
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } 

                else{

                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();
                    return 1;
                }

                break;

            case 2:
                //ASCII del 0..9
                while(isdigit(*charActual) || *charActual == '+' || *charActual == '-' || *charActual == '_'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                if(*charActual == 'j'){
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else{
                    retrocederCaracter();

                    tokenProcesado->lexema = devolverLexema();
                    return 1;
                }

                break;

        }
    }
}

int automataOp(char *charActual, token *tokenProcesado){

    int estado = 0;

    /**
     * Ver el autómata en la carpeta automatasPNG
     *  -> Estado 0: Inicial
     *  -> Estado 10: Aceptación
    */
    while(1){
        switch(estado){

            case 0:

                if(*charActual == '+'|| *charActual == '%' || *charActual == '&' || *charActual == '|' || *charActual == '@' || *charActual == '^'){
                    estado = 1;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '*'){
                    estado = 2;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '~'){
                    estado = 10;
                }

                else if (*charActual == '/'){
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == ':' || *charActual == '=' || *charActual == '!'){
                    estado = 5;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '>'){
                    estado = 6;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '<'){
                    estado = 7;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '-' ){
                    estado = 9;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else return 0;

                break;

            case 1:

                if(*charActual != '='){
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                } else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    return 0;
                }

                break;

            case 2:
                if(*charActual == '*'){
                    estado = 3;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual != '='){
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                }

                else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 3:

                if(*charActual != '='){
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                } else {
                    //Retrocedemos dos veces porque el automata leyo '**'
                    retrocederCaracter();
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 4:

                if(*charActual == '/'){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual != '='){
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                }

                else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 5:

                if(*charActual == '='){
                    estado = 10;
                } else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 6:

                if(*charActual == '>'){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '='){
                    estado = 10;
                }

                else {
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                }

                break;

            case 7:

                if(*charActual == '<'){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '='){
                    estado = 10;
                }

                else {
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                }

                break;

            case 8:

                if(*charActual != '='){
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                } else { // Retrocedemos dos veces porque el automata leyo '<<' o '>>' o '//'
                    retrocederCaracter();
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }
                break;

            case 9:

                if(*charActual != '=' && *charActual != '>'){
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                } else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 10:

                tokenProcesado->lexema = devolverLexema();
                return 1;

        }
    }
}

int automataDel(char *charActual, token *tokenProcesado){

    int estado = 0;

    //Ver el automata en la carpeta automatasPNG
    /**
     * estado 0: Inicial
     * estado 10: Aceptación
    */
    while(1){
        switch(estado){

            //Incial
            case 0:
                if(*charActual == '(' || *charActual == ')' || *charActual == '{' || *charActual == '}' || *charActual == '['
                   || *charActual == ']' || *charActual == ',' || *charActual == ';' || *charActual == '.'){
                    estado = 10;
                }

                else if( *charActual == '^' || *charActual == '&' || *charActual == '|'
                         || *charActual == '@' || *charActual == '%' || *charActual == '+'){
                    estado = 1;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == ':' || *charActual == '='){
                    estado = 2;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '-'){
                    estado = 3;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '/'){
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '<'){
                    estado = 5;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '>'){
                    estado = 6;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if (*charActual == '*'){
                    estado = 7;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else return 0;
                break;

            case 1:

                if(*charActual == '='){
                    estado = 10;

                }  else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 2:

                if(*charActual != '='){
                    estado = 10;
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                }  else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 3:

                if(*charActual == '=' || *charActual == '>'){
                    estado = 10;
                }  else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 4:

                if(*charActual == '/'){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '='){
                    estado = 10;
                }

                else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 5:

                if(*charActual == '<'){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }  else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 6:

                if(*charActual == '>'){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 7:

                if(*charActual == '*'){
                    estado = 8;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '='){
                    estado = 10;
                }

                else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 8:

                if(*charActual == '='){
                    estado = 10;
                }  else { // Retrocedemos dos veces porque el automata leyo '//' o '<<' o '>>' o '**'
                    retrocederCaracter();
                    retrocederCaracter();
                    *charActual = devolverDelantero();
                    return 0;
                }

                break;

            case 10:

                tokenProcesado->lexema = devolverLexema();

                return 1;
        }
    }
}

int automataString(char *charActual, token *tokenProcesado){

    int estado = 0;

    /**
     * Ver automata en automatasPNG
     * Estado 0: Inicial
     * Estados 4,6: Finales
    */
    while(1){

        switch (estado){

            case 0:

                if(*charActual == '\"'){
                    estado = 1;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else if(*charActual == '\''){
                    estado = 2;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else{
                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    return 0;
                }

                break;

            case 1:

                if(*charActual != '\"'){
                    estado = 3;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                else{
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                break;

            case 2:

                if(*charActual != '\''){
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } else{
                    estado = 5;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                break;


            case 3:

                while(*charActual != '\"'){
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                if(*charActual == '\"'){
                    estado = 5;
                }

                else return 0;

                break;

            case 4:

                while(*charActual != '\''){
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                if(*charActual == '\''){
                    estado = 5;
                }

                else{
                    return 0;
                }

                break;

            case 5:

                tokenProcesado->lexema = devolverLexema();
                return 1;


        }
    }
}

int automataComentariosComillas(char *charActual){

    int estado = 0;
    char tipoComillas = ' '; //Guarda el tipo de comillas que se está leyendo para simplificar el automata

    /**
     * Ver automata en automatasPNG (ambos caminos del automata fueron colapsados en uno solo
     * para simplificar el código, ya que ambos caminos son iguales)
     *
     * Estado 0: Inicial
     * Estados 6: Final
    */
    while(1){

        switch (estado){

            case 0:

                if(*charActual == '\"'){
                    tipoComillas = '\"';
                    estado = 1;
                    *charActual = siguienteCaracter(ficheroEntrada);

                } else if(*charActual == '\''){
                    tipoComillas = '\'';
                    estado = 1;
                    *charActual = siguienteCaracter(ficheroEntrada);

                } else return 0;

                break;

            case 1:
                if(*charActual == tipoComillas){
                    estado = 2;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    return 0;
                }

                break;

            case 2:
                if(*charActual == tipoComillas){
                    estado = 3;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } else {
                    retrocederCaracter();
                    *charActual = devolverDelantero();

                    return 0;
                }

                break;

            case 3:

                while(*charActual != tipoComillas){
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                if(*charActual == tipoComillas){
                    estado = 4;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } else {
                    return 0;
                }

                break;

            case 4:

                if(*charActual == tipoComillas){
                    estado = 5;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } else {
                    estado = 3;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                break;

            case 5:

                if(*charActual == tipoComillas){
                    estado = 6;
                    *charActual = siguienteCaracter(ficheroEntrada);
                } else {
                    estado = 3;
                    *charActual = siguienteCaracter(ficheroEntrada);
                }

                break;

            case 6:
                return 1;
        }
    }
}