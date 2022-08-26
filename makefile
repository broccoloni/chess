CC = g++
FLAGS = -Wall
LIBS = -LGameEngine/
INCS = -IGameEngine/

all: Chess

Chess: main.cpp MainGame.cpp Board.cpp Piece.cpp Square.cpp
	$(CC) $(FLAGS) $(INCS) $(LIBS) -o $@ $^ -lGL -lGLEW -lSDL2 -ggdb -lengine


clean:
	rm *.o



