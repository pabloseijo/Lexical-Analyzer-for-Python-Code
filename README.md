# Lexical Analyzer for Python Code

This project aims to create a lexical analyzer that processes Python code, specifically targeting a file named `wilcoxon.py`. The analyzer identifies and returns the lexical components found within the source code. This document outlines the structure and implementation approach for the analyzer, divided into several key components for modularity and ease of development.

## Project Structure

The lexical analyzer project is organized into five main files, each serving a distinct purpose:

1. **Definitions File**: Contains the definitions of Python's lexical components used in `wilcoxon.py`. Each lexical component is associated with an integer value, which the lexical analyzer returns upon recognition. This file also defines the symbol table as a vector of structures to store each lexical component and its corresponding lexeme where applicable.

2. **Lexical Analyzer File**: Hosts the core function of the project, which iterates over the source code to identify and return the next lexical component. This function interacts with the input system to read characters from the source code, using a switch statement to simulate the operation of finite automata for pattern recognition.

3. **Symbol Table File**: Contains essential functions for managing the symbol table, including search and insertion operations. These functions prevent unnecessary duplication of lexical components within the table.

4. **Input System File**: Manages access to the `wilcoxon.py` file, treating it as a stream of characters. It includes functions for retrieving the next character from the source code and for returning characters to the stream if needed.

5. **Error Management File**: Handles any errors that may occur during the lexical analysis process.

Additionally, a driver file is suggested for invoking the lexical analyzer, displaying each lexical component found in the source code for evaluation purposes.

## Implementation Notes

- The **definitions file** should include constants for lexical components, such as `#define IMPORT 273` and `#define RETURN 274`, and the structure for the symbol table.

- The **lexical analyzer file** should implement a function, possibly named `next_lexical_component()`, that uses a switch statement and a loop to process input characters and determine the corresponding lexical component.

- The **symbol table file** should implement search and insertion functions to manage lexical components efficiently.

- The **input system file** should provide functionality to read characters from `wilcoxon.py` and manage the character stream.

- Useful C functions for this project include `isalpha()`, `isdigit()`, `isalnum()`, `atoi()`, `atof()`, `getc()`, and `strcpy()`.

## Usage

To use the lexical analyzer, compile all the source files and run the driver program. The analyzer will process `wilcoxon.py`, identifying and displaying the lexical components found within the code.

## Contributing

Contributions to improve the lexical analyzer are welcome. Please feel free to fork the repository, make changes, and submit a pull request with your enhancements.

## License

This project is open-source and available under the MIT license. See the LICENSE file for more details.
