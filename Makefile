# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++17 -Wall -m64 -g -I./my_engine

# Linker flags
LDFLAGS := -s
LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lGL -lGLEW -lglfw

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin/debug

# Source and object files
SRC := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Engine source and object files
ENGINE_SRC := $(shell find my_engine -name '*.cpp')
ENGINE_OBJ := $(patsubst my_engine/%.cpp,$(OBJ_DIR)/my_engine/%.o,$(ENGINE_SRC))

# Target executable
TARGET := $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ) $(ENGINE_OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) $(ENGINE_OBJ) -o $@ $(LDFLAGS) $(LIBS)

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)  # Create subdirectories in obj/ as needed
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilation of engine source files
$(OBJ_DIR)/my_engine/%.o: my_engine/%.cpp | $(OBJ_DIR)/my_engine
	@mkdir -p $(dir $@)  # Create subdirectories in obj/ as needed
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/my_engine:
	mkdir -p $(OBJ_DIR)/my_engine

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	rm -rf $(OBJ_DIR)

# Phony targets
.PHONY: all clean
