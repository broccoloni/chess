#include "Window.h"
#include "Errors.h"

#include <iostream>
#include <string>

Window::Window(){

}

Window::~Window(){

}

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags){
	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE){ 
		flags |= SDL_WINDOW_HIDDEN;
	}

	if (currentFlags & FULLSCREEN){
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (currentFlags & BORDERLESS){
		flags |= SDL_WINDOW_BORDERLESS;
	}

	m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

    if (m_sdlWindow == nullptr) {
            fatalError("SDL Window could not be created!");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
    if (glContext == nullptr) {
            fatalError("SDL_GL context could not be created!");
    }

    //if (glewInit() != GLEW_OK){
    //        fatalError("Could not initialize glew!");
    //}

    //check the OpenGL version to make sure its >= 3
    std::printf("***\tOpenGL Version: %s\t***\n", glGetString(GL_VERSION));

    //string ESstr = "OpenGL ES";
    //for (unsigned int i = 0; i < 9; i++){
    //    if (strcmp(glGetString(GL_VERSION).substr(0,9),"OpenGL ES")){
    //}

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); //set background colour

    //turns on vertical syncronization (VSYNC) if input is 1, turns it off if input is 0
    SDL_GL_SetSwapInterval(1);	

	//Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer(){
	SDL_GL_SwapWindow(m_sdlWindow);	
}














