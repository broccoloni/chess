CC = g++
FLAGS = -Wall
LIBS = -L$(HOME)/Desktop/GameEngine/
INCS = -I$(HOME)/Desktop/GameEngine/

all: Chess

Chess: main.cpp MainGame.cpp Board.cpp Piece.cpp Background.cpp
	$(CC) $(FLAGS) $(INCS) $(LIBS) -o $@ $^ -lGL -lGLEW -lSDL2 -ggdb -lengine


clean:
	rm *.o



