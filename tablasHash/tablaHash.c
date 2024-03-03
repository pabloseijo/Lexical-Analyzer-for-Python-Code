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

//Nos serviremos de una variable global para el tamaño de la tabla hash
int TABLE_SIZE = 0; // Tamaño de la tabla de hash
int NUMBER_OF_ELEMENTS = 0; // Número de elementos en la tabla de hash

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
 * @param size: tamaño de la tabla de hash
*/
int initHashTable(hashTable *tabla, int size){

    // Reservamos memoria para la tabla
    *tabla = (token **) malloc (sizeof(token*) * size);

    // Verificamos si la reserva de memoria fue exitosa
    if (*tabla == NULL) {
        perror("Failed to allocate memory for hash table");
        return 0;
    }

    // Inicializamos la tabla de hash
    for (int i = 0; i < size; i++){
        (*tabla)[i] = NULL;
    }

    TABLE_SIZE = size;
 
    //La tabla se inicializa vacía
    return 1;
}

/**
 * @brief Función que libera la memoria asociada con la tabla de hash
 * @param tabla: tabla de hash que se liberará
 * @return 1 si se ha liberado correctamente, 0 si no
*/
int deleteHashTable(hashTable *tabla){

    if(tabla == NULL)

    // Recorremos la tabla
    for (int i = 0; i < TABLE_SIZE; i++) {
        // Si hay tokens en esta posición
        if (tabla[i] != NULL) {
            // Recorremos la lista enlazada en esta posición de la tabla
            token *actual = *tabla[i];
            while (actual != NULL) {
                token *temp = actual; // Guardamos el token actual
                actual = actual->next; // Avanzamos al siguiente token en la lista
                free(temp->lexema); // Liberamos la memoria del lexema
                free(temp); // Liberamos la memoria del token
            }
        }
    }

    // Liberamos la memoria de la tabla
    free(tabla);

    printf("Memoria liberada\n");

    return 1;
}

/**
 * @brief Función que redimensiona la tabla de hash
 * @param tabla: tabla de hash que se redimensionará
 * @param newSize: nuevo tamaño de la tabla
 * @return 1 si se ha redimensionado correctamente, 0 si no
*/
int resizeHashTable(hashTable *tabla, int newSize) {

    // Paso 1: Crear una nueva tabla de hash del nuevo tamaño que sera nuestra nueva tabla
    
    hashTable nuevaTabla = (token **) malloc (sizeof(token *) * newSize);

    if (nuevaTabla == NULL) {
        printf("ERROR: Fallo al asignar nueva tabla de hash\n");
        return 0;
    }


    // Paso 2: Inicializar la nueva tabla

    for (int i = 0; i < newSize; i++) {
        nuevaTabla[i] = NULL;
    }

    // Paso 3: Rehashear e insertar cada token de la tabla antigua en la nueva tabla

    int oldSize = TABLE_SIZE; // Guardar el tamaño de la tabla antigua para recorrerla en el for
    TABLE_SIZE = newSize; // Actualizar el tamaño de la tabla para la funcion hash

    for (int i = 0; i < oldSize; i++) {

        //Guardamos el token actual
        token *actual = (*tabla)[i];

        while (actual != NULL) {
            // Guarda el próximo token antes de modificar el actual
            token *siguiente = actual->next;

            // Calcula el nuevo índice para el token actual basado en el nuevo tamaño de tabla
            int nuevoIndice = hash(actual->lexema);

            // Inserta el token en la nueva tabla (al principio de la lista en ese índice)
            actual->next = nuevaTabla[nuevoIndice];
            nuevaTabla[nuevoIndice] = actual;

            // Avanza al siguiente token en la lista original
            actual = siguiente;
        }
    }

    // Paso 4: Liberar la memoria de la tabla antigua (solo el array de punteros, no los tokens)
    free(*tabla);

    // Paso 5: Actualizar el puntero de la tabla a la nueva tabla y el tamaño de la tabla
    *tabla = nuevaTabla;
    TABLE_SIZE = newSize;

    return 1;
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
                printf("( %s | %d ) -> ", actual->lexema, actual->componente); // Imprime el lexema y el componente del token actual
                actual = actual->next; // Avanza al siguiente token en la lista
            }
            printf("\n"); // Nueva línea para el siguiente índice de la tabla
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
int insertToken(hashTable *tabla, char *lexema, int componente){

    // Comprobamos que la tabla no sea nula
    if (*tabla == NULL) return 0; 

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
    if ((*tabla)[index] == NULL) {
        (*tabla)[index] = nuevoToken;
    } 
    // Si ya hay un token en esa posición, lo añadimos al final de la lista enlazada
    else {
        // Para añadirlo en la lista enlazada, simplemente hacemos que el nuevo token.next apunte al token que ya estaba en la tabla y tabla
        //[index] apunte al nuevo token
        nuevoToken->next = (*tabla)[index];
        (*tabla)[index] = nuevoToken;
    }

    NUMBER_OF_ELEMENTS++; // Aumentamos el número de elementos en la tabla

    // Una parte muy importante de las tablas hash es el factor de carga, el cual nos indicará cuando debemos redimensionar la tabla
    // Por tanto si al insertar un elemento el factor de carga (numero de elementos / tamaño de la tabla) es mayor que 0.75, redimensionamos la tabla,
    // multiplicando el tamaño de la tabla por 2
    if(NUMBER_OF_ELEMENTS/TABLE_SIZE > 0.75){
        resizeHashTable(tabla, TABLE_SIZE * 2);
    }

    return 1;
}

/**
 * @brief Función que busca un token en la tabla de hash
 * @param tabla: tabla de hash en la que se buscará el token
 * @param lexema: lexema que se buscará
 * @return componente léxico si se ha encontrado el token, 0 si no
*/
int searchToken(hashTable tabla, char *lexema) {
    // Calculamos el índice de la tabla
    int index = hash(lexema);

    // Recorremos la lista enlazada en la posición calculada
    token *actual = tabla[index];
    
    // Recorrermos la lista enlazada hasta llegar al ultimo elemento, en el cual el puntero next apuntará a NULL
    while (actual != NULL) {

        if (strcmp(actual->lexema, lexema) == 0) {

            return actual->componente; // Token encontrado
        }
        actual = actual->next; // Pasamos al siguiente token en la lista
    }

    // Si llegamos al final de la lista sin encontrar el lexema
    return 0;
}


/**
 * @brief Función que elimina un token en la tabla de hash
 * @param tabla: tabla de hash en la que se eliminará el token
 * @param lexema: lexema que se eliminará
 * @return 1 si se ha eliminado correctamente, 0 si no se ha encontrado el token
*/
int deleteToken(hashTable tabla, char *lexema) {

    // Calculamos el índice de la tabla
    int index = hash(lexema);

    // Variables para recorrer la lista
    token *actual = tabla[index]; // Token actual
    token *anterior = NULL; // Token anterior

    printf("hola\n");

    while (actual != NULL) {
        //Comparamos los strings, si son iguales, eliminamos el token
        if (strcmp(actual->lexema, lexema) == 0) {

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
            return 1;
        }
        // Avanzar en la lista
        anterior = actual;
        actual = actual->next;
    }

   

    return 0; // No se ha encontrado el token
}


/**
 * @brief Función que modifica el componente del token a modificar a la tabla hash
 * @param tabla: tabla de hash en la que se modificará el token
 * @param t: token que se modificará
*/
int modifyToken(hashTable *tabla, char * lexema, int componente){

    //                       ! IMPORTANTE !
    // ! Esta función solo sirve para la modificación del componente léxico !

    // Buscamos el token en la tabla
    int componenteAux = searchToken(*tabla, lexema);

    // Si no se ha encontrado el token, devolvemos 0
    if(componenteAux == 0) return 0;

    // Si se ha encontrado el token, modificamos el componente
    else {
        // Borramos el token de la tabla
        deleteToken(*tabla, lexema);

        // Insertamos el token modificado en la tabla
        insertToken(tabla, lexema, componente);
        
        return 1;
    } 
}

//------------------------------- Funciones privadas auxiliares ------------------------------

/**
 * @brief Estructura de la tabla de hash (Es donde se produce la magia del hash)
 * @param string: cadena de caracteres que representa el lexema
 * @return hash: valor hash del lexema
*/
unsigned int hash(char *string){

    unsigned int hash = 0; // Inicializamos el valor hash a 0
    int c; // Variable para recorrer la cadena de caracteres

    // Usamos una constante de multiplicación prima para la dispersión, puesto que los primos tienen una capacidad de dispersión muy buena
    const unsigned int prime = 31;

    // Utilizaremos un hash muy específico en este caso, que consiste en entremezclar operaciones 
    // de multiplicación y XOR para conseguir una operación bit a bit y asi conseguir una 
    // dispersión muy buena. La operación XOR (^ c) es una operación no lineal, por lo que los 
    // cambios mas pequeños pueden significar cambios muy grandes en el resultado final, lo que 
    // mejora la dispersión de los datos en la tabla
    for(int i = 0; i < strlen(string); i++){
        c = string[i]; // Obtenemos el valor ASCII del caracter
        hash = (hash * prime) ^ c; // Operaciones bit a bit para mezclar los bits
    }

    return hash % TABLE_SIZE; // Devolvemos el valor hash módulo el tamaño de la tabla
}
