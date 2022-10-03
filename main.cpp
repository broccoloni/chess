#include <iostream>
#include "MainGame.h"
#include <glm/glm.hpp>
//#include <tensorflow/c/c_api.h>
int main(int argc, char* argv[]){
	bool verbose = false;
    unsigned int n = 1;
    unsigned int result;
    //std::cout<<"TensorFlow C library version: "<<TF_Version()<<std::endl;
    /*mode:
     * 0 - full display
     * 1 - no display but some text, human controlled
     * 2 - no display at all, ai controlled
     */
    unsigned int mode = 0; //default is full display
    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "--help")==0 || strcmp(argv[i], "-h")==0){
            std::cout<<"To run the program, run ./Chess in the terminal"<<std::endl;
            std::cout<<"Optional flag arguments:"<<std::endl;
            std::cout<<" --help or -h:\tPrints these instructions on how to run the program"<<std::endl;
            std::cout<<" --verbose or -v:\tPrints what the program is doing as it's running"<<std::endl;
            std::cout<<" --no-display or -nd:\tRuns the program with no graphical display. The game"<<std::endl;
            std::cout<<"\t\t\tthen runs in the command line, prompting the user(s) for moves"<<std::endl;
            std::cout<<" --numgames or -n:\tThe number after the flag is how many times the game will be run"<<std::endl;
            std::cout<<" --train-ai or -ai:\tTrains the AI bot against itself. The number after the flag is how"<<std::endl;
            std::cout<<"\t\t\tmany games the ai will train on"<<std::endl;
            std::cout<<std::endl;
            return 0;
        }
        
        if (strcmp(argv[i], "--verbose")==0 || strcmp(argv[i], "-v")==0) verbose = true;
        if (strcmp(argv[i], "--no-display")==0 || strcmp(argv[i], "-nd")==0) mode = 1;
        if (strcmp(argv[i], "--numgames")==0 || strcmp(argv[i], "-n")==0){
            n = atoi(argv[i+1]);
        }
        if (strcmp(argv[i], "--train-ai")==0 || strcmp(argv[i], "-ai")==0){
            mode = 2;
            n = atoi(argv[i+1]);
        }
        
    }
    MainGame mainGame(verbose,mode);
    for (unsigned int i = 0; i < n; i++){
	    result = mainGame.run();	
        std::cout<<"Result: "<<result<<std::endl;
    }
    return 0;
}
