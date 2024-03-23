CC = gcc
CFLAGS = -Wall
OBJ_DIR = obj
BIN_DIR = bin

# Encuentra todos los archivos .c en los directorios del proyecto, excepto en src ya que no existe.
SOURCES = $(wildcard *.c) $(wildcard */*.c)
# Convierte los archivos .c encontrados a rutas .o en el directorio obj.
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/miCompilador

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean
