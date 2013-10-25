CC = g++
CFLAGS = -c -std=c++11 -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL_ttf
EXECUTABLE = ./bin/Peli
SOURCES := main.cpp $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
