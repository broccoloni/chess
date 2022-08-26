#include "MainGame.h"
#include <iostream>
#include <string>
#include <Errors.h>
#include <Engine.h>
#include <ResourceManager.h>

MainGame::MainGame() : 
m_screenWidth(1600), 
m_screenHeight(900), 
m_boardSize(750),
m_moveNum(0),
m_turnColour(0),
m_boardOrientation(0),
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
    
    //NOTE: 0,0 is center of the screen
    //initialize background from 10 pixels outside screen 
    m_background.init(glm::vec4(-m_screenWidth/2-10, -m_screenHeight/2-10, m_screenWidth+20, m_screenHeight+20), "textures/dark_gray.png");
    m_background.setDepth(-2.0f);
    //initialize border around 8x8 chess board
    int borderWidth = 0.25*m_squareSize;
    m_border.init(glm::vec4(-4*m_squareSize - borderWidth, -4*m_squareSize - borderWidth, 
                            8*m_squareSize + 2 * borderWidth,8*m_squareSize + 2*borderWidth), "textures/darkwood.png");
    m_border.setDepth(-1.0f); //to have it behind the board
    
    //put flip board button in top right of window
    int buttonWidth = m_squareSize*3/4;
    int buttonHeight = buttonWidth/2;
    m_flipBoardButton.init(glm::vec4(m_screenWidth/2-buttonWidth,m_screenHeight/2-buttonHeight,
                                    buttonWidth,buttonHeight), "textures/dark_brown.png","textures/dark_brown.png");

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
	m_curPiece = m_board.isOccupied(4,0); //initialize current piece to white king - maybe initialize to nullptr??!?
	m_board.calculateAllMoves();    
    std::cout<<"******************************";
    std::cout<<" Starting Game ";
    std::cout<<"******************************";
    std::cout<<std::endl;
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
    int numMoves;

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
                //record event
                m_inputManager.releaseKey(evnt.button.button);
				
                //get mouse coordinates
                glm::vec2 mouseCoords = m_camera.screenCoordsToWorld(m_inputManager.getMouseCoords());
				
                //find tile mouse is over
                glm::vec2 mouseTile = m_board.getTile(mouseCoords, m_boardOrientation);
                //glm::vec2 mouseTile = m_board.getTile(mouseCoords, m_turnColour);
				
                if (m_board.isOnBoard(mouseTile)){
                    //find new position of piece -- look into this, dont remember what I did
                    glm::vec2 newPos = glm::vec2(m_board.getTile(m_curPiece->pos()+glm::vec2(m_squareSize/2, m_squareSize/2), m_boardOrientation));
                    //glm::vec2 newPos = glm::vec2(m_board.getTile(m_curPiece->pos()+glm::vec2(m_squareSize/2, m_squareSize/2), m_turnColour));
		    		
                    //invert if blacks turn - change this to rely on m_boardOrientation
                    if (m_boardOrientation == 1) newPos = glm::vec2(7,7) - newPos;
                    //if (m_turnColour == 1) newPos = glm::vec2(7,7) - newPos;

    				//if piece is being held
	    			if (m_curPiece -> isBeingHeld() && m_curPiece->colour() == m_turnColour){
		    			//move piece by dragging it

                        //verify it's a valid move
	    				if (m_curPiece -> isAMove(newPos)){
                            //Move the piece
			    			m_board.movePiece(m_curPiece, newPos);
    
                            //Update values
                            m_moveNum += 1;
			    			m_turnColour = 1 - m_turnColour;
                            numMoves = m_board.calculateNextTurnMoves(m_turnColour);
            
                            //Check if end of game
                            if (numMoves == -1) std::cout<<"DRAW!"<<std::endl;
                            else if (numMoves == 0){
                                if (m_turnColour == 0) std::cout<<"BLACK WINS!"<<std::endl;
                                else std::cout<<"WHITE WINS"<<std::endl;
                            }
                            else{
                                std::cout<<"##############################";
                                if (m_turnColour == 0) std::cout<<" White's Turn ";
                                else std::cout<< " Black's Turn ";
                                std::cout<<"##############################"<<std::endl;
                                std::cout<<"Legal Moves: "<<numMoves<<std::endl;
                            }   
                        }
		    			//reset piece to original position if not a valid move
			    		else {
				    		m_board.resetPiece(m_curPiece);
					    }
    				}
				    //If no piece is being held but there is a current clicked on piece
    				else if (m_curPiece -> isClickedOn() && m_curPiece->colour() == m_turnColour){
	    				//move piece by clicking to see moves and clicking on move
                        
                        //Check if it is a valid move
				    	if (m_curPiece->isAMove(mouseTile)){
                            //Move the piece
						    m_board.movePiece(m_curPiece, mouseTile);

                            //Update values
                            m_moveNum += 1;
		    				m_turnColour = 1 - m_turnColour;
                            numMoves = m_board.calculateNextTurnMoves(m_turnColour);
                            
                            //Check if end of game
                            if (numMoves == -1) std::cout<<"DRAW!"<<std::endl;
                            else if (numMoves == 0){
                                if (m_turnColour == 0) std::cout<<"BLACK WINS!"<<std::endl;
                                else std::cout<<"WHITE WINS"<<std::endl;
                            }
                            else{
                                std::cout<<"##############################";
                                if (m_turnColour == 0) std::cout<<" White's Turn ";
                                else std::cout<< " Black's Turn ";
                                std::cout<<"##############################"<<std::endl;
                                std::cout<<"Legal Moves: "<<numMoves<<std::endl;
                            }
       		    		}
                        //If not a valid move
                        else {
                            //Remove display of dots
                            m_curPiece->clickOff();
                            //m_curPiece = nullptr;
                        }
		    		}
                }
                //If the click location was not on the board, check if flip board button was clicked
                else{
                    if (m_flipBoardButton.isUnder(mouseCoords)){
                        m_boardOrientation = 1 - m_boardOrientation;
                    }
                }
				//Whether valid move or not, drop the current piece when mouse is lifted
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
        //get mouse coordinates
		glm::vec2 mouseCoords = m_camera.screenCoordsToWorld(m_inputManager.getMouseCoords());
		
        //find tile from mouse coordinates
        glm::vec2 mouseTile = m_board.getTile(mouseCoords, m_boardOrientation);
        //glm::vec2 mouseTile = m_board.getTile(mouseCoords, m_turnColour);
		
        //update position of piece if it's being held
		if (m_curPiece -> isBeingHeld()){
			m_mouseChange = mouseCoords - m_mouseClickPos;
			m_curPiece->setPos(m_curPiece->tileToPos(m_curPiece->boardPos()) + m_mouseChange); 
		}

		//clicking on a new piece
        //Check if tile is occupied by a piece
		else if (m_board.isOccupied(mouseTile) != nullptr){
            //If the piece is the players (not the opponents)
			if (m_board.isOccupied(mouseTile) -> colour() == m_turnColour){
                //Reset the mouse change to zero 
				m_mouseChange = glm::vec2(0,0);
                
                //record the initial click position
				m_mouseClickPos = mouseCoords;

                //set a new current piece
                //MAYBE SET CURRENT PIECE TO NULL POINTER IF THIS NEW CURRENT PIECE IS THE PREV ONE--need to fix dots 
				m_curPiece = m_board.isOccupied(mouseTile);
				m_curPiece -> hold();
                m_curPiece -> click();    
                //std::cout<<"CURRENT PIECE"<<std::endl;
                    //std::cout<<"location: "<<(int)m_curPiece->boardPos().x<<", "<<(int)m_curPiece->boardPos().y<<std::endl;
                    //std::cout<<"type: "<<m_curPiece->type()<<std::endl;
                    //std::cout<<"colour: "<<m_curPiece->colour()<<std::endl;
                    //std::cout<<"moves:"<<std::endl;
                    //for (unsigned int i = 0; i < m_curPiece->moves().size(); i++){
                    //    std::cout<<"Move "<<i<<": "<<(int)m_curPiece->moves()[i].x<<", "<<(int)m_curPiece->moves()[i].y<<std::endl;
                    //}
                    //std::cout<<std::endl;
            }
            else{
                m_curPiece->clickOff();
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
	
    //Draw square objects
	m_background.draw(m_spriteBatch);
	m_border.draw(m_spriteBatch);
    m_flipBoardButton.draw(m_spriteBatch);

    //Draw board
	m_board.draw(m_spriteBatch, m_boardOrientation);
	//m_board.draw(m_spriteBatch, m_turnColour);
	
	//m_spriteBatch.draw(pos, uv, texture.id, colour, 0.0f);
	
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colourProgram.unuse();	

	m_window.swapBuffer();
}



