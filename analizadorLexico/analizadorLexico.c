/**
 * @file analizadorLexico.c
 * @author Pablo Seijo García
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
#include "../tablaSimbolos/tablaSimbolos.h"
#include "../sistemaEntrada/sistemaEntrada.h"

FILE* ficheroEntrada; 



//Autómata para el componente léxico ID
void automataID(char *siguienteChar, token *tokenProcesado);

//Autómata para el componente léxico NÚMEROS
void automataNums(char *siguienteChar, token *tokenProcesado);

//Autómata para el componente léxico NÚMEROS ENTEROS
int automataInts(char *siguienteChar, token *tokenProcesado);

//Autómata para el componente léxico NÚMEROS FLOTANTES e IMAGINARIOS
int automataFloats(char *siguienteChar, token *tokenProcesado);

//Autómata para el componente léxico OPERADORES
int automataOp(char *siguienteChar, token *tokenProcesado);

//Autómata para el componente léxico DELIMITADORES
int automataDel(char *siguienteChar, token *tokenProcesado);

//Autómata para el componente léxico STRING
void automataString(char *siguienteChar);

// Función que devuelve el siguiente token (es decir el siguiente componente léxico)
int seguinte_comp_lexico(token *tokenProcesado, hashTable *tabla){

    if(ficheroEntrada == NULL){
        printf("Error: No se ha abierto el fichero de entrada\n");
        exit(EXIT_FAILURE);
    }

    //-------------------- COMENTARIOS Y ESPACIOS --------------------

    // Para este analizador léxico, saltamos los comentarios y los espacio en blanco
    // que en python los comentarios empiezan con #

    char siguienteChar = devolverDelantero();
    moverInicioLexemaADelantero();

    int contadorCharSaltados = 0;

    while(siguienteChar == ' ' || siguienteChar == '\n' || siguienteChar == '\t' || siguienteChar == '#'){
        if(siguienteChar == '#'){
            while(siguienteChar != '\n'){
                siguienteChar = siguienteCaracter(ficheroEntrada);
            }
            
        }
        siguienteChar = siguienteCaracter(ficheroEntrada);
        contadorCharSaltados++;
    }

    if(contadorCharSaltados>0){
        moverInicioLexemaADelantero();
    }

    //-------------------- 1: CADENAS ALFANUMÉRICAS --------------------

    // Si el caracter es una letra, podria ser el inicio de una cadema alfanumérica

    if(isalpha(siguienteChar)){
        automataID(&siguienteChar, tokenProcesado);

        tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

        if(tokenProcesado->componente == 0){
            tokenProcesado->componente = ID;
            insertarElemento(*tokenProcesado, tabla);
        }

        siguienteChar = siguienteCaracter(ficheroEntrada);

        return 1;
    }

    //-------------------- 2: NÚMEROS --------------------

    // Si el caracter es un número, podria ser el inicio de un número
    if(isdigit(siguienteChar) || siguienteChar == '.'){

        if (siguienteChar != '.'){
            if(automataInts(&siguienteChar, tokenProcesado)){

                tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

                siguienteChar = siguienteCaracter(ficheroEntrada);

                return 1;
            }
        }

        if(automataFloats(&siguienteChar, tokenProcesado)){

            tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

            siguienteChar = siguienteCaracter(ficheroEntrada);

            return 1;
        }
    }

    //-------------------- 3: OPERADORES --------------------

    // Si el caracter es un operador, podria ser el inicio de un operador
    if(siguienteChar == '+' || siguienteChar == '-' || siguienteChar == '*' || siguienteChar == '/' ||
        siguienteChar == '%' || siguienteChar == '=' || siguienteChar == '!' || siguienteChar == '<' ||
        siguienteChar == '>' || siguienteChar == '&' || siguienteChar == '|' || siguienteChar == '^' ||
        siguienteChar == '~' || siguienteChar == '@' || siguienteChar == ':'){

        if(automataOp(&siguienteChar, tokenProcesado)){

            tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

            if(tokenProcesado->componente == 0){
                tokenProcesado->componente = OP;
                insertarElemento(*tokenProcesado, tabla);
            }

            siguienteChar = siguienteCaracter(ficheroEntrada);

            return 1;
        }
    }

    //-------------------- 4: DELIMITADORES --------------------

    // Si el caracter es un delimitador, podria ser el inicio de un delimitador
    if(siguienteChar == '(' || siguienteChar == ')' || siguienteChar == '{' || siguienteChar == '}' || siguienteChar == '[' 
        || siguienteChar == ']' || siguienteChar == ',' || siguienteChar == ';' || siguienteChar == '.' || siguienteChar == ':'
        || siguienteChar == '=' || siguienteChar == '+' || siguienteChar == '-' || siguienteChar == '*' || siguienteChar == '/'
        || siguienteChar == '%' || siguienteChar == '@' || siguienteChar == '&' || siguienteChar == '|' || siguienteChar == '^'
        || siguienteChar == '>' || siguienteChar == '<'){

        if(automataDel(&siguienteChar, tokenProcesado)){

            tokenProcesado->componente = buscarElemento(tokenProcesado->lexema, *tabla);

            if(tokenProcesado->componente == 0){
                tokenProcesado->componente = DEL;
                insertarElemento(*tokenProcesado, tabla);
            }

            siguienteChar = siguienteCaracter(ficheroEntrada);

            return 1;
        }


    }

    //-------------------- 5: STRINGS -------------------- 

    // Si el caracter es una comilla doble, podria ser el inicio de un string   
    if(siguienteChar == '\"'){
        automataString(&siguienteChar);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al string
        retrocederCaracter();

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
    }

    
}

//--------------------------- AUTÓMATAS ------------------------------

void automataID(char *siguienteChar, token *tokenProcesado){
    
    int estado = 0; 
    int index = 0; 
    
    char *lexema = NULL;

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO ID
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de caracteres
     *  -> Estado 2: Aceptación
    */
    while (estado != -1){
        switch(estado){

            case 0:

                if(isalpha(*siguienteChar) || *siguienteChar == '_'){
                    estado = 1; 

                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }    
                
                else return; 

                break;

            case 1:

                // isalnum devuelve 1 si el caracter es alfanumérico, mientras que isalpha 
                // devuelve 1 si el caracter es una letra
                if(isalnum(*siguienteChar) || *siguienteChar == '_'){
                    estado = 1;

                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 

                else estado = 2;

                break;

            case 2:

                retrocederCaracter();
    
                tokenProcesado->lexema = devolverLexema();
                estado = -1;

                break;

            default:
                return;
                break;
        }
    }
}

void automataNums(char *siguienteChar, token *tokenProcesado){

    int estado = 0; 

    char cadenaCharLeidos[30];
    int index = 0; 

    char *lexema = NULL;

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO NÚMEROS
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de dígitos
     *  -> Estado 2: Aceptación
    */
    while (estado != 2){
        switch(estado){

            case 0:
                if(isdigit(*siguienteChar)){
                    estado = 1; 
                    cadenaCharLeidos[index] = *siguienteChar; 
                    index++;
                } 
                
                else return; 

                break;

            case 1:

                if(isdigit(*siguienteChar)){
                    estado = 1; 
                    cadenaCharLeidos[index] = *siguienteChar;
                    index++;
                }

                else estado = 2;

                break;

            
            case 2:
        
                lexema = (char*) malloc (sizeof(char) * index);

                for(int i = 0 ; i < index; i++){
                    lexema[i] = cadenaCharLeidos[i];
                }

                strncpy(tokenProcesado->lexema, lexema, index);

                break;

            default:
                return;
                break;
        }
    }
}

int automataInts(char *siguienteChar, token *tokenProcesado){

    int estado = 0;

    int contadorParaRetroceder = 1;

     /**
     * Ver el autómata en la carpeta automatasPNG
     *  -> Estado 0: Inicial
     *  -> Estado 1, 2, 6, 7, 8  : Aceptación
    */ 
    while (estado != -1){
        switch(estado){

            case 0:
                //ASCII del 1..9 
                if(*siguienteChar >= 49 && *siguienteChar <= 57){
                    estado = 1;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 
                
                else if(*siguienteChar == '0'){
                    estado = 2;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 
                
                else {
                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                    }

                    return 0;
                }

                break;

            case 1:

                //ASCII del 0..9
                while(*siguienteChar >= 48 && *siguienteChar <= 57 || *siguienteChar == '_'){
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

               if(!isdigit(*siguienteChar) && *siguienteChar != '_' && *siguienteChar != '.' && *siguienteChar != 'e' && *siguienteChar != 'E'){

                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();
                    estado = -1;
                    return 1;
                } else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *siguienteChar = devolverDelantero();
                    }
                    
                    return 0;
                }

                break;

            
            case 2:
        
                if(*siguienteChar == 'b' || *siguienteChar == 'B'){
                    estado = 3;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else if (*siguienteChar == 'o' || *siguienteChar == 'O'){
                    estado = 4;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else if (*siguienteChar == 'x' || *siguienteChar == 'X'){
                    estado = 5;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                else{
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();
                    estado = -1;
                    return 1;
                }

                break;

            case 3:

                if(*siguienteChar == '0' || *siguienteChar == '1'){
                    estado = 6;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                } 

                else if(*siguienteChar == '_'){
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                    if(*siguienteChar == '0' || *siguienteChar == '1'){
                        estado = 6;
                    }
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *siguienteChar = devolverDelantero();
                    }
                    return 0;
                }

                break;

            case 4:
                //ASCII del 0..8
                if(*siguienteChar >= 48 && *siguienteChar <= 56 ){
                    estado = 7;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                } 

                else if(*siguienteChar == '_'){
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                    if(*siguienteChar >= 48 && *siguienteChar <= 56){
                        estado = 7;
                    }
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *siguienteChar = devolverDelantero();
                    }
                    return 0;
                }

                break;

            case 5:
                //ASCII del 0..9, a..f y A..F
                if(*siguienteChar >= 48 && *siguienteChar <= 57 ||          
                    *siguienteChar >= 97 && *siguienteChar <= 102 || 
                    *siguienteChar >= 65 && *siguienteChar <= 70){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                } 

                else if(*siguienteChar == '_'){
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                    if(*siguienteChar >= 48 && *siguienteChar <= 57 || 
                        *siguienteChar >= 97 && *siguienteChar <= 102 || 
                        *siguienteChar >= 65 && *siguienteChar <= 70){
                        estado = 8;
                    } else return 0;
                }

                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *siguienteChar = devolverDelantero();
                    }
                    return 0;
                }

                break;

            case 6:

                while(*siguienteChar == '0' || *siguienteChar == '1' || *siguienteChar == '_'){
                        *siguienteChar = siguienteCaracter(ficheroEntrada);
                        contadorParaRetroceder++;
                }

                if(*siguienteChar != '1' && *siguienteChar != 0 && *siguienteChar != '_' && *siguienteChar != '.' && *siguienteChar != 'e' && *siguienteChar != 'E'){
                    retrocederCaracter();

                    tokenProcesado->lexema = devolverLexema();
                    estado = -1;
                    return 1;
                } 
                
                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *siguienteChar = devolverDelantero();
                    }

                    return 0;
                }

                break;

            case 7:
                //ASCII del 0..8
                while(*siguienteChar >= 48 && *siguienteChar <= 56 || *siguienteChar == '_'){
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                }

                if(!(*siguienteChar >= 48 && *siguienteChar <= 56) && *siguienteChar != '_' && *siguienteChar != '.' && *siguienteChar != 'e' && *siguienteChar != 'E'){
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();
                    estado = -1;
                    return 1;
                } 
                
                else {

                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *siguienteChar = devolverDelantero();
                    }
                    return 0;
                }

                break;

            case 8:
                while(*siguienteChar >= 48 && *siguienteChar <= 57 ||          
                    *siguienteChar >= 97 && *siguienteChar <= 102 || 
                    *siguienteChar >= 65 && *siguienteChar <= 70 || *siguienteChar == '_'){

                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    contadorParaRetroceder++;
                
                }

                if(!(*siguienteChar >= 48 && *siguienteChar <= 57 || 
                    *siguienteChar >= 97 && *siguienteChar <= 102 || 
                    *siguienteChar >= 65 && *siguienteChar <= 70) && *siguienteChar != '_' && *siguienteChar != '.' && *siguienteChar != 'e' && *siguienteChar != 'E'){

                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();

                    tokenProcesado->lexema = devolverLexema();
                    estado = -1;
                    return 1;
                } 
                
                else {
                    for(int i = 0; i < contadorParaRetroceder; i++){
                        retrocederCaracter();
                        *siguienteChar = devolverDelantero();
                    }
                    return 0;
                }

                break;

            default:
                return 0;
                break;
        }
    }
}

int automataFloats(char *siguienteChar, token *tokenProcesado){

    int estado = 0;
    

     /**
     * Ver el autómata en la carpeta automatasPNG (Los estados 2 y 3 se colapsan en uno)
     *  -> Estado 0: Inicial
     *  -> Estado 1, 2, 3  : Aceptación
    */ 
    while (estado != -1){
        switch(estado){

            case 0:
                //ASCII del 1..9 
                if(*siguienteChar >= 48 && *siguienteChar <= 57){
                    estado = 0;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 
                
                else if(*siguienteChar == '.'){
                    estado = 1;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 

                else if(*siguienteChar == 'e' || *siguienteChar == 'E'){
                    estado = 1;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 
                
                else return 0;

                break;

            case 1:
                //ASCII del 0..9
                while(*siguienteChar >= 48 && *siguienteChar <= 57 || *siguienteChar == '_'){
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                if(*siguienteChar == 'j'){
                    estado = 4;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else{
                    retrocederCaracter();

                    tokenProcesado->lexema = devolverLexema();
                    estado = -1;

                    return 1;
                }

                break;

            case 2:
                //ASCII del 0..9
                while(*siguienteChar >= 48 && *siguienteChar <= 57 || *siguienteChar == '+' || *siguienteChar == '-' || *siguienteChar == '_'){
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                if(*siguienteChar == 'j'){
                    estado = 4;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else{
                    retrocederCaracter();

                    tokenProcesado->lexema = devolverLexema();
                    estado = -1;
                    return 1;
                }

                break;

            case 3:
                //ASCII del 0..9
                
                tokenProcesado->lexema = devolverLexema();
                estado = -1;
                return 1;
                
                break;
        }
    }
}

int automataOp(char *siguienteChar, token *tokenProcesado){

    int estado = 0;

    /**
     * Ver el autómata en la carpeta automatasPNG
     *  -> Estado 0: Inicial
     *  -> Estado 10: Aceptación
    */
    while(estado != -1){
        switch(estado){

            case 0:

                if(*siguienteChar == '+'|| *siguienteChar == '%' || *siguienteChar == '&' || *siguienteChar == '|' || *siguienteChar == '@' || *siguienteChar == '^'){ 
                    estado = 1;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '*'){ 
                    estado = 2;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '~'){ 
                    estado = 10;
                }

                else if (*siguienteChar == '/'){ 
                    estado = 4;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == ':' || *siguienteChar == '=' || *siguienteChar == '!'){
                    estado = 5;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 

                else if (*siguienteChar == '>'){
                    estado = 6; 
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '<'){ 
                    estado = 7;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if(*siguienteChar == '-' ){
                    estado = 9;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }
                
                else return 0; 

                break;

            case 1:

                if(*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                } else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
    
                    return 0;
                } 

                break;

            case 2:
                if(*siguienteChar == '*'){
                    estado = 3;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 

                else if (*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                } 
                
                else { 
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;
            
            case 3:

                if(*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                } else {
                    //Retrocedemos dos veces porque el automata leyo '**'
                    retrocederCaracter();
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;

            case 4:

                if(*siguienteChar == '/'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 
                
                else if (*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                } 
                
                else { 
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;

            case 5:

                if(*siguienteChar == '='){
                    estado = 10;
                } else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;

            case 6:
                
                if(*siguienteChar == '>'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }
                
                else if(*siguienteChar == '='){
                    estado = 10;
                }
                
                else {
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                }
    
                break;

            case 7:
                
                if(*siguienteChar == '<'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 
                
                else if(*siguienteChar == '='){
                    estado = 10;
                }

                else {
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                }
    
                break;

            case 8:
                
                if(*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                } else { // Retrocedemos dos veces porque el automata leyo '<<' o '>>' o '//'
                    retrocederCaracter();
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 
                break;

            case 9:
                    
                if(*siguienteChar != '=' && *siguienteChar != '>'){
                    estado = 10;
                } else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;

            case 10:
                tokenProcesado->lexema = devolverLexema();
                estado = -1;
                return 1;
                break;

            default:
                // Estado de error
                return 0;
                break;
        }
    }
}

int automataDel(char *siguienteChar, token *tokenProcesado){

    int estado = 0; 

    //Ver el automata en la carpeta automatasPNG
    /**
     * estado 0: Inicial
     * estado 10: Aceptación
    */
    while(estado != -1){
        switch(estado){
            
            //Incial
            case 0:
                if(*siguienteChar == '(' || *siguienteChar == ')' || *siguienteChar == '{' || *siguienteChar == '}' || *siguienteChar == '[' 
                    || *siguienteChar == ']' || *siguienteChar == ',' || *siguienteChar == ';' || *siguienteChar == '.'){ 
                    estado = 10;  
                }

                else if( *siguienteChar == '^' || *siguienteChar == '&' || *siguienteChar == '|' 
                    || *siguienteChar == '@' || *siguienteChar == '%' || *siguienteChar == '+'){
                    estado = 1;  
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == ':' || *siguienteChar == '='){ 
                    estado = 2;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '-'){
                    estado = 3;  
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '/'){
                    estado = 4;  
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '<'){
                    estado = 5;  
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '>'){
                    estado = 6; 
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '*'){
                    estado = 7;  
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } 

                else return 0;
                break;
            
            case 1:

                if(*siguienteChar == '='){
                    estado = 10;

                }  else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;  

            case 2:

                if(*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                }  else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break; 
            
            case 3:

                if(*siguienteChar == '=' || *siguienteChar == '>'){
                    estado = 10;
                }  else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break; 

            case 4:

                if(*siguienteChar == '/'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }  
                
                else if(*siguienteChar == '='){
                    estado = 10;
                } 
                
                else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break; 

            case 5:

                if(*siguienteChar == '<'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada); 
                }  else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 
                
                break; 

            case 6:

                if(*siguienteChar == '>'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } else {
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;

            case 7:

                if(*siguienteChar == '*'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);  
                }  
                
                else if(*siguienteChar == '='){
                    estado = 10;
                }
                
                else { 
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break;

            case 8:

                if(*siguienteChar == '='){
                    estado = 10; 
                }  else { // Retrocedemos dos veces porque el automata leyo '//' o '<<' o '>>' o '**'
                    retrocederCaracter();
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                    return 0;
                } 

                break; 

            case 10:

                tokenProcesado->lexema = devolverLexema();

                estado = -1;
                return 1;
                break;  
        }
    }
}

void automataString(char *siguienteChar){

    int estado = 0; 

    while(estado != 2){
        switch (estado){
            
            case 0:
                
                if(*siguienteChar == '\"'){
                    estado = 1;
                }

                else return;

            case 1:

                if(*siguienteChar != '\"'){
                    estado = 1;
                }

                else estado = 2;

            case 2:

                break;
        }
    }
}


int main(){
    token t;
    ficheroEntrada = fopen("archivo.txt", "r");

    inicializarDobleCentinela(ficheroEntrada);

    hashTable tabla;

    inicializarTabla(&tabla);

    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);

    printf("\n");
}