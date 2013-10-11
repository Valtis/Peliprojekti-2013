CC = g++
CFLAGS = -c -std=c++11 -Iinclude -I/usr/include/SDL2
LDFLAGS = -lSDL2
EXECUTABLE = bin/Peli
SOURCES := main.cpp $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

