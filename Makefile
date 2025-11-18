CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -Isrc
TEST_TARGET = build/tests.exe

SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
TEST_DIR = tests

COMMON_SOURCES = $(wildcard $(CORE_DIR)/*.cpp)
COMMON_OBJECTS = $(COMMON_SOURCES:$(CORE_DIR)/%.cpp=build/core/%.o)

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=build/tests/%.o)

build:
	@mkdir -p build/core
	@mkdir -p build/tests

test: build $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(COMMON_OBJECTS) | build
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS) $(COMMON_OBJECTS)

build/tests/%.o: $(TEST_DIR)/%.cpp | build
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR) -c $< -o $@

build/core/%.o: $(CORE_DIR)/%.cpp | build
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-tests: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf build

.PHONY: test run-tests clean build