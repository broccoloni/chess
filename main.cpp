#include <iostream>
#include "MainGame.h"
#include <glm/glm.hpp>

int main(int argc, char* argv[]){
	bool verbose = false;
	if (argc == 2) verbose = true;
	MainGame mainGame;
	mainGame.run(verbose);	
	return 0;
}
