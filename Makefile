# Define the compiler
CXX = g++
CXXFLAGS = -std=c++14 -Wall

# Define directories
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

# Define the output executable
TARGET = $(BUILD_DIR)/assembler

# Define source and header files
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/SymbolTable.cpp $(SRC_DIR)/Parser.cpp $(SRC_DIR)/Code.cpp
HEADERS = $(INCLUDE_DIR)/SymbolTable.h $(INCLUDE_DIR)/Parser.h $(INCLUDE_DIR)/Code.h

# Default rule to build the project
$(TARGET): $(SOURCES) $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $(SOURCES) -o $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)/*

