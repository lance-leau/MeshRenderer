# ==== Makefile for 3D Renderer ====

CXX := g++
CXXFLAGS := -g -Wall -Wextra -pedantic
LDFLAGS := -lSDL2

SRC := main.cc mesh.cc model.cc projector.cc renderer.cc vertex.cc parseOBJ.cc
OBJ := $(SRC:.cc=.o)
TARGET := main

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild from scratch
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
