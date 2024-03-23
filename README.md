# Compilador Python Simplificado ![Lenguaje](https://img.shields.io/badge/Lenguaje-C-green.svg) ![GCC](https://img.shields.io/badge/Compilado%20con-GCC-red.svg)

Esto es un proyecto de un compilador simple de python desarrollado para la materia de compiladores e intérpretes de la Universidade de Santiago de Compostela en el grado de Ingeniería Informática (2ª edición).


## Tabla de Contenidos
- [Compilador Python Simplificado  ](#compilador-python-simplificado--)
  - [Tabla de Contenidos](#tabla-de-contenidos)
  - [Estructura del Proyecto](#estructura-del-proyecto)
  - [Características](#características)
      - [Tamaño de la tabla hash](#tamaño-de-la-tabla-hash)
      - [Tamaño del buffer](#tamaño-del-buffer)
  - [Compilación](#compilación)
  - [Uso](#uso)
      - [Con Valgrind](#con-valgrind)
  - [Automátas](#automátas)
    - [Delimitadores](#delimitadores)
    - [Operadores](#operadores)
    - [Strings](#strings)
    - [Ints](#ints)
    - [Floats](#floats)

## Estructura del Proyecto

El proyecto se estructura en varios componentes clave, organizados en directorios específicos para facilitar su manejo y comprensión:

- **analizadorLexico/**: Contiene el código fuente del analizador léxico, responsable de descomponer el código fuente en tokens léxicos.
- **analizadorSintactico/**: Alberga el analizador sintáctico, que construye el árbol sintáctico a partir de los tokens proporcionados por el analizador léxico.
- **sistemaEntrada/**: Gestiona la entrada de datos al compilador, leyendo el código fuente desde un archivo.
- **tablasHash/**: Implementa una tabla de hash utilizada para el almacenamiento y recuperación eficiente de tokens durante el análisis.
- **tablaSimbolos/**: Administra la tabla de símbolos, esencial para el seguimiento de las entidades del lenguaje.
- **gestionErrores/**: Proporciona funcionalidades para el reporte de errores encontrados durante el análisis del código fuente.
- **definiciones.h**: Define constantes y estructuras globales usadas a lo largo del proyecto.

## Características

El compilador presenta dos principales definiciones que pueden ser cambiadas:

#### Tamaño de la tabla hash

Esto es bastante importante cara a las gestión espacial y temporal del proyecto. Se encueentra definida en `tablaSimbolos/tablaSimbolos.c` y está por defecto definido en 64. El número ha sido adecuadamente seleccionado para provocar las mínimas colisiones posibles y asegurar la dispersión de la tabla, pero si el usuario desea comprobar el funcionamiento de la función resize puede ser cambiado. 

La función de resize ha sido testeada con los números: 1, 2, 5, 10, 20, 30 y 40. Su funcionamiento ha sido correcto en todos los casos.

En los primeros casos nos daremos cuenta que se producen varios resize nada más empezar el programa (lo cuál es tremendamente costoso) y funciona correctamente situando la tabla inicial en 16. 

Una vez más, el tamaño recomendado es 64, pero las potencias de 2 suelen funcionar bien en las tablas hash. 

#### Tamaño del buffer

Por otra parte tenemos el tamaño del buffer (tanto del A como del B). Esta definición se encuentra en `sistemaEntrada/sistemaEntrada.h` y está por defecto definido en 32. Esta variable determina la capacidad de lectura que tiene el programa, pues el tamaño del buffer determina el tamaño máximo que puede tener un lexema, para evitar así errores.

Está por defecto en 32 para que muestre la expcepción que se lanza si un lexema es más grande que el buffer pero puede ser modificado para ver el funcionamiento del código.

Se recomienda poner un tamaño de potencia de dos como pueden ser los siguientes: 32, 64, 128, 256, 512, 1024 y 2048.

## Compilación

Para compilar el proyecto, asegúrate de tener instalado [GCC](https://gcc.gnu.org/) en tu sistema. El proyecto incluye un `Makefile` que simplifica el proceso de compilación.

Sigue estos pasos para compilar el proyecto:

1. Abre una terminal.
2. Navega al directorio raíz del proyecto.
3. Ejecuta el comando `make`.

Esto generará el ejecutable en el directorio `bin/` del proyecto.

## Uso

Para utilizar el compilador con un archivo de código fuente, sigue estos pasos:

```
./bin/miCompilador wilcoxon.py
```

#### Con Valgrind

Si no tienes valgrind instalado:

***Mac***:

```
brew install valgrind
```

***Linux***:

```
sudo apt install valgrind
```

Y comprobamos que se instalo correctamente:

```
valgrind --version
```

**Ejecución**:

```
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./bin/miCompilador wilcoxon.py
```


## Automátas

Los autómatas presentados en el código `analizadorLexico/analizadorLexico.c` son los mostrados a continuación. Cabe destacar que con objetivo de simplificar el código, pueden haberse cambiado algunas funcionalidades, siempre manteniendo la esencia de los autómatas.

### Delimitadores

![Delimitadores](automatasPNG/delAutomaton.jpg "Delimitadores")

### Operadores

![Operadores](automatasPNG/opAutomaton.jpg "Operadores")

### Strings

![Strings](automatasPNG/stringAutomaton.jpg "Strings")

### Ints

![Ints](automatasPNG/intAutomaton.jpg "Ints")

### Floats

![Floats](automatasPNG/floatAutomaton.jpg "Floats")



---

Gracias por visitar y explorar este proyecto de compilador Python Simplificado. Esperamos que este readme te haya sido de utilidad para entender tanto el uso del proyecto como su estructura. 
