#include <SDL2/SDL.h>
#include <GL/glew.h>
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include "Engine.h"
#include <iostream>

int InitEngine(){
	SDL_Init(SDL_INIT_EVERYTHING);
    
    /*
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    if (!info){
        std::cout<<"No video info"<<std::endl;
    }
    const char* driver = SDL_GetCurrentVideoDriver(void);
    std::cout<<"Driver: "<<driver<<std::endl;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    */

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return 0;	
}

