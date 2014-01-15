CFLAGS = `sdl2-config --cflags` -O4 -Wall -pedantic -std=c99 -lm
INCS = programagotchi.h incdisplay.h neillsdl2_inc.h jump.h maze.h
TARGET = main
SOURCES =  display.c incdisplay.c neillsdl2_inc.c jump.c maze.c interpreter.c fcnreader.c $(TARGET).c
LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET)
