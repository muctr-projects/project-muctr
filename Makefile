CXX = g++
CXXFLAGS = -g -fopenmp -Isrc
SOURCES = src/main.cpp src/core/newton_interpolator.cpp src/core/gnuplot_visualizer.cpp
TARGET = build/main.exe

build: $(TARGET)

$(TARGET): $(SOURCES)
	@if not exist build mkdir build
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	cd build && main.exe

clean:
	del /Q build 2>nul || exit 0