#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <Sprite.h>
#include <GLSLProgram.h>
#include <GLTexture.h>
#include <vector>
#include <Window.h>
#include <Camera2D.h>
#include <SpriteBatch.h>
#include <InputManager.h>
#include <Timing.h>
#include "Board.h"
#include "Piece.h"
#include "Square.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
	public:
		MainGame();
		~MainGame();

		void run(bool verbose);


	private:
		void initSystems();
		void initShaders();
		void gameLoop();
		void processInput();
		void drawGame();

		Window m_window;
		int m_screenWidth;
		int m_screenHeight;
		int m_boardSize;
		int m_boardStart;
		int m_squareSize;
		int m_moveNum;
		int m_turnColour;
        int m_boardOrientation;

		GameState m_gameState;
		GLSLProgram m_colourProgram;
		Camera2D m_camera;
		SpriteBatch m_spriteBatch;
		InputManager m_inputManager;
		FPSLimiter m_fpsLimiter;
		Board m_board;
		Piece* m_curPiece;
		glm::vec2 m_mouseClickPos;
		glm::vec2 m_mouseChange;
		Square m_background;
		Square m_border;
        Square m_flipBoardButton;

		float m_maxFPS;
		float m_fps;
		float m_time;
};
