CXX = g++
CXXFLAGS = -g -fopenmp -Isrc
SOURCES = src/main.cpp src/core/newton_interpolator.cpp src/core/gnuplot_visualizer.cpp

# Определение ОС
ifeq ($(OS),Windows_NT)
    TARGET = build/main.exe
    TEST_TARGET = build/test.exe
    MKDIR = @if not exist build mkdir build
    RUN_CMD = cd build && main.exe
    TEST_CMD = cd build && test.exe
    RM = del /Q build 2>nul || exit 0
else
    TARGET = build/main
    TEST_TARGET = build/test
    MKDIR = @mkdir -p build
    RUN_CMD = cd build && ./main
    TEST_CMD = cd build && ./test
    RM = rm -rf build
endif

# Тестовые файлы
TEST_SOURCES = tests/test_main.cpp tests/test_utils.cpp tests/test_newton_interpolator.cpp tests/test_gnuplot_visualizer.cpp src/core/newton_interpolator.cpp src/core/gnuplot_visualizer.cpp

build: $(TARGET)

$(TARGET): $(SOURCES)
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	$(RUN_CMD)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -Itests -o $(TEST_TARGET) $(TEST_SOURCES)

run-tests: $(TEST_TARGET)
	$(TEST_CMD)

clean:
	$(RM)