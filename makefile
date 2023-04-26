CC = g++
FLAGS = -Wall
LIBS = -LGameEngine/
INCS = -IGameEngine/

all: Chess Chess.exe

Chess: main.cpp MainGame.cpp Board.cpp Piece.cpp Square.cpp BoardStates.cpp
	$(CC) $(FLAGS) $(INCS) $(LIBS) -o $@ $^ -lGL -lGLEW -lSDL2 -ggdb -lengine 

CC = x86_64-w64-mingw32-g++
FLAGS = -Wall
LIBS = -L/usr/lib/ -LGameEngine/
INCS = -I/usr/include/ -IGameEngine/

Chess.exe: main.cpp MainGame.cpp Board.cpp Piece.cpp Square.cpp BoardStates.cpp
	$(CC) $(FLAGS) $(INCS) $(LIBS) -o $@ $^ -lGL -lGLEW -lSDL2 -ggdb -lengine 

clean:
	rm *.o



