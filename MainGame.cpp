#include "MainGame.h"
#include <iostream>
#include <string>
#include <Errors.h>
#include <Engine.h>
#include <ResourceManager.h>

MainGame::MainGame() : 
m_screenWidth(1024), 
m_screenHeight(768), 
m_boardSize(750),
m_moveNum(0),
m_turnColour(0),
m_gameState(GameState::PLAY),
m_curPiece(nullptr),
m_mouseClickPos(0),
m_mouseChange(0),
m_maxFPS(60.0f),
m_fps(0),
m_time(0) 
{
	m_boardStart = -m_boardSize/2;
	m_squareSize = m_boardSize/8;
	m_camera.init(m_screenWidth, m_screenHeight);
}

MainGame::~MainGame(){

}

void MainGame::run(bool verbose){
	initSystems();
	m_board.init(m_boardStart, m_squareSize, verbose);

	m_background.init(glm::vec4(-m_screenWidth/2-10, -m_screenHeight/2-10, m_screenWidth+20, m_screenHeight+20), "textures/dark_gray.png");
	m_border.init(glm::vec4(-m_screenHeight/2, -m_screenHeight/2, m_screenHeight,m_screenHeight), "textures/darkwood.png");
	m_border.setDepth(0.5f);
	//pawns piecetype = 0

	for (int i = 0; i < 8; i++) {
		m_board.setPiece(i,1, new Piece(m_boardStart, m_squareSize, glm::vec2(i, 1), 0, 0, "textures/Pieces/w_pawn.png"));
		m_board.setPiece(i,6, new Piece(m_boardStart, m_squareSize, glm::vec2(i, 6), 1, 0, "textures/Pieces/b_pawn.png"));
	}
	//rooks piecetype = 1
	m_board.setPiece(0,0, new Piece(m_boardStart, m_squareSize, glm::vec2(0, 0), 0, 1, "textures/Pieces/w_rook.png"));
	m_board.setPiece(0,7, new Piece(m_boardStart, m_squareSize, glm::vec2(0, 7), 1, 1, "textures/Pieces/b_rook.png"));
	m_board.setPiece(7,0, new Piece(m_boardStart, m_squareSize, glm::vec2(7, 0), 0, 1, "textures/Pieces/w_rook.png"));
	m_board.setPiece(7,7, new Piece(m_boardStart, m_squareSize, glm::vec2(7, 7), 1, 1, "textures/Pieces/b_rook.png"));
	//knights piecetype = 2
	m_board.setPiece(1,0, new Piece(m_boardStart, m_squareSize, glm::vec2(1, 0), 0, 2, "textures/Pieces/w_knight.png"));
	m_board.setPiece(1,7, new Piece(m_boardStart, m_squareSize, glm::vec2(1, 7), 1, 2, "textures/Pieces/b_knight.png"));
	m_board.setPiece(6,0, new Piece(m_boardStart, m_squareSize, glm::vec2(6, 0), 0, 2, "textures/Pieces/w_knight.png"));
	m_board.setPiece(6,7, new Piece(m_boardStart, m_squareSize, glm::vec2(6, 7), 1, 2, "textures/Pieces/b_knight.png"));
	//bishops piecetype = 3
	m_board.setPiece(2,0, new Piece(m_boardStart, m_squareSize, glm::vec2(2, 0), 0, 3, "textures/Pieces/w_bishop.png"));
	m_board.setPiece(2,7, new Piece(m_boardStart, m_squareSize, glm::vec2(2, 7), 1, 3, "textures/Pieces/b_bishop.png"));
	m_board.setPiece(5,0, new Piece(m_boardStart, m_squareSize, glm::vec2(5, 0), 0, 3, "textures/Pieces/w_bishop.png"));
	m_board.setPiece(5,7, new Piece(m_boardStart, m_squareSize, glm::vec2(5, 7), 1, 3, "textures/Pieces/b_bishop.png"));
	//queens piecetype = 4
	m_board.setPiece(3,0, new Piece(m_boardStart, m_squareSize, glm::vec2(3, 0), 0, 4, "textures/Pieces/w_queen.png"));
	m_board.setPiece(3,7, new Piece(m_boardStart, m_squareSize, glm::vec2(3, 7), 1, 4, "textures/Pieces/b_queen.png"));
	//kings piecetype = 5
	m_board.setPiece(4,0, new Piece(m_boardStart, m_squareSize, glm::vec2(4, 0), 0, 5, "textures/Pieces/w_king.png"));
	m_board.setPiece(4,7, new Piece(m_boardStart, m_squareSize, glm::vec2(4, 7), 1, 5, "textures/Pieces/b_king.png"));

	for (int j = 2; j < 6; j++){
		for (int i = 0; i < 8; i++){
			m_board.setPiece(i,j, nullptr);
		}
	}
	m_curPiece = m_board.piece(4,0); //initialize current piece to white king
	m_board.calculateAllMoves();	
	m_board.validateAllMoves(m_turnColour);
	gameLoop();
}

void MainGame::initSystems(){
	InitEngine();

	m_window.create("Chess", m_screenWidth, m_screenHeight, 0);

	initShaders();
	
	m_spriteBatch.init();
	m_fpsLimiter.init(m_maxFPS);	

}

void MainGame::initShaders(){
	m_colourProgram.compileShaders("shaders/shader.vs", "shaders/shader.fs");
	m_colourProgram.addAttribute("vertPosition");
	m_colourProgram.addAttribute("vertColour");
	m_colourProgram.addAttribute("vertUV");
	m_colourProgram.linkShaders();
}

void MainGame::gameLoop(){
	while (m_gameState != GameState::EXIT){
		m_fpsLimiter.begin();

		processInput();
		m_time += 0.01;

		m_camera.update();

		drawGame();

		m_fps = m_fpsLimiter.end();
		
		//print only once every 10 frames
		/*
		static int frameCounter = 0;
		frameCounter++;
		
		if (frameCounter == 10){
			std::cout << m_fps <<std::endl;
			frameCounter = 0;
		}
		*/
	}
}

void MainGame::processInput(){
	SDL_Event evnt;
	
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt) == true){
		switch (evnt.type){
			case SDL_QUIT:
				m_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				m_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			
			case SDL_KEYUP:
				m_inputManager.releaseKey(evnt.key.keysym.sym);
				break;

			case SDL_MOUSEBUTTONDOWN:
				m_inputManager.pressKey(evnt.button.button);
				break;

			case SDL_MOUSEBUTTONUP:
			{
				m_inputManager.releaseKey(evnt.button.button);
				glm::vec2 mouseCoords = m_camera.screenCoordsToWorld(m_inputManager.getMouseCoords());
				glm::vec2 mouseTile = m_board.getTile(mouseCoords, m_turnColour);
				glm::vec2 newPos = glm::vec2(m_board.getTile(m_curPiece->pos()+glm::vec2(m_squareSize/2, m_squareSize/2), m_turnColour));
				if (m_turnColour == 1) newPos = glm::vec2(7,7) - newPos;
				//if piece is being held
				if (m_curPiece -> isBeingHeld() && m_curPiece->colour() == m_turnColour){
					//move piece by dragging it
					if (m_curPiece ->isAMove(newPos)){
						m_board.movePiece(m_curPiece, newPos, m_turnColour);
						m_moveNum += 1;
						m_turnColour += 1;
						m_turnColour = m_turnColour%2;
					}
					//reset piece to original position if not a valid move
					else {
						m_board.resetPiece(m_curPiece);
					}
				}
				//if here we know it's not being held
				else if (m_curPiece -> isClickedOn() && m_curPiece->colour() == m_turnColour){
					//move piece to where move is shown 
					if (m_curPiece->isAMove(mouseTile)){
						m_board.movePiece(m_curPiece, mouseTile, m_turnColour);
						m_moveNum += 1;
						m_turnColour += 1;
						m_turnColour = m_turnColour%2;
       					}
					//remove display of moves if clicking somewhere else
					else {
						m_curPiece -> clickOff();
					}
				}
				m_curPiece -> drop();
				break;
			}

			case SDL_MOUSEMOTION:
				m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			
		}
	}
	if (m_inputManager.isKeyPressed(SDLK_w)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}

	if (m_inputManager.isKeyPressed(SDLK_s)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}	
	
	if (m_inputManager.isKeyPressed(SDLK_a)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	
	if (m_inputManager.isKeyPressed(SDLK_d)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}				
	
	if (m_inputManager.isKeyPressed(SDLK_q)) {
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
	}
	
	if (m_inputManager.isKeyPressed(SDLK_e)) {
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
	}
	
	if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = m_camera.screenCoordsToWorld(m_inputManager.getMouseCoords());
		glm::vec2 mouseTile = m_board.getTile(mouseCoords, m_turnColour);
		//still clicking on a piece
		if (m_curPiece -> isBeingHeld()){
			m_mouseChange = mouseCoords - m_mouseClickPos;
			m_curPiece->setPos(m_curPiece->tileToPos(m_curPiece->boardPos()) + m_mouseChange); 
		}
		//new click on a piece
		else if (m_board.isOccupied(mouseTile) != nullptr){
			if (m_board.piece(mouseTile) -> colour() == m_turnColour){
				m_curPiece -> clickOff();
				m_mouseChange = glm::vec2(0,0);
				m_mouseClickPos = mouseCoords;
				m_curPiece = m_board.piece(mouseTile);
				m_curPiece -> hold();
				m_curPiece->clickOn();
			}
		}
	}

}

void MainGame::drawGame(){

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colourProgram.use();

	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = m_colourProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//GLuint timeLocation = m_colourProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, m_time);

	GLuint PLocation = m_colourProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(PLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_spriteBatch.begin(GlyphSortType::FRONT_TO_BACK);
	
	m_background.draw(m_spriteBatch);
	m_border.draw(m_spriteBatch);

	m_board.draw(m_spriteBatch, m_turnColour);
	
	//m_spriteBatch.draw(pos, uv, texture.id, colour, 0.0f);
	
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colourProgram.unuse();	

	m_window.swapBuffer();
}



