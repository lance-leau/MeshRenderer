CXX := g++
CXXFLAGS := -g -Wall -Wextra -pedantic -std=c++17
LDFLAGS := -lSDL2

# Automatically find all source files
SRC := $(wildcard src/**/*.cc) main.cc
OBJ := $(SRC:.cc=.o)
TARGET := main

# Collect all subdirectories in src/ as include paths
INCLUDE_DIRS := $(shell find src -type d)
CXXFLAGS += $(addprefix -I, $(INCLUDE_DIRS))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
