CC = gcc
LIBS = -lSDL2main -lSDL2 -lm
INCLUDES = -Iinclude
FILES = main.c simulation.c graphismes.c

all:
	$(CC) $(INCLUDES) $(FILES) $(LIBS) -o prog