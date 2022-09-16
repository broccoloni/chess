#include <iostream>
#include "MainGame.h"
#include <glm/glm.hpp>

int main(int argc, char* argv[]){
	bool verbose = false;
    bool showdisplay = true;
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "--no-display")==0 || strcmp(argv[i], "-nd")==0) showdisplay = false;
        if (strcmp(argv[i], "--verbose")==0 || strcmp(argv[i], "-v")==0) verbose = true;
    }
    MainGame mainGame(verbose,showdisplay);
	mainGame.run();	
	return 0;
}
