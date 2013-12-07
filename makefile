CFLAGS = `sdl2-config --cflags` -O4 -Wall -pedantic -std=c99 -lm
TARGET = sdl
SOURCES =  $(TARGET).c
LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) 
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET)
