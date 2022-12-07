CC = gcc
CFLAGS = -g -O3 -std=c99 -Wall -D_DEFAULT_SOURCE
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
TARGET = lattes

all: $(TARGET)

$(TARGET): $(OBJ)

clean:
	rm -f *.o vgcore* 

purge: clean
	rm -f $(TARGET)
