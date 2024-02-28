/**
 * @author Pablo Seijo García
 * @date 21/02/2024
 * @brief Fichero que contiene las funciones de la tabla de hash
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "tablaHash.h"


//------ Inicialización de las funciones privadas auxiliares ---------

/**
 * @brief Estructura de la tabla de hash
 * @param string: cadena de caracteres que representa el lexema
 * @return hash: valor hash del lexema
*/
unsigned int hash(char *string);


//--------------------------- Estructura ------------------------------


/**
 * @brief Función que inicializa la tabla de hash
 * @param tabla: tabla de hash que se inicializará
*/
void initHashTable(hashTable tabla){

    // Inicializamos la tabla de hash
    for (int i = 0; i < TABLE_SIZE; i++){
        tabla[i] = NULL;
    }
 
    //La tabla se inicializa vacía
}

//------------------------------- Funciones ------------------------------


/**
 * @brief Función que imprime la tabla hash
 * @param tabla: tabla de hash que se imprimirá
*/
void printTable(hashTable tabla) {
    printf("START\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("%d\t", i); // Imprime el índice de la tabla
        if (tabla[i] == NULL) {
            printf("---\n"); // Indica que no hay token en esta posición
        }
        // Si hay tokens en esta posición
        else {
            // Recorre la lista enlazada en esta posición de la tabla
            token *actual = tabla[i];
            while (actual != NULL) {
                printf("%s | %d -> ", actual->lexema, actual->componente); // Imprime el lexema y el componente del token actual
                actual = actual->next; // Avanza al siguiente token en la lista
            }
        }
    }

    printf("END\n");
}



/**
 * @brief Función que inserta un token en la tabla de hash
 * @param tabla: tabla de hash en la que se insertará el token
 * @param lexema: lexema que se insertará
 * @param componente: componente léxico que se insertará
 * @return true si se ha insertado correctamente, false si no
*/
int insertToken(hashTable tabla, char *lexema, int componente){

    // Comprobamos que la tabla no sea nula
    if (tabla == NULL) return 0; 

    // Calculamos el indice de la tabla 
    int index = hash(lexema);

    // Creamos un nuevo token
    token *nuevoToken = (token *) malloc (sizeof(token));

    if (nuevoToken == NULL) return -1; // Fallo al reservar memoria

    // Dado a que nuestro sistema tiene pocas capacidades, tanto de memoria como de procesamiento, lo mas ideal a implementar en este caso sería el 
    // método del encadenamiento en la tabla hash, por el cual cada elemento de la lista consistirá en una lista enlazada de elementos que 
    // tienen el mismo valor hash

    nuevoToken->lexema = strdup(lexema); // Duplicamos el lexema, y lo guardamos en el token
    nuevoToken->componente = componente; // Guardamos el componente léxico
    nuevoToken->next = NULL; // Inicializamos el puntero al siguiente token a NULL

    // Si la posición de la tabla está vacía, simplemente insertamos el token
    if (tabla[index] == NULL) {
        tabla[index] = nuevoToken;
    } 
    // Si ya hay un token en esa posición, lo añadimos al final de la lista enlazada
    else {
        // Para añadirlo en la lista enlazada, simplemente hacemos que el nuevo token.next apunte al token que ya estaba en la tabla y tabla
        //[index] apunte al nuevo token
        nuevoToken->next = tabla[index];
        tabla[index] = nuevoToken;
    }

    return 0;
}

/**
 * @brief Función que busca un token en la tabla de hash
 * @param tabla: tabla de hash en la que se buscará el token
 * @param lexema: lexema que se buscará
 * @return token si se ha encontrado, NULL si no
*/
token *searchToken(hashTable tabla, char *lexema) {
    // Calculamos el índice de la tabla
    int index = hash(lexema);

    // Recorremos la lista enlazada en la posición calculada
    token *actual = tabla[index];
    
    // Recorrermos la lista enlazada hasta llegar al ultimo elemento, en el cual el puntero next apuntará a NULL
    while (actual != NULL) {
        if (strcmp(actual->lexema, lexema) == 0) {
            printf("Token encontrado\n");
            return actual; // Token encontrado
        }
        actual = actual->next; // Pasamos al siguiente token en la lista
    }

    // Si llegamos al final de la lista sin encontrar el lexema
    printf("No se ha encontrado el token\n");
    return NULL;
}


/**
 * @brief Función que elimina un token en la tabla de hash
 * @param tabla: tabla de hash en la que se eliminará el token
 * @param lexema: lexema que se eliminará
*/
void deleteToken(hashTable tabla, char *lexema) {
    // Calculamos el índice de la tabla
    int index = hash(lexema);

    // Variables para recorrer la lista
    token *actual = tabla[index]; // Token actual
    token *anterior = NULL; // Token anterior

    while (actual != NULL) {
        //Comparamos los strings, si son iguales, eliminamos el token
        if (strcmp(actual->lexema, lexema) == 0) {

            printf("Token eliminado\n");

            if (anterior == NULL) {
                // El token a eliminar está al principio de la lista
                tabla[index] = actual->next;
            } 
            
            else {
                // El token a eliminar está en medio o al final de la lista
                anterior->next = actual->next;
            }

            // Liberar la memoria del token eliminado
            free(actual->lexema); // Liberamos la memoria del lexema
            free(actual); // Liberamos la memoria del token
            return;
        }
        // Avanzar en la lista
        anterior = actual;
        actual = actual->next;
    }

    // Si no se ha encontrado el token
    printf("No se ha encontrado el token\n");
}


/**
 * @brief Función que modifica el componente del token a modificar a la tabla hash
 * @param tabla: tabla de hash en la que se modificará el token
 * @param t: token que se modificará
*/
int modifyToken(hashTable tabla, char * lexema, int componente){

    //                       ! IMPORTANTE !
    // ! Esta función solo sirve para la modificación del componente léxico !

    // Buscamos el token en la tabla
    token * t = searchToken(tabla, lexema);

    // Si no se ha encontrado el token, devolvemos 0
    if(t == NULL) return 0;
    // Si se ha encontrado el token, modificamos el componente
    else {
        t->componente = componente;
        return 1;
    } 
}

//------------------------------- Funciones privadas auxiliares ------------------------------

/**
 * @brief Estructura de la tabla de hash
 * @param string: cadena de caracteres que representa el lexema
 * @return hash: valor hash del lexema
*/
unsigned int hash(char *string){

    // Primero calculamos la longitud del lexema
    int legth = strnlen(string, MAX_NAME);

    unsigned int hash = 0;

    // Recorreremos el lexema
    for(int i = 0; i < legth; i++){

        hash += string[i]; // Sumamos el valor ASCII de cada caracter

        // Ahora para conseguir que los valores se dispersen y evitar las colisiones, mulitplicaremos el valor de hash 
        // por el valor ASCII de cada caracter, y para asegurarnos de que el valor no se desborde, lo dividiremos por 
        // el tamaño de la tabla y nos quedaremos con el resto
        hash = (hash * string[i]) % TABLE_SIZE; 
    }

    return hash;
}

//------------------------------- Función principal ------------------------------
int main() {
    hashTable tabla;
    initHashTable(tabla);

    printf("Insertando tokens...\n");
    insertToken(tabla, "token1", 1);
    insertToken(tabla, "token2", 2);
    insertToken(tabla, "token3", 3);

    printf("\nImprimiendo la tabla de hash después de las inserciones:\n");
    printTable(tabla);

    printf("\nBuscando token 'token2' en la tabla:\n");
    token *t = searchToken(tabla, "token2");
    if (t != NULL) {
        printf("Encontrado: %s, Componente: %d\n", t->lexema, t->componente);
    } else {
        printf("Token no encontrado.\n");
    }

    printf("\nModificando el componente del token 'token2' a 20:\n");
    if (modifyToken(tabla, "token2", 20)) {
        printf("Token modificado correctamente.\n");
    } else {
        printf("Error al modificar el token.\n");
    }

    printf("\nImprimiendo la tabla de hash después de la modificación:\n");
    printTable(tabla);

    printf("\nEliminando el token 'token2':\n");
    deleteToken(tabla, "token2");
    printf("Token 'token2' eliminado.\n");

    printf("\nImprimiendo la tabla de hash después de la eliminación:\n");
    printTable(tabla);

    // Limpieza final: liberar toda la memoria asociada con la tabla de hash
    // Esto es algo que tendrías que implementar, ya que la tabla de hash puede tener listas enlazadas de tokens
    // Puedes crear una función 'freeHashTable' que recorra la tabla y libere todos los tokens y sus lexemas

    return 0;
}