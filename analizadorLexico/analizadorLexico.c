/**
 * @author Pablo Seijo García
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones del analizador léxico
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analizadorLexico.h"
#include "../definiciones.h"
#include "../tablaSimbolos/tablaSimbolos.h"
#include "../tablasHash/tablaHash.h"

FILE* fichero; // Fichero de entrada

//--------------------------- Declaración de Funciones Privadas ------------------------------

/** AUTÓMATAS **/

/**
 * @brief Función que implementa el autómata para el componente léxico ID
 * @param sig: puntero al siguiente caracter a leer
*/
void automataID(char *sig);

/**
 * @brief Función que implementa el autómata para el componente léxico NÚMEROS
 * @param sig: puntero al siguiente caracter a leer
*/
void automataNums(char *sig);


/**
 * @brief Función que implementa el autómata para el componente léxico OPERADORES
 * @param sig: puntero al siguiente caracter a leer
*/
void automataOp(char *sig);

//--------------------------- Funciones Públicas ------------------------------

/**
 * @brief Función que devuelve el siguiente token (es decir el siguiente componente léxico)
 * @param t: puntero a una estructura token
*/
int seguinte_comp_lexico(token *t){

    // Comprobamos si el fichero de entrada está abierto
    if(fichero == NULL){
        printf("Error: No se ha abierto el fichero de entrada\n");
        exit(EXIT_FAILURE);
    }

    //-------------------- COMENTARIOS Y ESPACIOS --------------------

    // Para este analizador léxico, saltamos los comentarios y los espacio en blanco
    // En python los comentarios empiezan con #

    char sig = siguienteCaracter(fichero);

    // Saltamos espacios, saltos de linea, tabualciones y comentarios
    while(sig == " " || sig == "\n" || sig == "\t" || sig == "#"){
        // En el caso de que sea un comentario leeremos hasta el final de la línea
        if(sig == "#"){
            while(sig != "\n"){
                sig = siguienteCaracter(fichero);
            }
        }
        // Leemos el siguiente caracter
        sig = siguienteCaracter(fichero);
    }

    //################# ANALISIS DE LEXEMAS #################

    //-------------------- 1: CADENAS ALFANUMÉRICAS --------------------

    // Si el caracter es una letra, podria ser el inicio de una cadema alfanumérica

    if(isalpha(sig)){
        //Llamamos al autómata para el componente léxico ID
        automataID(&sig);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece a la cadena alfanumérica
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
    }

    //-------------------- 2: NÚMEROS --------------------

    // Si el caracter es un número, podria ser el inicio de un número
    if(isdigit(sig)){
        //Llamamos al automata de números
        automataNums(&sig);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al número
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
    }

    //-------------------- 3: OPERADORES --------------------

    // Si el caracter es un operador, podria ser el inicio de un operador
    if(sig == "+" || sig == "-" || sig == "*" || sig == "/" || sig == "%" ||
        sig == "=" || sig == "!" || sig == "<" || sig == ">"){

        //Llamamos al automata de operadores
        automataOp(&sig);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al operador
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos

    }

    //-------------------- 4: DELIMITADORES --------------------

    // Si el caracter es un delimitador, podria ser el inicio de un delimitador
    if(sig == "(" || sig == ")" || sig == "{" || sig == "}" || sig == "[" || sig == "]" || sig == "," || sig == ";"){

        //Llamamos al automata de delimitadores
        automataDel(&sig);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al delimitador
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
        
    }

    //-------------------- 5: STRINGS -------------------- 

    // Si el caracter es una comilla doble, podria ser el inicio de un string   
    if(sig == "\""){
        //Llamamos al automata de strings
        automataString(&sig);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al string
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
    }

    
}


//--------------------------- Funciones Privadas ------------------------------

//--------------------------- AUTÓMATAS ------------------------------

/**
 * @brief Función que implementa el autómata para el componente léxico ID
 * @param sig: puntero al siguiente caracter a leer
*/
void automataID(char *sig){
    
    int estado = 0; // Estado inicial

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO ID
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de caracteres
     *  -> Estado 2: Aceptación
    */
    switch(estado){

        // Estado 0: Inicial
        case 0:

            // Si leemos un caracter afanumerico o un guión bajo, pasamos al estado 1
            if(isalpha(*sig) || *sig == "_") estado = 1;
            
            // En caso contrario salimos del autómata
            else return; 

            break;

        // Estado 1: Lectura de caracteres
        case 1:

            // Mientras sigamos leyendo caracteres alfanuméricos o guiones bajos, seguimos en el estado 1
            // Recordemos que isalnum devuelve 1 si el caracter es alfanumérico, mientras que is alpha 
            // devuelve 1 si el caracter es una letra
            if(isalnum(*sig)) estado = 1; 

            // En caso contrario, salimos del autómata
            else estado = 2;

            break;

        // Estado 2: Aceptación
        case 2:
            //Cuando llegamos al estado de aceptación, devolvemos el lexema leido
            break;

        default:
            // Estado de error
            return;
            break;
    }
}

/**
 * @brief Función que implementa el autómata para el componente léxico NÚMEROS
 * @param sig: puntero al siguiente caracter a leer
*/
void automataNums(char *sig){

    int estado = 0; // Estado inicial

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO NÚMEROS
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de dígitos
     *  -> Estado 2: Aceptación
    */
   switch(estado){

        // Estado 0: Inicial
        case 0:

            // Si leemos un numero pasamos al estado 1
            if(isdigit(*sig)) estado = 1;
            
            // En caso contrario salimos del autómata
            else return; 

            break;

        // Estado 1: Lectura de numeros
        case 1:

            // Mientras sigamos leyendo numeros, seguimos en el estado 1
            if(isdigit(*sig)) estado = 1; 

            // En caso contrario, salimos del autómata
            else estado = 2;

            break;

        // Estado 2: Aceptación
        case 2:
            //Cuando llegamos al estado de aceptacion salimos del automata
            break;

        default:
            // Estado de error
            return;
            break;
   }
}


/**
 * @brief Función que implementa el autómata para el componente léxico OPERADORES
 * @param sig: puntero al siguiente caracter a leer
*/
void automataOp(char *sig){

    int estado = 0; // Estado inicial

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO NÚMEROS
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de operadores
     *  -> Estado 2: Aceptación
    */
   switch(estado){

        // Estado 0: Inicial
        case 0:

            // Si leemos un caracter afanumerico o un guión bajo, pasamos al estado 1
            if(sig == "+" || sig == "-" || sig == "*" || sig == "/" || sig == "%" ||
                sig == "=" || sig == "!" || sig == "<" || sig == ">"){

                estado = 1;
            }
            
            // En caso contrario salimos del autómata
            else return; 

            break;

        // Estado 1: Lectura de caracteres
        case 1:

            // Mientras sigamos leyendo operadores, seguimos en el estado 1
            if(sig == "+" || sig == "-" || sig == "*" || sig == "/" || sig == "%" ||
                sig == "=" || sig == "!" || sig == "<" || sig == ">"){

                estado = 1;
            }

            // En caso contrario, salimos del autómata
            else estado = 2;

            break;

        // Estado 2: Aceptación
        case 2:
            //Cuando llegamos al estado de acceptacion salimos del automata
            break;

        default:
            // Estado de error
            return;
            break;
   }
}