# chess

This is my implementation of a chess game.
I wrote the game engine in the GameEngine directory following a youtube tutorial, and used it to implement this chess game.
I designed the game to be similar to popular online chess implementations such as chess.com and lichess.com. One can click
on a piece to select it, displaying all valid moves of that piece. Then click on one of the moves to move the piece there.
Drag and drop can also be used to move the pieces, and a flip board button and auto-flip board on turn button are in the 
top right. 

To compile the game, run make in the directory with the makefile (one may need to cd into the GameEngine directory and run
./createlibrary.sh first). The  game requires SDL2 and openGL. I'm currently working on adding support for embedded systems
as I have a chromebook that would require that. To play the game, run ./Chess in the terminal, and use the -h or --help 
flag to see other optional flags (Note: the AI hasn't been implemented yet).

Here's a video of sample play (Note: the mouse was flipped by the screen recording and not by the game)
<video src="Images/chess_example.mov" width = 180/> 
