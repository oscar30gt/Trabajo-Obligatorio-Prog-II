# Variables
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
SRC := src/main.cpp src/GSenku.cpp
OBJ_DIR := .out
OBJ := $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)
OUT := main

# Crear las carpetas de salida si no existen
$(shell mkdir $(OBJ_DIR))

# Regla por defecto
all: $(OUT)

# Regla de enlazado
$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regla de compilación
$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJ) $(OUT)

.PHONY: all clean