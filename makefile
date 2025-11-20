#CC=clang++
CC=gcc
CFLAGS=-c -Wall -pedantic -std=c11 
LDFLAGS=
SOURCES=Galkasm.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=Galkasm.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm ./*.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

