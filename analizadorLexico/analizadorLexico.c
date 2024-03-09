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
    if(isdigit(siguienteChar)){
        automataNums(&siguienteChar, tokenProcesado);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al número
        retrocederCaracter();

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
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

int automataOp(char *siguienteChar, token *tokenProcesado){

    int estado = 0;

    /**
     * Ver el autómata en la carpeta automatasPNG
     *  -> Estado 0: Inicial
     *  -> Estado 10: Aceptación
    */
    while(estado != 2){
        switch(estado){

            case 0:

                if(*siguienteChar == '+' || *siguienteChar == '-' || *siguienteChar == '%' ||
                    *siguienteChar == '&' || *siguienteChar == '|' || *siguienteChar == '@' || *siguienteChar == '^'){ //TODO: funciona
                    estado = 1;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '*'){
                    estado = 2;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '~'){
                    estado = 12;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '/'){ //TODO: funciona
                    estado = 4;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == ':' || *siguienteChar == '=' || *siguienteChar == '!'){
                    estado = 5;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '>'){ //TODO: funciona
                    estado = 6; 
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '<'){ //TODO: funciona
                    estado = 7;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }
                
                else return 0; 

                break;

            case 1:

                if(*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
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
                } else { 
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
                    return 0;
                } 

                break;

            case 6:
                
                if(*siguienteChar == '>'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } else {
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                }
    
                break;

            case 7:
                
                if(*siguienteChar == '<'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } else {
                    estado = 10;
                    retrocederCaracter();
                    *siguienteChar = devolverDelantero();
                }
    
                break;

            case 8:
                
                if(*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                } else { // Retrocedemos dos veces porque el automata leyo '<<' o '>>' o '//'
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
                    //TODO: funciona
                    estado = 10;  
                }

                else if( *siguienteChar == '^' || *siguienteChar == '&' || *siguienteChar == '|' || *siguienteChar == '=' 
                    || *siguienteChar == '@' || *siguienteChar == '%' || *siguienteChar == '+'){
                    estado = 1;  //TODO: funciona
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == ':'){
                    estado = 2;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '-'){
                    estado = 3;  //TODO: funciona
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '/'){
                    estado = 4;  //TODO: funciona //= pero no /=
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                }

                else if (*siguienteChar == '<'){
                    estado = 5;  //TODO: funciona <<=
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
                    return 0;
                } 

                break;  

            case 2:

                if(*siguienteChar != '='){
                    estado = 10;
                    retrocederCaracter();
                    break;  
                }  else {
                    retrocederCaracter();
                    return 0;
                } 
            
            case 3:

                if(*siguienteChar == '=' || *siguienteChar == '>'){
                    estado = 10;
                    break;  
                }  else {
                    retrocederCaracter();
                    return 0;
                } 

            case 4:

                if(*siguienteChar == '/'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    break;  
                }  else {
                    retrocederCaracter();
                    return 0;
                } 

            case 5:

                if(*siguienteChar == '<'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    break;  
                }  else {
                    retrocederCaracter();
                    return 0;
                } 
                
            case 6:

                if(*siguienteChar == '>'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                } else {
                    retrocederCaracter();
                    return 0;
                } 

            case 7:

                if(*siguienteChar == '*'){
                    estado = 8;
                    *siguienteChar = siguienteCaracter(ficheroEntrada);
                    break;  
                }  else { 
                    retrocederCaracter();
                    return 0;
                } 

            case 8:

                if(*siguienteChar == '='){
                    estado = 10;
                    break;  
                }  else { // Retrocedemos dos veces porque el automata leyo '//' o '<<' o '>>' o '**'
                    retrocederCaracter();
                    retrocederCaracter();
                    return 0;
                } 

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

    printf("\n");
}