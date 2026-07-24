CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Icls -MMD -MP
LDFLAGS  :=

SRC    := $(wildcard src/*.cpp)
OBJ    := $(SRC:.cpp=.o)
DEPS   := $(OBJ:.o=.d)
MAIN   ?= main.cpp
TARGET := wheat_predictor

# Тести
TESTS_SRC    := tests/test_yield.cpp
TEST_TARGET  := wheat_predictor_tests
TEST_LDFLAGS := -lCatch2Main -lCatch2

all: $(TARGET)

$(TARGET): $(OBJ) $(MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(MAIN) $(LDFLAGS)

# Компіляція та запуск тестів (Catch2 v3 — системний пакет)
test: $(OBJ) $(TESTS_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(OBJ) $(TESTS_SRC) $(LDFLAGS) $(TEST_LDFLAGS)
	./$(TEST_TARGET)

# Очищення
clean:
	rm -f $(OBJ) $(DEPS) $(TARGET) $(TEST_TARGET)

# Підключення автоматично згенерованих файлів залежностей
-include $(DEPS)

.PHONY: all test clean
