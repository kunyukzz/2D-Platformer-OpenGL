# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++17 -Wall -m64 -g

# Linker flags
LDFLAGS := -s
LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lGL -lGLEW -lglfw

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin/debug

# Source and object files
#SRC := $(wildcard $(SRC_DIR)/*.cpp)
SRC := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Target executable
TARGET := $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)  # Create subdirectories in obj/ as needed
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	rm -rf $(OBJ_DIR)

# Phony targets
.PHONY: all clean
