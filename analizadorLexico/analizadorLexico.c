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
#include "../sistemaEntrada/sistemaEntrada.h"

FILE* fichero; // Fichero de entrada

//--------------------------- Declaración de Funciones Privadas ------------------------------

/** AUTÓMATAS **/

/**
 * @brief Función que implementa el autómata para el componente léxico ID
 * @param sig: puntero al siguiente caracter a leer
 * @param t: puntero a una estructura token
*/
void automataID(char *sig, token *t);

/**
 * @brief Función que implementa el autómata para el componente léxico NÚMEROS
 * @param sig: puntero al siguiente caracter a leer
*/
void automataNums(char *sig, token *t);

/**
 * @brief Función que implementa el autómata para el componente léxico OPERADORES
 * @param sig: puntero al siguiente caracter a leer
*/
void automataOp(char *sig);

/**
 * @brief Función que implementa el autómata para el componente léxico DELIMITADORES
 * @param sig: puntero al siguiente caracter a leer
*/
void automataDel(char *sig);

/**
 * @brief Función que implementa el autómata para el componente léxico STRING
 * @param sig: puntero al siguiente caracter a leer
*/
void automataString(char *sig);

//--------------------------- Funciones Públicas ------------------------------

/**
 * @brief Función que devuelve el siguiente token (es decir el siguiente componente léxico)
 * @param t: puntero a una estructura token
 * @param tabla: puntero a una tabla de símbolos
*/
int seguinte_comp_lexico(token *t, hashTable *tabla){

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
    while(sig == ' ' || sig == '\n' || sig == '\t' || sig == '#'){
        // En el caso de que sea un comentario leeremos hasta el final de la línea
        if(sig == '#'){
            while(sig != '\n'){
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
        automataID(&sig, t);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece a la cadena alfanumérica
        devolverCaracter(sig);

        // Bussacmos el token en la tabla de símbolos, y le metemos el componente léxico
        t->componente = buscarElemento(t->lexema, *tabla);

        if(t->componente == 0){
            t->componente = ID;
            insertarElemento(*t, tabla);
        }

        return 1;
    }

    //-------------------- 2: NÚMEROS --------------------

    // Si el caracter es un número, podria ser el inicio de un número
    if(isdigit(sig)){
        //Llamamos al automata de números
        automataNums(&sig, t);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al número
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
    }

    //-------------------- 3: OPERADORES --------------------

    // Si el caracter es un operador, podria ser el inicio de un operador
    if(sig == '+' || sig == '-' || sig == '*' || sig == '/' || sig == '%' ||
        sig == '=' || sig == '!' || sig == '<' || sig == '>'){

        //Llamamos al automata de operadores
        automataOp(&sig);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al operador
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos

    }

    //-------------------- 4: DELIMITADORES --------------------

    // Si el caracter es un delimitador, podria ser el inicio de un delimitador
    if(sig == '(' || sig == ')' || sig == '{' || sig == '}' || sig == '[' || sig == ']' || sig == ',' || sig == ';'){

        //Llamamos al automata de delimitadores
        automataDel(&sig);

        // Al acabar el autómata deberemos devolver el último caracter leído, ya que no pertenece al delimitador
        devolverCaracter(sig);

        //TODO: meter sig en el token para buscarlo en la tabla de símbolos
        
    }

    //-------------------- 5: STRINGS -------------------- 

    // Si el caracter es una comilla doble, podria ser el inicio de un string   
    if(sig == '\"'){
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
 * @param t: puntero a una estructura token
*/
void automataID(char *sig, token *t){
    
    int estado = 0; // Estado inicial

    char cadena[30]; // Cadena que almacenará el lexema leido
    int index = 0; // Índice de la cadena
    

    char *lexema = NULL; // Lexema que almacenará el lexema leido

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO ID
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de caracteres
     *  -> Estado 2: Aceptación
    */
    while (estado != -1){
        switch(estado){

            // Estado 0: Inicial
            case 0:

                // Si leemos un caracter afanumerico o un guión bajo, pasamos al estado 1
                if(isalpha(*sig) || *sig == '_'){
                    estado = 1; // Pasamos al estado 1
                
                    cadena[index] = *sig; // Almacenamos el caracter en la cadena
                    index++; // Incrementamos el índice

                    *sig = siguienteCaracter(fichero); // Leemos el siguiente caracter
                }    
                
                // En caso contrario salimos del autómata
                else return; 

                break;

            // Estado 1: Lectura de caracteres
            case 1:

                // Mientras sigamos leyendo caracteres alfanuméricos o guiones bajos, seguimos en el estado 1
                // Recordemos que isalnum devuelve 1 si el caracter es alfanumérico, mientras que is alpha 
                // devuelve 1 si el caracter es una letra
                if(isalnum(*sig) || *sig == '_'){
                    estado = 1;

                    cadena[index] = *sig; // Almacenamos el caracter en la cadena
                    index++; // Incrementamos el índice

                    *sig = siguienteCaracter(fichero); // Leemos el siguiente caracter
                } 

                // En caso contrario, salimos del autómata
                else estado = 2;

                break;

            // Estado 2: Aceptación
            case 2:
                // Cuando llegamos al estado de aceptacion, hacemos una reserva de memoria con la memoria justa para 
                // almacenar el lexema
                lexema = (char*) malloc (sizeof(char) * (index + 1));

                // Copiamos la cadena en el lexema
                for(int i = 0 ; i < index; i++){
                    lexema[i] = cadena[i];
                }

                //Reservamos memoria para el lexema y copiamos la cadena en el lexema
                t->lexema = (char*) malloc (sizeof(char) * index);

                strncpy(t->lexema, lexema, index);

                estado = -1; // Salimos del autómata

                break;

            default:
                // Estado de error
                return;
                break;
        }
    }
}

/**
 * @brief Función que implementa el autómata para el componente léxico NÚMEROS
 * @param sig: puntero al siguiente caracter a leer
 * @param t: puntero a una estructura token
*/
void automataNums(char *sig, token *t){

    int estado = 0; // Estado inicial

    char cadena[30]; // Cadena que almacenará el lexema leido
    int index = 0; // Índice de la cadena

    char *lexema = NULL; // Lexema que almacenará el lexema leido

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO NÚMEROS
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de dígitos
     *  -> Estado 2: Aceptación
    */
    while (estado != 2){
        switch(estado){

            // Estado 0: Inicial
            case 0:

                // Si leemos un numero pasamos al estado 1
                if(isdigit(*sig)){
                    estado = 1; // Pasamos al estado 1
                    cadena[index] = *sig; // Almacenamos el caracter en la cadena
                    index++; // Incrementamos el índice
                } 
                
                // En caso contrario salimos del autómata
                else return; 

                break;

            // Estado 1: Lectura de numeros
            case 1:

                // Mientras sigamos leyendo numeros, seguimos en el estado 1
                if(isdigit(*sig)){
                    estado = 1; // Pasamos al estado 1
                    cadena[index] = *sig; // Almacenamos el caracter en la cadena
                    index++; // Incrementamos el índice
                }

                // En caso contrario, salimos del autómata
                else estado = 2;

                break;

            // Estado 2: Aceptación
            case 2:
                // Cuando llegamos al estadoii de aceptacion, hacemos una reserva de memoria con la memoria justa para 
                // almacenar el lexema
                lexema = (char*) malloc (sizeof(char) * index);

                // Copiamos la cadena en el lexema
                for(int i = 0 ; i < index; i++){
                    lexema[i] = cadena[i];
                }

                strncpy(t->lexema, lexema, index);

                break;

            default:
                // Estado de error
                return;
                break;
        }
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
    while(estado != 2){
        switch(estado){

            // Estado 0: Inicial
            case 0:

                // Si leemos un caracter afanumerico o un guión bajo, pasamos al estado 1
                if(*sig == '+' || *sig == '-' || *sig == '*' || *sig == '/' || *sig == '%' ||
                    *sig == '=' || *sig == '!' || *sig == '<' || *sig == '>'){

                    estado = 1;
                }
                
                // En caso contrario salimos del autómata
                else return; 

                break;

            // Estado 1: Lectura de caracteres
            case 1:

                // Si leemos un caracter afanumerico o un guión bajo, pasamos al estado 1
                if(*sig == '+' || *sig == '-' || *sig == '*' || *sig == '/' || *sig == '%' ||
                    *sig == '=' || *sig == '!' || *sig == '<' || *sig == '>'){

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
}

/**
 * @brief Función que implementa el autómata para el componente léxico DELIMITADORES
 * @param sig: puntero al siguiente caracter a leer
*/
void automataDel(char *sig){

    int estado = 0; // Estado inicial

    /**
     * AUTÓMATA PARA EL COMPONENTE LÉXICO NÚMEROS
     *  -> Estado 0: Inicial
     *  -> Estado 1: Lectura de delimitadores
     *  -> Estado 2: Aceptación
    */
    while(estado != 2){
        switch(estado){
            // Estado 0: Inicial
            case 0:
                //Si leemos un delimitador pasamos al estado 1
                if(*sig == '(' || *sig == ')' || *sig == '{' || *sig == '}' || *sig == '[' || *sig == ']' 
                || *sig == ',' || *sig == ';'){
                    estado = 1;
                }

                //En caso contrario salimos del autómata
                else return;
            
            // Estado 1: Lectura de delimitadores
            case 1:
                //Si leemos un delimitador pasamos al estado 1
                if(*sig == '(' || *sig == ')' || *sig == '{' || *sig == '}' || *sig == '[' || *sig == ']' 
                || *sig == ',' || *sig == ';'){
                    estado = 1;
                }

                //En caso contrario, salimos del autómata
                else estado = 2;

            // Estado 2: Aceptación
            case 2:
                //Cuando llegamos al estado de aceptación salimos del autómata
                break;
        }
    }
}

/**
 * @brief Función que implementa el autómata para el componente léxico STRING
 * @param sig: puntero al siguiente caracter a leer
*/
void automataString(char *sig){

    int estado = 0; // Estado inicial

    while(estado != 2){
        switch (estado){
            // Estado 0: Inicial
            case 0:
                //Si leemos una comilla doble pasamos al estado 1
                if(*sig == '\"'){
                    estado = 1;
                }

                //En caso contrario salimos del autómata
                else return;

            // Estado 1: Lectura de caracteres
            case 1:
                //Mientras sigamos leyendo caracteres, seguimos en el estado 1
                if(*sig != '\"'){
                    estado = 1;
                }

                //En caso contrario, pasamos al estado 2
                else estado = 2;

            // Estado 2: Aceptación
            case 2:
                //Cuando llegamos al estado de aceptación salimos del autómata
                break;
        }
    }
}


int main(){
    token t;
    fichero = fopen("wilcoxon.py", "r");

    hashTable tabla;

    inicializarTabla(&tabla);

    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);
    seguinte_comp_lexico(&t, &tabla);
    printf("%s %d\n", t.lexema, t.componente);

    imprimirTabla(tabla);

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

    imprimirTabla(tabla);
}