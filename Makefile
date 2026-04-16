CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Icls

LDFLAGS :=

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)
MAIN ?= main.cpp
TARGET := wheat_predictor

all: $(TARGET)

$(TARGET): $(OBJ) $(MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(MAIN) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
